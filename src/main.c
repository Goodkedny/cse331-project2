/*******************************************************************************
/
/  filename: main.c
/  description: Main file to load and start the cache simulator.
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
#include "cache.h"
#include "sim.h"

#include <string.h>

int main(int argc, char *argv[])
{
  CacheConf cacheConf;
  SimResult results;

  if(argc != 3) {
    printf("Usage: sim <config_file> <trace_file>\n");
    exit(1);
  }

  cacheConf = build_config(argv[1]);
  results = simulate(&cacheConf, argv[2]);

  char *filename = malloc(strlen(argv[2]) + 5);
  sprintf(filename, "%s.out", argv[2]);

  FILE *fp = fopen(filename, "w");


  print_results(&cacheConf, results, fp);  

  fclose(fp);
  free(filename);

  return 0;
}
