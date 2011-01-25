#include "bitvector.h"
#include <gtest/gtest.h>

TEST(bitvector, CreationFailsForLargeBitWidths) {
  EXPECT_EXIT({bitvector<uint64_t>(65);}, ::testing::KilledBySignal(SIGABRT),
              "Assertion failed: .*");
  EXPECT_EXIT({bitvector<uint32_t>(33);}, ::testing::KilledBySignal(SIGABRT),
              "Assertion failed: .*");
  EXPECT_EXIT({bitvector<uint16_t>(17);}, ::testing::KilledBySignal(SIGABRT),
              "Assertion failed: .*");
  EXPECT_EXIT({bitvector<uint8_t>(9);}, ::testing::KilledBySignal(SIGABRT),
              "Assertion failed: .*");
}

TEST(bitvector, CreationSucceedsForValidBitWidths) {
  bitvector<uint8_t> *vector8 = new bitvector<uint8_t>(1); 
  vector8 = new bitvector<uint8_t>(8); 
  bitvector<uint16_t> *vector16 = new bitvector<uint16_t>(1);
  vector16 = new bitvector<uint16_t>(8);
  vector16 = new bitvector<uint16_t>(16);
  bitvector<uint32_t> *vector32 = new bitvector<uint32_t>(1);
  vector32 = new bitvector<uint32_t>(8);
  vector32 = new bitvector<uint32_t>(16);
  vector32 = new bitvector<uint32_t>(32);
  bitvector<uint64_t> *vector64 = new bitvector<uint64_t>(1);
  vector64 = new bitvector<uint64_t>(8);
  vector64 = new bitvector<uint64_t>(16);
  vector64 = new bitvector<uint64_t>(32);
  vector64 = new bitvector<uint64_t>(64);
}

TEST(bitvector, StoresValuesWithPushBack) {
  uint64_t values[24] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4};
  bitvector<uint64_t> vector(7);
  for(int i = 0; i<24; ++i) {
    vector.push_back(values[i]);
  }
  for(int i = 0; i<24; ++i) {
    EXPECT_EQ(vector[i], values[i]);
  }
}

TEST(bitvector, SizeIncreasesWithPushBack) {
  uint64_t values[24] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4};
  bitvector<uint64_t> vector(7);
  for(int i = 0; i<24; ++i) {
    EXPECT_EQ(vector.size(), i);
    vector.push_back(values[i]);
    EXPECT_EQ(vector.size(), i+1);    
  }
}

TEST(bitvector, OnlyLeastSignificantBitsAreStored) {
  uint64_t values[24] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4};
  bitvector<uint64_t> vector(2);
  for(int i = 0; i<24; ++i) {
    vector.push_back(values[i]);
  }
  for(int i = 0; i<24; ++i) {
    EXPECT_EQ(vector[i], values[i] & 3);
  }
}

TEST(bitvector, UpdatesValuesWithSet) {
  uint64_t old_values[24] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4};
  bitvector<uint64_t> vector(7);
  for(int i = 0; i<24; ++i) {
    vector.push_back(old_values[i]);
  }
  uint64_t new_values[24] = {4,3,2,1,0,9,8,7,6,5,4,3,2,1,0,9,8,7,6,5,4,3,2,1};
  for(int i = 0; i<24; ++i) {
    vector.set(i, new_values[i]);
  }
  for(int i = 0; i<24; ++i) {
    EXPECT_EQ(vector[i], new_values[i]);
  }
}