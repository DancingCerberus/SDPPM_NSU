#include "gtest/gtest.h"
#include "CircularBuffer.h"
using namespace collections;


TEST(CreationTest, DefaultConstructor) {
  auto buffer = CircularBuffer<int>();
  EXPECT_EQ(buffer.empty(), true);
  EXPECT_EQ(buffer.size(), 0);
  EXPECT_EQ(buffer.capacity(), 0);
  EXPECT_EQ(buffer.full(), true);
  EXPECT_EQ(buffer.is_linearized(), true);
  EXPECT_EQ(buffer.reserve(), 0);
  EXPECT_ANY_THROW(buffer.at(0));
  EXPECT_ANY_THROW(buffer.pop_back());
  EXPECT_ANY_THROW(buffer.pop_front());
}

TEST(CreationTest, CapacityConstructor) {
  auto buffer = CircularBuffer<int>(5);
  EXPECT_EQ(buffer.empty(), true);
  EXPECT_EQ(buffer.size(), 0);
  EXPECT_EQ(buffer.capacity(), 5);
  EXPECT_EQ(buffer.full(), false);
  EXPECT_EQ(buffer.is_linearized(), true);
  EXPECT_EQ(buffer.reserve(), 5);
  EXPECT_ANY_THROW(buffer.at(0));
  EXPECT_ANY_THROW(buffer.pop_back());
  EXPECT_ANY_THROW(buffer.pop_front());
}

TEST(CreationTest, InitializingConstructor) {
  auto buffer = CircularBuffer<int>(5, 2);
  EXPECT_EQ(buffer.empty(), false);
  EXPECT_EQ(buffer.size(), 5);
  EXPECT_EQ(buffer.capacity(), 5);
  EXPECT_EQ(buffer.full(), true);
  EXPECT_EQ(buffer.is_linearized(), true);
  EXPECT_EQ(buffer.reserve(), 0);
  EXPECT_EQ(buffer.at(0), 2);
  EXPECT_NO_THROW(buffer.pop_back());
  EXPECT_NO_THROW(buffer.pop_front());
}

TEST(CreationTest, CopyConstructor) {
  auto buffer = CircularBuffer<int>(5, 2);
  auto buffer_copy = buffer;
  EXPECT_EQ(buffer_copy.empty(), false);
  EXPECT_EQ(buffer_copy.size(), 5);
  EXPECT_EQ(buffer_copy.capacity(), 5);
  EXPECT_EQ(buffer_copy.full(), true);
  EXPECT_EQ(buffer_copy.is_linearized(), true);
  EXPECT_EQ(buffer_copy.reserve(), 0);
  EXPECT_EQ(buffer_copy.at(0), 2);
  EXPECT_NO_THROW(buffer_copy.pop_back());
  EXPECT_NO_THROW(buffer_copy.pop_front());
}