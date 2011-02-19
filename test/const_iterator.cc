#include "test/varbit_test.h"

class ConstIterator : public varbit::TestWithVector { };

TEST_F(ConstIterator, returnsTheSameValuesAsSubscript) {
  int i = 0;
  varbit::vector<uint64_t>::const_iterator it = vector.begin();
  while (it != vector.end()) {
    EXPECT_EQ(vector[i], *it);
    ++it;
    ++i;
  }
  EXPECT_EQ(24, i);
}

TEST_F(ConstIterator, returnsTheSameValuesAsConstSubscript) {
  const varbit::vector<uint64_t>& const_vector = vector;
  int i = 0;
  varbit::vector<uint64_t>::const_iterator it = const_vector.begin();
  while (it != const_vector.end()) {
    EXPECT_EQ(const_vector[i], *it);
    ++it;
    ++i;
  }
  EXPECT_EQ(24, i);
}

TEST_F(ConstIterator, returnsTheSameValuesAsSubscriptUsingPostIncrement) {
  int i = 0;
  varbit::vector<uint64_t>::const_iterator it = vector.begin();
  while (!(it == vector.end())) {
    EXPECT_EQ(vector[i++], *it++);
  }
  EXPECT_EQ(24, i);
}

TEST_F(ConstIterator, returnsTheSameValuesAsConstSubscriptUsingPostIncrement) {
  const varbit::vector<uint64_t>& const_vector = vector;
  int i = 0;
  varbit::vector<uint64_t>::const_iterator it = const_vector.begin();
  while (!(it == const_vector.end())) {
    EXPECT_EQ(const_vector[i++], *it++);
  }
  EXPECT_EQ(24, i);
}
