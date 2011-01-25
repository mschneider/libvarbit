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
