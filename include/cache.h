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

/**
 * Typdef for the Index of a memory location. Used to determind cach block.
 */
typedef unsigned int Index;

/**
 * Typedef for the Tag of a memory location. Used to determin match.
 */
typedef unsigned int Tag;

/**
 * Simple struct used to represent a cache line.
 */
typedef struct {
  Tag tag;
  bool valid;
  bool dirty;
}Cache;

/**
 * SimResult is used to store all information about the simulation. It is used
 * by the simulator runtime environment.
 */
typedef struct {
  unsigned long load_hit;
  unsigned long load_total;
  unsigned long store_hit;
  unsigned long store_total;
  unsigned long instructions;
} SimResult;

/**
 * CacheConf contains all settings for the cache. In many cases, the values
 * are redundant, and simply added for convenience. This is where we store
 * information like line size, cache size, replacement policy, write allocation
 * policy, ect..
 */
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
 * @param associativity Rank of the associativity of the cache.
 * @param setsize Size of each individual set. Related to associativity.
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
 * @param associativity The rank of the cache associativity.
 * @param setsize The length of cache block set.
 * @param tag Tag used to check if the content in a slot is the desired content.
 * @param index The index of the memory.
 *
 * @return If the memory was in cache or not.
 */
bool is_hit_associative(Cache* cache, int associativity, int setsize, Tag tag,
                        Index index);

/**
 * Simulat a single memory instruction. Will update the cache as necessary,
 * and return the number of misses (either 0 or 1... it made sense at the time).
 *
 * @param cache Array of cache slots.
 * @param config Configuration of the cache.
 * @param tag Tag of the memory being operated on.
 * @param index Index of the memory being operated on.
 * @param read Whether or not this action is a read, or a write.
 *
 * return 0 for hit, 1 for miss.
 */
int sim(Cache* cache, CacheConf *config, Tag tag, Index index, bool read);

/**
 * Replacement is called whenever the replacement policy needs to be enforced.
 *
 * @param cache Array of cache slots to replace memory in.
 * @param tag Tag of the memory to be writting to cache.
 * @param index Index of the memory to be writting to cache.
  */
void replacement(Cache* cache, CacheConf *config, Tag tag, Index index);

/**
 * Random replacement implements a replacement policy where the memory is written
 * to a random line in the set.
 *
 * @param cache Array of cache slots to replace memory in.
 * @param associativity Associativity of the cache.
 * @param setsize The Size of each set in the cache.
 * @param tag Tag of the memory to be randomly written to cache.
 * @param index Index of the memory to be randomly written to cache.
 */
void random_replacement(Cache* cache, int associativity,
                        int setsize, Tag tag, Index index);

/**
 * FIFO replacement implements a replacement policy where the memory is written
 * to a consecutive offset in the block.
 *
 * @param cache Array of cache slots to replace memory in.
 * @param associativity Associativity of the cache.
 * @param setsize The Size of each set in the cache.
 * @param tag Tag of the memory to be randomly written to cache.
 * @param index Index of the memory to be randomly written to cache.
 */
void FIFO_replacement(Cache* cache, int associativity,
                      int setsize, Tag tag, Index index);
