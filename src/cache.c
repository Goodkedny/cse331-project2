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
