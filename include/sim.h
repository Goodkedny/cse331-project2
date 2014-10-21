#pragma once
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

SimResult simulate(CacheConf *config, char *trace);

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
