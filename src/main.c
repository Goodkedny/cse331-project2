#include <stdio.h>

#include "cache.h"
// 
// Config file
// Read line : Returns (Hit/Miss, store/load, instructions)
// --> sim_load : Return Hit/Miss
// ----> is_hit : Returns t/f
// ----> on_load_miss   (replace policy...) : void and will have side effects
// --> sim_store : Return Hit/Miss
// ----> on_store_miss : void and will have side effects
// Simulate : Returns (Total Load, Load Hits, Total Store,
//                     Store Hits, Simulated Clock Time)
// Make output : Void, writes simulate results to file.

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
