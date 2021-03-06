#include <gtest/gtest.h>
#include "varbit/vector.h"

TEST(reference, UsesImplicitConversion) {
  uint16_t block = 0x01FE;
  varbit::reference<uint16_t> reference1(&block, 0xF, 0);
  varbit::reference<uint16_t> reference2(&block, 0xF0, 4);
  varbit::reference<uint16_t> reference3(&block, 0xF00, 8);
  varbit::reference<uint16_t> reference4(&block, 0xF000, 12);
  EXPECT_EQ(static_cast<uint8_t>(0xE), reference1);
  EXPECT_EQ(static_cast<uint16_t>(0xE), reference1);
  EXPECT_EQ(static_cast<uint32_t>(0xE), reference1);
  EXPECT_EQ(static_cast<uint64_t>(0xE), reference1);
  EXPECT_EQ(0x000E, reference1);
  EXPECT_EQ(0x000F, reference2);
  EXPECT_EQ(0x0001, reference3);
  EXPECT_EQ(0x0000, reference4);
}

TEST(reference, IsWritable) {
  uint32_t block = 0x0001FFFE;
  varbit::reference<uint32_t> reference1(&block, 0xFF, 0);
  varbit::reference<uint32_t> reference2(&block, 0xFF00, 8);
  varbit::reference<uint32_t> reference3(&block, 0xFF0000, 16);
  varbit::reference<uint32_t> reference4(&block, 0xFF000000, 24);
  EXPECT_EQ(0x000000FE, reference1);
  EXPECT_EQ(0x000000FF, reference2);
  EXPECT_EQ(0x00000001, reference3);
  EXPECT_EQ(0x00000000, reference4);
  reference1 = 0x00;
  reference2 = 0x11;
  reference3 = 0xFE;
  reference4 = 0xFF;
  EXPECT_EQ(0x00000000, reference1);
  EXPECT_EQ(0x00000011, reference2);
  EXPECT_EQ(0x000000FE, reference3);
  EXPECT_EQ(0x000000FF, reference4);
}

TEST(reference, CorrectlyMasksHighBits) {
  uint64_t block = 0x00010203FFFEFDFCULL;
  varbit::reference<uint64_t> reference1(&block, 0xFFULL, 0);
  varbit::reference<uint64_t> reference2(&block, 0xFF00ULL, 8);
  varbit::reference<uint64_t> reference3(&block, 0x00FF000000000000ULL, 48);
  varbit::reference<uint64_t> reference4(&block, 0xFF00000000000000ULL, 56);
  EXPECT_EQ(0x00000000000000FCULL, reference1);
  EXPECT_EQ(0x00000000000000FDULL, reference2);
  EXPECT_EQ(0x0000000000000001ULL, reference3);
  EXPECT_EQ(0x0000000000000000ULL, reference4);
  reference1 = 0x0000000000000100ULL;
  reference2 = 0x0000000000000111ULL;
  reference3 = 0xFFFFFFFFFFFFFFFEULL;
  reference4 = 0xFFFFFFFFFFFFFFFFULL;
  EXPECT_EQ(0x0000000000000000ULL, reference1);
  EXPECT_EQ(0x0000000000000011ULL, reference2);
  EXPECT_EQ(0x00000000000000FEULL, reference3);
  EXPECT_EQ(0x00000000000000FFULL, reference4);
}
