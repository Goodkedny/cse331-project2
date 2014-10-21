/*******************************************************************************
/
/  filename: sim.c
/  description: Implementation file for sim.h. Implements all io and the runtime
/               for the cache.
/
/  Authors: Cecil, Tyler
/           VanWhy, Randy
/
/  Class: CSE 331
/  Instructor: Zheng
/  Assignment: Lab Project 2
/
/  Assigned: October 2, 2014
/  Due: October 22/ 2014
/
/******************************************************************************/
#include "sim.h"

void print_results(CacheConf *config, SimResult results, FILE *fp)
{
  unsigned long total_memory_accesses = results.load_total + results.store_total;
  unsigned long total_hits = results.load_hit + results.store_hit;
  unsigned long store_misses = results.store_total - results.store_hit;
  unsigned long load_misses = results.load_total - results.load_hit;
  unsigned long total_miss_penalty = (store_misses + load_misses) * config->miss_penalty;


  float total_hit_rate = (float) total_hits / total_memory_accesses;
  float load_hit_rate = (float) results.load_hit / (float) results.load_total;
  float store_hit_rate = (float) results.store_hit / results.store_total;
  float average_memory_access_latency = (float) (total_miss_penalty + total_hits)/ total_memory_accesses;

  fprintf(fp, "%f,", total_hit_rate);
  fprintf(fp, "%f,", load_hit_rate);
  fprintf(fp, "%f,", store_hit_rate);
  fprintf(fp, "%lu,", results.instructions);
  fprintf(fp, "%f\n", average_memory_access_latency);
}

SimResult simulate(CacheConf *config, char *trace)
{
  /*2-4-6-8 Show me how you simultate!*/
  SimResult result;
  result.load_hit = 0;
  result.store_hit = 0;
  result.load_total = 0;
  result.store_total = 0;
  result.instructions = 0;

  Cache *cache = malloc(sizeof(Cache)*config->cache_size);

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
      result.load_hit += (1 - sim_result);
      result.load_total++;
    }
    else {
      result.store_hit += (1 - sim_result);
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
  address <<= config->tag_size;
  return address >> (config->offset_bits + config->tag_size);
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

  if (configuration.associativity == 1) {
    configuration.set_size = configuration.num_sets;
  }
  else if (configuration.associativity > 1) {
    configuration.set_size = configuration.associativity;
  }
  else {
    configuration.set_size = 1;
  }

  /*We're using 32-bit addresses. To get tag length, subtract numsets and offset
   from 32*/

  configuration.tag_size = 32 - log2((float)configuration.num_sets/(configuration.associativity ? configuration.associativity:1))
      - configuration.offset_bits;

  fclose(config_file);
  return configuration;
}
