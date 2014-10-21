#include "cache.h"
#include "sim.h"

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
  
  FILE *fp = fopen("out","w");
  print_results(&cacheConf, results, fp);  
  fclose(fp);

  return 0;
}
