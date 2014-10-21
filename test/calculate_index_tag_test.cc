#include <stdlib.h>

#include "gtest/gtest.h"

extern "C" {
  #include "cache.h"
}


namespace {
  
  TEST(TagTest) {
    CacheConf conf;
    conf.tag_size = 8;
    ASSERT_EQ(0xFF, calculate_tag(&conf, 0xFF000000));
  }
}
