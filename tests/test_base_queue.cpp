#include <gtest/gtest.h>
#include <sys/types.h>

#include <queue>
#include <vector>

#include "queues/base_queue.hpp"
#include "utils/data.hpp"

TEST(QueueTest, PushPoPTest1) {
  my::queue<int> q;
  q.push(1);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.size(), 1);
  q.pop();
  EXPECT_TRUE(q.empty());
}

TEST(QueueTest, PushPoPTest2) {
  std::vector<int> v(1000);
  for (int i = 0; i < 1000; ++i) {
    v[i] = i;
  }
  my::queue<int> q;
  std::queue<int> q2;
  for (auto i : v) {
    q.push(i);
    q2.push(i);
  }
  EXPECT_EQ(q.size(), q2.size());
  while (!q.empty()) {
    EXPECT_EQ(q.front(), q2.front());
    q.pop();
    q2.pop();
  }
  EXPECT_TRUE(q.empty());
}

TEST(QueueTest, PushPoPTest3) {
  Data data(10);
  my::queue<Data> q;
  q.push(data);
  EXPECT_EQ(q.size(), 1);
  EXPECT_EQ(q.front(), data);
}

TEST(QueueTest, PushPoPTest4) {
  std::vector<Data> v(1000);
  for (int i = 0; i < 1000; ++i) {
    v[i] = Data(10);
  }
  my::queue<Data> q;
  std::queue<Data> q2;
  for (auto i : v) {
    q.push(i);
    q2.push(i);
  }
  EXPECT_EQ(q.size(), q2.size());
  while (!q.empty()) {
    EXPECT_EQ(q.front(), q2.front());
    q.pop();
    q2.pop();
  }
  EXPECT_TRUE(q.empty());
}
