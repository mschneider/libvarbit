#include "vector.h"
#include <gtest/gtest.h>

TEST(vector, CreationFailsForLargeBitWidths) {
  EXPECT_EXIT({varbit::vector<uint64_t>(65);}, ::testing::KilledBySignal(SIGABRT),
              "Assertion failed: .*");
  EXPECT_EXIT({varbit::vector<uint32_t>(33);}, ::testing::KilledBySignal(SIGABRT),
              "Assertion failed: .*");
  EXPECT_EXIT({varbit::vector<uint16_t>(17);}, ::testing::KilledBySignal(SIGABRT),
              "Assertion failed: .*");
  EXPECT_EXIT({varbit::vector<uint8_t>(9);}, ::testing::KilledBySignal(SIGABRT),
              "Assertion failed: .*");
}

TEST(vector, CreationSucceedsForValidBitWidths) {
  varbit::vector<uint8_t> *vector8 = new varbit::vector<uint8_t>(1);
  EXPECT_EQ(vector8->size(), 0);
  vector8 = new varbit::vector<uint8_t>(8); 
  EXPECT_EQ(vector8->size(), 0);
  varbit::vector<uint16_t> *vector16 = new varbit::vector<uint16_t>(1);
  EXPECT_EQ(vector16->size(), 0);
  vector16 = new varbit::vector<uint16_t>(8);
  EXPECT_EQ(vector16->size(), 0);
  vector16 = new varbit::vector<uint16_t>(16);
  EXPECT_EQ(vector16->size(), 0);
  varbit::vector<uint32_t> *vector32 = new varbit::vector<uint32_t>(1);
  EXPECT_EQ(vector32->size(), 0);
  vector32 = new varbit::vector<uint32_t>(8);
  EXPECT_EQ(vector32->size(), 0);
  vector32 = new varbit::vector<uint32_t>(16);
  EXPECT_EQ(vector32->size(), 0);
  vector32 = new varbit::vector<uint32_t>(32);
  EXPECT_EQ(vector32->size(), 0);
  varbit::vector<uint64_t> *vector64 = new varbit::vector<uint64_t>(1);
  EXPECT_EQ(vector64->size(), 0);
  vector64 = new varbit::vector<uint64_t>(8);
  EXPECT_EQ(vector64->size(), 0);
  vector64 = new varbit::vector<uint64_t>(16);
  EXPECT_EQ(vector64->size(), 0);
  vector64 = new varbit::vector<uint64_t>(32);
  EXPECT_EQ(vector64->size(), 0);
  vector64 = new varbit::vector<uint64_t>(64);
  EXPECT_EQ(vector64->size(), 0);
}

TEST(vector, StoresValuesWithPushBack) {
  uint64_t values[24] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4};
  varbit::vector<uint64_t> vector(7);
  for(int i = 0; i<24; ++i) {
    vector.push_back(values[i]);
  }
  for(int i = 0; i<24; ++i) {
    EXPECT_EQ(vector[i], values[i]);
  }
}

TEST(vector, SizeIncreasesWithPushBack) {
  uint64_t values[24] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4};
  varbit::vector<uint64_t> vector(7);
  for(int i = 0; i<24; ++i) {
    EXPECT_EQ(vector.size(), i);
    vector.push_back(values[i]);
    EXPECT_EQ(vector.size(), i+1);    
  }
}

TEST(vector, CapacityIsAlwaysGreaterOrEqualThanSize) {
  uint64_t values[24] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4};
  varbit::vector<uint64_t> vector(7);
  for(int i = 0; i<24; ++i) {
    EXPECT_GE(vector.capacity(), vector.size());
    vector.push_back(values[i]);
    EXPECT_GE(vector.capacity(), vector.size());    
  }
}

TEST(vector, OnlyLeastSignificantBitsAreStored) {
  uint64_t values[24] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4};
  varbit::vector<uint64_t> vector(2);
  for(int i = 0; i<24; ++i) {
    vector.push_back(values[i]);
  }
  for(int i = 0; i<24; ++i) {
    EXPECT_EQ(vector[i], values[i] & 3);
  }
}

TEST(vector, UpdatesValuesWithSet) {
  uint64_t old_values[24] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4};
  varbit::vector<uint64_t> vector(7);
  for(int i = 0; i<24; ++i) {
    vector.push_back(old_values[i]);
  }
  uint64_t new_values[24] = {4,3,2,1,0,9,8,7,6,5,4,3,2,1,0,9,8,7,6,5,4,3,2,1};
  for(int i = 0; i<24; ++i) {
    vector.Set(i, new_values[i]);
  }
  for(int i = 0; i<24; ++i) {
    EXPECT_EQ(vector[i], new_values[i]);
  }
}