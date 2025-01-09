#include <gtest/gtest.h>
#include <sys/types.h>

#include <cstddef>
#include <thread>

#include "gtest/gtest.h"
#include "queues/lock_free_queue.hpp"
#include "utils/data.hpp"

template <typename T>
class LockFreeQueueTest : public ::testing::Test {};

using MyTypes =
    ::testing::Types<u_int8_t, u_int16_t, u_int32_t, u_int64_t, int8_t, int16_t,
                     int32_t, int64_t, float, double, std::string, Data>;
// using MyTypes = ::testing::Types<int>;
TYPED_TEST_SUITE(LockFreeQueueTest, MyTypes, );

TYPED_TEST(LockFreeQueueTest, BasicTest1) {
  my::LockFreeQueue<int, 16> q;
  q.push(1);
  q.push(2);
  q.push(3);
  int value;
  q.pop(value);
  EXPECT_EQ(value, 1);
  q.pop(value);
  EXPECT_EQ(value, 2);
  q.pop(value);
  EXPECT_EQ(value, 3);
  EXPECT_TRUE(q.empty());
}

TEST(LockFreeQueueTest, BasicTest2) {
  my::LockFreeQueue<Data, 16> q;
  q.push(Data(10));
  q.push(Data(20));
  q.push(Data(30));
  Data value;
  q.pop(value);
  EXPECT_EQ(value, Data(10));
  q.pop(value);
  EXPECT_EQ(value, Data(20));
  q.pop(value);
  EXPECT_EQ(value, Data(30));
  EXPECT_TRUE(q.empty());
}

TYPED_TEST(LockFreeQueueTest, BasicTest3) {
  const size_t queue_capacity = 1024;
  my::LockFreeQueue<TypeParam, queue_capacity> q;
  for (size_t i = 0; i < queue_capacity; i++) {
    q.push(TypeParam());
  }
  EXPECT_EQ(q.size(), queue_capacity);
  TypeParam value;
  for (size_t i = 0; i < queue_capacity; i++) {
    q.pop(value);
  }
  EXPECT_TRUE(q.empty());
}

TYPED_TEST(LockFreeQueueTest, MultiThreadTest1) {
  // multi-thread push and pop
  const size_t num_push_threads = 1;
  const size_t num_pushes_per_thread = (1 << 10);
  const size_t num_pop_threads = 1;
  const size_t num_pops_per_thread = (1 << 10);
  const size_t queue_capacity = num_push_threads * num_pushes_per_thread;
  std::vector<std::thread> push_threads;
  std::vector<std::thread> pop_threads;
  my::LockFreeQueue<TypeParam, queue_capacity> queue;

  size_t missed_pushes = 0;
  size_t missed_pops = 0;

  for (size_t tid = 0; tid < num_push_threads; tid++) {
    push_threads.emplace_back([&] {
      for (size_t i = 0; i < num_pushes_per_thread; i++) {
        if (!queue.push(TypeParam())) {
          missed_pushes++;
        }
      }
    });
  }

  for (size_t tid = 0; tid < num_pop_threads; tid++) {
    pop_threads.emplace_back([&] {
      TypeParam value;
      for (size_t i = 0; i < num_pops_per_thread; i++) {
        if (!queue.pop(value)) {
          missed_pops++;
        }
      }
    });
  }

  for (auto& thread : push_threads) {
    thread.join();
  }

  for (auto& thread : pop_threads) {
    thread.join();
  }

  EXPECT_EQ(queue.size(), missed_pops - missed_pushes);
  // EXPECT_TRUE(queue.empty());
}