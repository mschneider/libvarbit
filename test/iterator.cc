#include "test/varbit_test.h"

class Iterator : public varbit::TestWithVector { };

TEST_F(Iterator, returnsTheSameValuesAsSubscript) {
  int i = 0;
  varbit::vector<uint64_t>::iterator it = vector.begin();
  while (it != vector.end()) {
    EXPECT_EQ(vector[i], *it);
    ++it;
    ++i;
  }
  EXPECT_EQ(24, i);
}

TEST_F(Iterator, returnsTheSameValuesAsSubscriptUsingPostIncrement) {
  int i = 0;
  varbit::vector<uint64_t>::iterator it = vector.begin();
  while (!(it == vector.end())) {
    EXPECT_EQ(vector[i++], *it++);
  }
  EXPECT_EQ(24, i);
}
