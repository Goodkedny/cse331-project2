#include <stdlib.h>
#include <stdbool.h>

#include "cache.h"

bool is_hit (Cache* cache, bool direct_mapped, int slots, Tag tag, Index index)
{
  if(direct_mapped) {
    return is_hit_direct(cache, tag, index);
  } else {
    return is_hit_associative(cache, slots, tag);
  }
}

bool is_hit_direct(Cache* cache, Tag tag, Index index) {
  /* Only directly looks at the index block. */
  return (cache[index].tag == tag) && cache[index].valid;
}

bool is_hit_associative(Cache* cache, int slots, Tag tag) {
  /* Searches through all cache blocks. */
  for(int i = 0; i < slots; i++)
  {
    if(cache[i].tag == tag && cache[i].valid){
      return true;
    }
  }
  return false;
}

int sim_load(Cache* cache, CacheConf *config, Tag tag, Index index) {
  int misses = 0;

  if(!is_hit(cache, config->associativity, config->num_sets, tag, index)) {
    on_load_miss(cache, config, tag, index);
    misses = 1;
  }
  return misses;
}

void on_load_miss(Cache* cache, CacheConf *config, Tag tag, Index index) {
  if(config->associativity) {
    /* Direct mapped just writes. */
    cache[index].tag = tag;
    cache[index].valid = true;
    return;
  }

  if(config->replacement_policy) {
    load_FIFO_replacement(cache, config->num_sets, tag);
  } else {
    load_random_replacement(cache, config->num_sets, tag);
  }
}

void load_random_replacement(Cache* cache, int slots, Tag tag) {
  unsigned int index = rand() % slots;

  cache[index].tag = tag;
  cache[index].valid = true;
}

void load_FIFO_replacement(Cache* cache, int slots, Tag tag) {
  static unsigned int count = 0;

  cache[count].tag = tag;
  cache[count].valid = true;

  count = (count + 1) % slots;
}
