#include <gtest/gtest.h>
#include <sys/types.h>

#include <thread>

#include "data.hpp"
#include "gtest/gtest.h"
#include "queue.hpp"
#include "thread_safe_queue.hpp"

template <typename T>
class ThreadSafeQueueTest : public ::testing::Test {};

using MyTypes =
    ::testing::Types<u_int8_t, u_int16_t, u_int32_t, u_int64_t, int8_t, int16_t,
                     int32_t, int64_t, float, double, std::string, Data>;
// using MyTypes = ::testing::Types<int>;
TYPED_TEST_SUITE(ThreadSafeQueueTest, MyTypes, );

TYPED_TEST(ThreadSafeQueueTest, BasicTest1) {
  my::ThreadSafeQueue<int, my::queue<int>> q(16);
  q.enqueue(1);
  q.enqueue(2);
  q.enqueue(3);
  EXPECT_EQ(q.dequeue(), 1);
  EXPECT_EQ(q.dequeue(), 2);
  EXPECT_EQ(q.dequeue(), 3);
  EXPECT_TRUE(q.empty());
}

TEST(ThreadSafeQueueTest, BasicTest2) {
  my::ThreadSafeQueue<Data, my::queue<Data>> q(16);
  q.enqueue(Data(10));
  q.enqueue(Data(20));
  q.enqueue(Data(30));
  EXPECT_EQ(q.dequeue(), Data(10));
  EXPECT_EQ(q.dequeue(), Data(20));
  EXPECT_EQ(q.dequeue(), Data(30));
  EXPECT_TRUE(q.empty());
}

TYPED_TEST(ThreadSafeQueueTest, BasicTest3) {
  const int queue_capacity = 1024;
  my::ThreadSafeQueue<TypeParam, my::queue<TypeParam>> q(queue_capacity);
  for (int i = 0; i < queue_capacity; i++) {
    q.enqueue(TypeParam());
  }
  EXPECT_EQ(q.size(), queue_capacity);
  for (int i = 0; i < queue_capacity; i++) {
    q.dequeue();
  }
  EXPECT_TRUE(q.empty());
}

TYPED_TEST(ThreadSafeQueueTest, MultiThreadTest1) {
  // multi-thread push
  const int num_threads = 16;
  const int num_pushes_per_thread = 1024;
  std::vector<std::thread> threads;
  my::ThreadSafeQueue<TypeParam, my::queue<TypeParam>> queue(
      num_threads * num_pushes_per_thread + 1);

  for (int tid = 0; tid < num_threads; tid++) {
    threads.emplace_back(
        [&queue](int tid) {
          for (int i = tid * num_pushes_per_thread;
               i < (tid + 1) * num_pushes_per_thread; i++) {
            queue.enqueue(TypeParam());
          }
        },
        tid);
  }

  for (auto& thread : threads) {
    thread.join();
  }

  EXPECT_EQ(queue.size(), num_threads * num_pushes_per_thread);
}

TYPED_TEST(ThreadSafeQueueTest, MultiThreadTest2) {
  // multi-thread pop
  const int num_threads = 16;
  const int num_pops_per_thread = 1024;
  std::vector<std::thread> threads;
  my::ThreadSafeQueue<TypeParam, my::queue<TypeParam>> queue(
      num_threads * num_pops_per_thread + 1);

  for (int i = 0; i < num_threads * num_pops_per_thread; i++) {
    queue.enqueue(TypeParam());
  }

  for (int tid = 0; tid < num_threads; tid++) {
    threads.emplace_back([&] {
      for (int i = 0; i < num_pops_per_thread; i++) {
        queue.dequeue();
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }

  EXPECT_TRUE(queue.empty());
}

TYPED_TEST(ThreadSafeQueueTest, MultiThreadTest3) {
  // multi-thread push and pop
  const int num_push_threads = 16;
  const int num_pushes_per_thread = 1024;
  const int num_pop_threads = 16;
  const int num_pops_per_thread = 1024;
  std::vector<std::thread> push_threads;
  std::vector<std::thread> pop_threads;
  my::ThreadSafeQueue<TypeParam, my::queue<TypeParam>> queue(
      num_push_threads * num_pushes_per_thread + 1);

  for (int tid = 0; tid < num_push_threads; tid++) {
    push_threads.emplace_back([&] {
      for (int i = 0; i < num_pushes_per_thread; i++) {
        queue.enqueue(TypeParam());
      }
    });
  }

  for (int tid = 0; tid < num_pop_threads; tid++) {
    pop_threads.emplace_back([&] {
      for (int i = 0; i < num_pops_per_thread; i++) {
        queue.dequeue();
      }
    });
  }

  for (auto& thread : push_threads) {
    thread.join();
  }

  for (auto& thread : pop_threads) {
    thread.join();
  }

  EXPECT_TRUE(queue.empty());
}