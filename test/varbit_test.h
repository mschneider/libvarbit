#ifndef TEST_VARBIT_TEST_H_
#define TEST_VARBIT_TEST_H_
#include <gtest/gtest.h>
#include "varbit/vector.h"

const int kSignificantBits = 7;
const int kNumElements = 24;
const int64_t kElements[kNumElements] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
    12, 128, 129, 127, 126, 98, 45, -1, -2, -233, -98, -52};

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
