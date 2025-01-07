#include <gtest/gtest.h>
#include <sys/types.h>

#include <queue>
#include <vector>

#include "queues/ring_queue.hpp"
#include "utils/data.hpp"

TEST(RingQueueTest, PushPoPTest1) {
  my::RingQueue<int, 64> q;
  q.push(1);
  EXPECT_EQ(q.front(), 1);
  q.pop();
  EXPECT_TRUE(q.empty());
}

TEST(RingQueueTest, PushPoPTest2) {
  my::RingQueue<int, 64> q;
  q.push(1);
  EXPECT_EQ(q.front(), 1);
  q.pop();
  EXPECT_TRUE(q.empty());
}

TEST(RingQueueTest, PushPoPTest3) {
  std::vector<int> v(1000);
  for (int i = 0; i < 1000; ++i) {
    v[i] = i;
  }
  my::RingQueue<int, 1024> q;
  std::queue<int> q2;
  for (auto i : v) {
    q.push(i);
    q2.push(i);
  }
  while (!q.empty()) {
    EXPECT_EQ(q.front(), q2.front());
    q.pop();
    q2.pop();
  }
  EXPECT_TRUE(q.empty());
}

TEST(RingQueueTest, PushPoPTest4) {
  Data data(10);
  my::RingQueue<Data> q;
  q.push(data);
  EXPECT_EQ(q.front(), data);
}

TEST(RingQueueTest, PushPoPTest5) {
  std::vector<Data> v(1000);
  for (int i = 0; i < 1000; ++i) {
    v[i] = Data(10);
  }
  my::RingQueue<Data, 1024> q;
  std::queue<Data> q2;
  for (auto i : v) {
    q.push(i);
    q2.push(i);
  }
  while (!q.empty()) {
    EXPECT_EQ(q.front(), q2.front());
    q.pop();
    q2.pop();
  }
  EXPECT_TRUE(q.empty());
}
