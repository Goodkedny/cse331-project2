#include "cache.h"
#include "sim.h"

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
