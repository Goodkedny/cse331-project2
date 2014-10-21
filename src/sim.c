#include "sim.h"


SimResult simulate(CacheConf *config, char *trace) 
{
  /*2-4-6-8 Show me how you simultate!*/
  SimResult result;
  Cache *cache = malloc(sizeof(Cache) * config->num_sets);

  FILE *trace_file;
  trace_file = fopen(trace, "r");

  if (trace_file == NULL) {
    fprintf(stderr, "Cannot open file.");
    exit(1);
  }

  /*We now assume that the file is valid*/
  /*For each instruction simulation we need the following variables*/
  char inst_type;
  unsigned int address;
  int inst_since_last;
  int sim_result;
    

  char buffer[100];
  while(fgets(buffer, 100, trace_file)) {
    /*Read a formatted line from the trace file*/
    sscanf(buffer, "%c %x %d", &inst_type, &address, &inst_since_last);

    /*Calculate the tag and the index and simulate the instruction*/
    sim_result = sim(cache, config, calculate_tag(config, address), 
	calculate_index(config, address), inst_type == 'l');
    
    /*Add the result of the instruction to the count*/
    if (inst_type == 'l') {
      result.load_hit += 1 - sim_result;
      result.load_total++;
    }
    else {
      result.store_hit += 1 - sim_result;
      result.store_total++;
    }
    
    /*Add the miss penalty*/
    if (sim_result) {
      result.instructions += config->miss_penalty;
    }

    /*Add the instructions since last access and 1 for the access itself.*/
    result.instructions += inst_since_last + 1;
  }

  fclose(trace_file);
  free(cache);
  return result;
}

Tag calculate_tag(CacheConf *config, unsigned int address) 
{
  return address >> (32 - config->tag_size);
}

Index calculate_index(CacheConf *config, unsigned int address) 
{
  address <<= config->tag_size
  
  return address >> config->offset_bits + config->tag_size;
}

CacheConf build_config(char *config)
{
  FILE *config_file;
  config_file = fopen(config, "r");

  if (config_file == NULL) {
    fprintf(stderr, "Cannot open file.");
    exit(1);
  }

  /*At this point we will assume the config file is valid*/
  CacheConf configuration;
  fscanf(config_file, "%u", &configuration.line_size);
  fscanf(config_file, "%u", &configuration.associativity);
  fscanf(config_file, "%u", &configuration.cache_size);
  fscanf(config_file, "%u", &configuration.replacement_policy);
  fscanf(config_file, "%u", &configuration.miss_penalty);
  fscanf(config_file, "%u", &configuration.write_allocate);

  /*number of sets = cache size / line size*/
  configuration.num_sets = configuration.cache_size / configuration.line_size;
  configuration.offset_bits = (int) log2(configuration.line_size);

  /*We're using 32-bit addresses. To get tag length, subtract numsets and offset
   from 32*/
  configuration.tag_size = 32 - configuration.num_sets
    - configuration.offset_bits;

  fclose(config_file);
  return configuration;
}


