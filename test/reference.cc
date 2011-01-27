#include "varbit/newvector.h"
#include <gtest/gtest.h>

TEST(reference, test) {
  varbit::vector<uint8_t> vector(2);
  vector.push_back(1);
  vector.push_back(2);
  vector.push_back(3);
  vector.push_back(4);
  vector.push_back(5);
  vector.push_back(6);
  vector.push_back(7);
  vector.push_back(8);
  vector[0]=8;
  vector[1]=7;
  vector[2]=6;
  vector[3]=5;
  vector[4]=4;
  vector[5]=3;
  vector[6]=2;
  vector[7]=1;
  EXPECT_EQ(vector[0], 0);
  EXPECT_EQ(vector[1], 3);
  EXPECT_EQ(vector[2], 2);
  EXPECT_EQ(vector[3], 1);
  EXPECT_EQ(vector[4], 0);
  EXPECT_EQ(vector[5], 3);
  EXPECT_EQ(vector[6], 2);
  EXPECT_EQ(vector[7], 1);
}