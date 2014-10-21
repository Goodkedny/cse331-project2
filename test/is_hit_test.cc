#include <stdlib.h>

#include "gtest/gtest.h"

extern "C" {
  #include "cache.h"
}

namespace {

  class IsHitTests : public ::testing::Test {
   public:
    int cache_blocks = 10;;
    Tag tag = 100;
    Index index = 5;
    Cache *cache;

    virtual void SetUp() {
      cache = (Cache*)malloc(sizeof(Cache) * cache_blocks);

      for(int i = 0; i < cache_blocks; i++) {
        cache[i].tag = 0;
        cache[i].valid = false;
      }

    }

    virtual void TearDown() {
      free(cache);
    }
  }; // class

  TEST_F(IsHitTests, DirectExistsIsValid) {
    cache[index].valid = true;
    cache[index].tag = tag;

    EXPECT_TRUE(is_hit(cache, true, cache_blocks, tag, index));
  }

  TEST_F(IsHitTests, DirectExistsIsValidWrongIndex) {
    cache[index + 1].valid = true;
    cache[index + 1].tag = tag;

    EXPECT_FALSE(is_hit(cache, true, cache_blocks, tag, index));
  }

  TEST_F(IsHitTests, DirectExistsIsNotValid) {
    cache[index].valid = false;
    cache[index].tag = tag;

    EXPECT_FALSE(is_hit(cache, true, cache_blocks, tag, index));
  }

  TEST_F(IsHitTests, DirectDoesNotExist) {
    EXPECT_FALSE(is_hit(cache, true, cache_blocks, tag, index));
  }

  TEST_F(IsHitTests, AssociativeExistsIsValid) {
    cache[index + 2].valid = true;
    cache[index + 2].tag = tag;

    EXPECT_TRUE(is_hit(cache, false, cache_blocks, tag, index));
  }

  TEST_F(IsHitTests, AssociativeExistsIsNotValid) {
    cache[index + 2].valid = false;
    cache[index + 2].tag = tag;

    EXPECT_FALSE(is_hit(cache, false, cache_blocks, tag, index));
  }

  TEST_F(IsHitTests, AssociativeDoesNotExist) {
    EXPECT_FALSE(is_hit(cache, false, cache_blocks, tag, index));
  }

} // namespace