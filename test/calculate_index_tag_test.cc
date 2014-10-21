#include <stdlib.h>

#include "gtest/gtest.h"

extern "C" {
  #include "cache.h"
}


namespace {
  
  TEST(TagTest) {
    CacheConf conf;
    conf.tag_size = 8;
    ASSERT_EQ(0xFF, calculate_tag(&conf, 0xFFCCCCCC));
  }
  
  TEST(IndexTest) {
    CacheConf conf;
    conf.tag_size = 8;
    conf.offset_bits = 16;
    ASSERT_EQ(0xDD, 0xFFDDCCCC);
  }
}
