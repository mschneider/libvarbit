#ifndef TEST_VARBIT_TEST_H_
#define TEST_VARBIT_TEST_H_
#include <gtest/gtest.h>
#include "varbit/vector.h"

const int kSignificantBits = 7;
const int kNumElements = 24;
const uint64_t kElements[kNumElements] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    32, 33, 34, 60, 61, 62, 122, 123, 124, 125, 126, 127};

class TestWithVector : public testing::Test {
 public:
  TestWithVector() : testing::Test(), vector(kSignificantBits) { }
  virtual void SetUp() {
    vector = varbit::vector<uint64_t>(kSignificantBits);
    for (int i = 0; i < kNumElements; ++i) {
      vector.push_back(kElements[i]);
    }
  }
  varbit::vector<uint64_t> vector; //NOLINT: we don't use std::vector
};
#endif  // TEST_VARBIT_TEST_H_
