#pragma once

typedef unsigned long Cache;

typedef struct {
  unsigned long load_hit;
  unsigned long load_total;
  unsigned long store_hit;
  unsigned long store_total;
  unsigned long instructions;
} SimResult;

int is_hit();
int on_load_miss();
int on_store_miss();
SimResult sim_load();
SimResult sim_store();
