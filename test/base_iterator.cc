#include "test/varbit_test.h"

class BaseIterator : public TestWithVector { };

TEST_F(BaseIterator, Comparable) {
  EXPECT_TRUE(vector.begin() == vector.begin());
  EXPECT_TRUE(vector.end() == vector.end());
  EXPECT_TRUE(vector.begin() != vector.end());

  EXPECT_FALSE(vector.begin() != vector.begin());
  EXPECT_FALSE(vector.end() != vector.end());
  EXPECT_FALSE(vector.begin() == vector.end());
}
