/*******************************************************************************
/
/  filename: cache.c
/  description: Implementation file for cache.h. Implements the logic for the
/               cache.
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
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "cache.h"

bool is_hit (Cache* cache, int associativity, int setsize, Tag tag,
             Index index) {
  if(!associativity) {
    return is_hit_direct(cache, tag, index);
  } else {
    return is_hit_associative(cache, associativity, setsize, tag, index);
  }
}

bool is_hit_direct(Cache* cache, Tag tag, Index index) {
  /* Only directly looks at the index block. */
  return (cache[index].tag == tag) && cache[index].valid;
}

bool is_hit_associative(Cache* cache, int associativity, int setsize, Tag tag,
                        Index index) {
  /* Searches through all cache blocks. */
  if (associativity == 1) {
    index = 0;
  }

  for(int i = 0; i < setsize; i++)
  {
    if(cache[index * associativity + i].tag == tag &&
       cache[index * associativity + i].valid) {
      return true;
    }
  }
  return false;
}

int sim(Cache* cache, CacheConf *config, Tag tag, Index index, bool read) {
  int misses = 0;

  if(!is_hit(cache, config->associativity, config->set_size, tag, index)) {
    /* We have a miss. Need to handle replacement. */
    if(read || config->write_allocate) {
      replacement(cache, config, tag, index);
    }
    misses = 1;
  }

  return misses;
}

void replacement(Cache* cache, CacheConf *config, Tag tag, Index index) {
  if(!config->associativity) {
    /* Direct mapped just writes. */
    cache[index].tag = tag;
    cache[index].valid = true;
    return;
  }

  if(!config->replacement_policy) {
    FIFO_replacement(cache, config->associativity, config->set_size, tag,
                     index);
  } else {
    random_replacement(cache, config->associativity, config->set_size, tag,
                       index);
  }
}

void random_replacement(Cache* cache, int associativity, int setsize, Tag tag,
                        Index index) {
  unsigned int offset = rand() % setsize;

  if (associativity == 1) {
    index = 0;
  }

  cache[index * associativity + offset].tag = tag;
  cache[index * associativity + offset].valid = true;
}

void FIFO_replacement(Cache* cache, int associativity, int setsize, Tag tag,
                      Index index) {
  static unsigned int count = 0;

  if (associativity == 1) {
    index = 0;
  }
 

  cache[index * associativity + count].tag = tag;
  cache[index * associativity + count].valid = true;

  count = (count + 1) % setsize;
}
