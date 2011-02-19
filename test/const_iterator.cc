#include "test/varbit_test.h"

class ConstIterator : public TestWithVector { };

TEST_F(ConstIterator, CorrectNumberOfElements) {
  int i = 0;
  varbit::vector<uint64_t>::const_iterator it = vector.begin();
  while (it != vector.end()) {
    ++it;
    ++i;
  }
  EXPECT_EQ(kNumElements, i);
}

TEST_F(ConstIterator, SameValuesAsSubscript) {
  int i = 0;
  varbit::vector<uint64_t>::const_iterator it = vector.begin();
  while (it != vector.end()) {
    EXPECT_EQ(vector[i], *it);
    ++it;
    ++i;
  }
}

TEST_F(ConstIterator, SameValuesAsConstSubscript) {
  const varbit::vector<uint64_t>& const_vector = vector;
  int i = 0;
  varbit::vector<uint64_t>::const_iterator it = const_vector.begin();
  while (it != const_vector.end()) {
    EXPECT_EQ(const_vector[i], *it);
    ++it;
    ++i;
  }
}

TEST_F(ConstIterator, SameValuesAsSubscriptUsingPostIncrement) {
  int i = 0;
  varbit::vector<uint64_t>::const_iterator it = vector.begin();
  while (!(it == vector.end())) {
    EXPECT_EQ(vector[i++], *it++);
  }
}

TEST_F(ConstIterator, SameValuesAsConstSubscriptUsingPostIncrement) {
  const varbit::vector<uint64_t>& const_vector = vector;
  int i = 0;
  varbit::vector<uint64_t>::const_iterator it = const_vector.begin();
  while (!(it == const_vector.end())) {
    EXPECT_EQ(const_vector[i++], *it++);
  }
}
