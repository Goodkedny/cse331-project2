#include <stdlib.h>

#include "gtest/gtest.h"

extern "C" {
  #include "cache.h"
}


namespace {

  TEST(CalculateTests, TagTest) {
    CacheConf conf;
    conf.tag_size = 8;
    EXPECT_EQ(0xFF, calculate_tag(&conf, 0xFFCCCCCC));
  }

  TEST(CalculateTests, IndexTest) {
    CacheConf conf;
    conf.tag_size = 8;
    conf.offset_bits = 16;
    EXPECT_EQ(0xDD, calculate_index(&conf, 0xFFDDCCCC));
  }
}
