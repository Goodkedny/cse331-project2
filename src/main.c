#include <stdio.h>

#include "cache.h"

SimResult simulate(char *trace);
int print_results(FILE *fp);
int build_config(char *config);

int main(int argc, char *argv[])
{
  (void) argc;
  (void) argv;
  printf("Cache Sim!\n");
  return 0;
}
