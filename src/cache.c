#include <stdlib.h>
#include <stdbool.h>

#include "cache.h"

bool is_hit (Cache* cache, bool direct_mapped, int setsize, Tag tag,
             Index index) {
  if(direct_mapped) {
    return is_hit_direct(cache, tag, index);
  } else {
    return is_hit_associative(cache, setsize, tag, index);
  }
}

bool is_hit_direct(Cache* cache, Tag tag, Index index) {
  /* Only directly looks at the index block. */
  return (cache[index].tag == tag) && cache[index].valid;
}

bool is_hit_associative(Cache* cache, int setsize, Tag tag, Index index) {
  /* Searches through all cache blocks. */
  for(int i = 0; i < setsize; i++)
  {
    if(cache[index + i].tag == tag && cache[index + i].valid){
      return true;
    }
  }
  return false;
}

int sim(Cache* cache, CacheConf *config, Tag tag, Index index, bool read) {
  int misses = 0;

  if(!is_hit(cache, config->associativity, config->num_sets, tag, index)) {
    /* We have a miss. Need to handle replacement. */
    if(!read || config->write_allocate) {
      replacement(cache, config, tag, index);
    }
    misses = 1;
  }

  return misses;
}

void replacement(Cache* cache, CacheConf *config, Tag tag, Index index) {
  if(config->associativity) {
    /* Direct mapped just writes. */
    cache[index].tag = tag;
    cache[index].valid = true;
    return;
  }

  if(config->replacement_policy) {
    FIFO_replacement(cache, config->num_sets, tag, index);
  } else {
    random_replacement(cache, config->num_sets, tag, index);
  }
}

void random_replacement(Cache* cache, int setsize, Tag tag, Index index) {
  unsigned int offset = rand() % setsize;

  cache[index + offset].tag = tag;
  cache[index + offset].valid = true;
}

void FIFO_replacement(Cache* cache, int setsize, Tag tag, Index index) {
  static unsigned int count = 0;

  cache[index + count].tag = tag;
  cache[index + count].valid = true;

  count = (count + 1) % setsize;
}
