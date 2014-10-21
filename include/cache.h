/*******************************************************************************
/
/  filename: cache.h
/  description: Interface functions for a simple cache simulator, includeing
/               necessary structures and functions.
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

#pragma once

#include <stdbool.h>

typedef unsigned int Index;
typedef unsigned int Tag;
typedef struct {
  Tag tag;
  bool valid;
  bool dirty;
}Cache;

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
  unsigned set_size;
} CacheConf;

/**
 * `is_hit` will, given a cache and its associativity, look up a given index-tag
 * pair. If it is found in the cache, it will return true. Else it will return
 * false.
 *
 * @param cache Array of cache slots to be queried.
 * @param direct_mapped Flaggs whether this is a direct or associative lookup.
 * @param blocks Number of blocks in the cache.
 * @param sets Number of sets of blocks in the cache (used for associative)
 * @param tag Tag used to check if the content in a slot is the desired content.
 * @param index The direct index to use if cache is mapped.
 *
 * @return If the memory was in cache or not.
 */
bool is_hit(Cache* cache, int associativity, int setsize, Tag tag,
            Index index);

/**
 * Like `is_hit` but used for direct-mapped cache. Called by `is_hit`.
 *
 * @param cache Array of cache slots to be queried.
 * @param tag Tag used to check if the content in a slot is the desired content.
 * @param index The direct index to lookup.
 *
 * @return If the memory was in cache or not.
 */
bool is_hit_direct(Cache* cache, Tag tag, Index index);

/**
 * Like `is_hit` but used for fully associative cache. Called by `is_hit`.
 *
 * @param cache Array of cache slots to be queried.
 * @param setsize The length of cache block set.
 * @param tag Tag used to check if the content in a slot is the desired content.
 *
 * @return If the memory was in cache or not.
 */
bool is_hit_associative(Cache* cache, int associativity, int setsize, Tag tag,
                        Index index);

/**
 *
 */
int sim(Cache* cache, CacheConf *config, Tag tag, Index index, bool read);
void replacement(Cache* cache, CacheConf *config, Tag tag, Index index);
void random_replacement(Cache* cache, int associativity,
                        int setsize, Tag tag, Index index);
void FIFO_replacement(Cache* cache, int associativity,
                      int setsize, Tag tag, Index index);
