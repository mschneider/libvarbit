#include <gtest/gtest.h>
#include "varbit/vector.h"

TEST(const_iterator, returnsTheSameValuesAsSubscript) {
  uint64_t values[24] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 128, 129,
                         127, 126, 98, 45, -1, -2, -233, -98, -52};
  varbit::vector<uint64_t> vector(7);
  for (int i = 0; i < 24; ++i) {
    vector.push_back(values[i]);
  }
  int i = 0;
  for (varbit::vector<uint64_t>::const_iterator it = vector.begin();
       it != vector.end(); ++it, ++i) {
    EXPECT_EQ(vector[i], *it);
  }
  EXPECT_EQ(24, i);
}

TEST(const_iterator, returnsTheSameValuesAsConstSubscript) {
  uint64_t values[24] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 128, 129,
                         127, 126, 98, 45, -1, -2, -233, -98, -52};
  varbit::vector<uint64_t> vector(7);
  for (int i = 0; i < 24; ++i) {
    vector.push_back(values[i]);
  }
  const varbit::vector<uint64_t>& const_vector = vector;
  int i = 0;
  for (varbit::vector<uint64_t>::const_iterator it = const_vector.begin();
       it != const_vector.end(); ++it, ++i) {
    EXPECT_EQ(vector[i], *it);
    EXPECT_EQ(const_vector[i], *it);
  }
  EXPECT_EQ(24, i);
}

TEST(const_iterator, returnsTheSameValuesAsSubscriptWithPostIncrement) {
  uint64_t values[24] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 128, 129,
                         127, 126, 98, 45, -1, -2, -233, -98, -52};
  varbit::vector<uint64_t> vector(7);
  for (int i = 0; i < 24; ++i) {
    vector.push_back(values[i]);
  }
  int i = 0;
  for (varbit::vector<uint64_t>::const_iterator it = vector.begin();
       !(it == vector.end()); ++i) {
    EXPECT_EQ(vector[i], *it++);
  }
  EXPECT_EQ(24, i);
}

TEST(const_iterator, returnsTheSameValuesAsConstSubscriptWithPostIncrement) {
  uint64_t values[24] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 128, 129,
                         127, 126, 98, 45, -1, -2, -233, -98, -52};
  varbit::vector<uint64_t> vector(7);
  for (int i = 0; i < 24; ++i) {
    vector.push_back(values[i]);
  }
  const varbit::vector<uint64_t>& const_vector = vector;
  int i = 0;
  for (varbit::vector<uint64_t>::const_iterator it = const_vector.begin();
       !(it == const_vector.end()); ++i) {
    EXPECT_EQ(vector[i], *it);
    EXPECT_EQ(const_vector[i], *it++);
  }
  EXPECT_EQ(24, i);
}
