#include <vector>
#include <chrono>
#include "gtest/gtest.h"
#include "CircularBuffer.h"
#include "TestUtils.h"

using namespace collections;

TEST(ChangeMethodsTest, PushBack) {
  auto buffer = CircularBuffer<int>(2);
  buffer.push_back(1);
  buffer.push_back(2);
  std::vector<int> expected_1{1, 2};
  EXPECT_TRUE(isEqual(buffer, expected_1));

  buffer.push_back(2);
  std::vector<int> expected_2{2, 2};
  EXPECT_TRUE(isEqual(buffer, expected_2));
}

TEST(ChangeMethodsTest, PushFront) {
  auto buffer = CircularBuffer<int>(2);
  buffer.push_front(1);
  buffer.push_front(2);
  std::vector<int> expected{2, 1};
  EXPECT_TRUE(isEqual(buffer, expected));
}

TEST(ChangeMethodsTest, Resize) {
  auto buffer = CircularBuffer<int>{2, 1};
  buffer.resize(5, 3);
  std::vector<int> expected{1, 1, 3, 3, 3};
  EXPECT_TRUE(isEqual(buffer, expected));
}

TEST(ChangeMethodsTest, Swap) {
  auto fst_buffer = CircularBuffer<int>(3, 2);
  auto snd_buffer = CircularBuffer<int>(2, 3);
  fst_buffer.swap(snd_buffer);
  std::vector<int> expected_fst{3, 3};
  std::vector<int> expected_snd{2, 2, 2};
  EXPECT_TRUE(isEqual(fst_buffer, expected_fst));
  EXPECT_TRUE(isEqual(snd_buffer, expected_snd));
  snd_buffer.swap(fst_buffer);
  EXPECT_TRUE(isEqual(fst_buffer, expected_snd));
  EXPECT_TRUE(isEqual(snd_buffer, expected_fst));
}

//TEST(ChangeMethodsTest, Copy) {
//  CircularBuffer<int> a;
//  CircularBuffer<int> b;
//  auto c = b = a;
//  }

TEST(ChangeMethodsTest, Move) {
  auto buffer = CircularBuffer<int>{100000000, 1};
  auto start_copy = std::chrono::high_resolution_clock::now();
  auto buffer_copy = CircularBuffer(buffer);
  auto end_copy = std::chrono::high_resolution_clock::now();
  double time_taken_copy;
  time_taken_copy = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(end_copy - start_copy).count();

  time_taken_copy *= 1e-9;

  std::cout << "Time taken by copy is : " << std::fixed
            << time_taken_copy << std::setprecision(9);
  std::cout << " sec" << std::endl;

  auto start_move = std::chrono::high_resolution_clock::now();
  auto buffer_move = CircularBuffer(std::move(buffer));
  auto end_move = std::chrono::high_resolution_clock::now();
  double time_taken_move;
  time_taken_move = (double) std::chrono::duration_cast<std::chrono::nanoseconds>(end_move - start_move).count();

  time_taken_move *= 1e-9;

  std::cout << "Time taken by move is : " << std::fixed
            << time_taken_move << std::setprecision(9);
  std::cout << " sec" << std::endl;

  EXPECT_GT(time_taken_copy, time_taken_move);
  EXPECT_EQ(buffer_copy.size(), buffer_move.size());
}
