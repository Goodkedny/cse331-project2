#pragma once

typedef unsigned long Cache;

typedef struct {
  unsigned long load_hit;
  unsigned long load_total;
  unsigned long store_hit;
  unsigned long store_total;
  unsigned long instructions;
} SimResult;

typedef struct {
  unsigned line_size;
  unsigned associativity;
  unsigned cache_size;
  unsigned replacement_policy;
  unsigned miss_penalty;
  unsigned write_allocate;
  unsigned tag_size;
  unsigned num_sets;
  unsigned offset_bits;
} CacheConf;

int is_hit();
int is_hit_direct();
int is_hit_associative();

SimResult sim_load();
int on_load_miss();
int load_random_replacement();
int load_FIFO_replacement();

SimResult sim_store();
int on_store_miss();
int store_no_write_allocate();
// Call on_load_miss 
int store_write_allocate();
