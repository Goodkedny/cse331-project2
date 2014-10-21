#include "gtest/gtest.h"

extern "C" {
  #include "cache.h"
}

namespace {

  class SimTest : public ::testing::Test {
  public:
    Cache *cache;
    CacheConf config;
    Tag tag;
    Index index;

    virtual void SetUp() {
      config = { 0 };
      cache = (Cache*)malloc(sizeof(Cache) * 16);
      for(int i = 0; i < 16; i++) {
        cache[i].valid = false;
        cache[i].tag = 0;
      }
      tag = 100;
      index = 2;
    }

    virtual void TearDown() {
      free(cache);
      config = { 0 };
    }
  };

  TEST_F(SimTest, LoadDirectHit) {
    cache[index].tag = tag;
    cache[index].valid = true;
    config.associativity = 0;

    EXPECT_EQ(sim(cache, &config, tag, index, true), 0);
  }

  TEST_F(SimTest, LoadDirectMiss) {
    config.associativity = 0;
    config.set_size = 1;

    EXPECT_EQ(sim(cache, &config, tag, index, true), 1);
    EXPECT_TRUE(cache[index].valid);
    EXPECT_EQ(cache[index].tag, tag);
  }

  TEST_F(SimTest, LoadFreeHit) {
    config.associativity = 1;
    config.set_size = 16;

    cache[index + 3].tag = tag;
    cache[index + 3].valid = true;

    EXPECT_EQ(sim(cache, &config, tag, index, true), 0);
  }

  TEST_F(SimTest, LoadFreeMissFIFO) {
    config.associativity = 1;
    config.set_size = 16;

    EXPECT_EQ(sim(cache, &config, tag, index, true), 1);
    EXPECT_TRUE(cache[2].valid);
    EXPECT_EQ(cache[2].tag, tag);
  }

  TEST_F(SimTest, LoadFree2MissFIFO) {
    config.associativity = 1;
    config.set_size = 16;

    // We had to increment due to the static
    EXPECT_EQ(sim(cache, &config, tag, index, true), 1);
    EXPECT_EQ(sim(cache, &config, tag + 1, index, true), 1);
    EXPECT_TRUE(cache[3].valid);
    EXPECT_EQ(cache[3].tag, tag);
    EXPECT_TRUE(cache[4].valid);
    EXPECT_EQ(cache[4].tag, tag + 1);
  }

  TEST_F(SimTest, Load2AssocMissRandom) {
    config.associativity = 2;
    config.set_size = 2;
    config.replacement_policy = 1;

    EXPECT_EQ(sim(cache, &config, tag, index, true), 1);
    EXPECT_TRUE(cache[4].valid || cache[5].valid);
    EXPECT_TRUE((cache[4].tag == tag) || (cache[5].tag == tag));
  }

  TEST_F(SimTest, WriteDirectWrite) {
    config.associativity = 0;
    config.set_size = 1;
    config.write_allocate = true;

    EXPECT_EQ(sim(cache, &config, tag, index, false), 1);
    EXPECT_TRUE(cache[index].valid);
    EXPECT_EQ(cache[index].tag, tag);
  }

  TEST_F(SimTest, WriteDirectNoWrite) {
    config.associativity = 0;
    config.set_size = 1;
    config.write_allocate = false;

    EXPECT_EQ(sim(cache, &config, tag, index, false), 1);
    EXPECT_FALSE(cache[index].valid);
    EXPECT_NE(cache[index].tag, tag);
  }

} //Namespace
