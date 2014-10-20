#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "cache.h"

SimResult simulate(char *trace);


int print_results(FILE *fp);

/**
 * Parses a configuration file and returns a CacheConf structure filled with the
 * gathered information.
 *
 * @param config, the path to the config file to parse
 * @return populated cache configuration structure
 */
CacheConf build_config(char *config);

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
  printf("Cache Sim!\n");
  
  return 0;
}
