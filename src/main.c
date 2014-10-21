#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "cache.h"

/**
 * Parses a configuration file and returns a CacheConf structure filled with the
 * gathered information.
 *
 * @param config, the path to the config file to parse
 * @return populated cache configuration structure
 */
CacheConf build_config(char *config);

/**
 * Extracts the tag bits from the given 32 bit address.
 *
 * @param config, the configuration of the cache.
 * @param address, the address to extract the tag from.
 * @return the tag
 */
Tag calculate_tag(CacheConf *config, unsigned int address);

/**
 * Extracts the index bits from the given 32 bit address.
 *
 * @param config, the configuration of the cache.
 * @param address, the address to extract the index from.
 * @return the index
 */
Index calculate_index(CacheConf *config, unsigned int address); 

/**
 * Prints the results of a simulation to a file
 *
 * @param results, the result of the simulation
 * @param fp, the file to print to
 * TODO: Write this function
 */
void print_results(SimResult results, FILE *fp);

SimResult simulate(CacheConf *config, char *trace) 
{
  SimResult result;
  Cache *cache = malloc(sizeof(Cache) * config->num_sets);

  FILE *trace_file;
  trace_file = fopen(trace, "r");

  if (trace_file == NULL) {
    fprintf(stderr, "Cannot open file.");
    exit(1);
  }

  char buffer[100];
  char inst_type;
  unsigned int address;
  int inst_since_last;
  int sim_result;
  while(fgets(buffer, 100, trace_file)) {
    sscanf(buffer, "%c %x %d", &inst_type, &address, &inst_since_last);
    sim_result = sim(cache, config, calculate_tag(config, address), 
	calculate_index(config, address), inst_type == 'l');
    
    if (inst_type == 'l') {
      result.load_hit += 1 - sim_result;
      result.load_total++;
    }
    else {
      result.store_hit += 1 - sim_result;
      result.store_total++;
    }
    
    result.instructions += inst_since_last;
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
  for (unsigned i = 1 << (32 - config->tag_size); i < config->tag_size; i <<= 1) {
    address ^= i;
  }
  
  return address >> config->offset_bits;
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



int main(int argc, char *argv[])
{
  CacheConf cacheConf;

  if(argc != 3) {
    printf("Usage: sim <config_file> <trace_file>\n");
    exit(1);
  }

  cacheConf = build_config(argv[1]);
  simulate(&cacheConf, argv[2]);
  printf("Cache Sim!\n");
  
  return 0;
}
