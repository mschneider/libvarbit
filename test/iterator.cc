#include "test/varbit_test.h"

class Iterator : public TestWithVector { };

TEST_F(Iterator, CorrectNumberOfElements) {
  int i = 0;
  varbit::vector<uint64_t>::iterator it = vector.begin();
  while (it != vector.end()) {
    ++it;
    ++i;
  }
  EXPECT_EQ(kNumElements, i);
}

TEST_F(Iterator, SameValuesAsSubscript) {
  int i = 0;
  varbit::vector<uint64_t>::iterator it = vector.begin();
  while (it != vector.end()) {
    EXPECT_EQ(vector[i], *it);
    ++it;
    ++i;
  }
}

TEST_F(Iterator, SameValuesAsSubscriptUsingPostIncrement) {
  int i = 0;
  varbit::vector<uint64_t>::iterator it = vector.begin();
  while (!(it == vector.end())) {
    EXPECT_EQ(vector[i++], *it++);
  }
}

