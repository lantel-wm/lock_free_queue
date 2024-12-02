#include <gtest/gtest.h>
#include <sys/types.h>

#include <cstddef>
#include <queue>
#include <vector>

#include "lock_queue.hpp"

TEST(LockQueueTest, PushPoPTest1) {
  my::queue<int> q;
  q.push(1);
  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.size(), 1);
  q.pop();
  EXPECT_EQ(q.size(), 0);
}

TEST(LockQueueTest, PushPoPTest2) {
  std::vector<int> v(1000);
  for (int i = 0; i < 1000; ++i) {
    v[i] = i;
  }
  my::queue<int> q(2000);
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
}

struct Data {
  size_t m_size;
  uint64_t* m_data;

  Data() : m_size(0), m_data(nullptr) {}

  Data(size_t size) : m_size(size), m_data(new uint64_t[size]) {
    for (size_t i = 0; i < size; ++i) {
      m_data[i] = i;
    }
  }

  Data(const Data& rhs) : m_size(rhs.m_size), m_data(new uint64_t[rhs.m_size]) {
    for (size_t i = 0; i < rhs.m_size; ++i) {
      m_data[i] = rhs.m_data[i];
    }
  }

  Data& operator=(const Data& rhs) {
    if (this != &rhs) {
      delete[] m_data;
      m_size = rhs.m_size;
      m_data = new uint64_t[rhs.m_size];
      for (size_t i = 0; i < rhs.m_size; ++i) {
        m_data[i] = rhs.m_data[i];
      }
    }
    return *this;
  }

  Data(Data&& rhs) : m_size(rhs.m_size), m_data(rhs.m_data) {
    rhs.m_size = 0;
    rhs.m_data = nullptr;
  }

  Data& operator=(Data&& rhs) {
    if (this != &rhs) {
      delete[] m_data;
      m_size = rhs.m_size;
      m_data = rhs.m_data;
      rhs.m_size = 0;
      rhs.m_data = nullptr;
    }
    return *this;
  }

  ~Data() { delete[] m_data; }

  bool operator==(const Data& rhs) const {
    if (m_size != rhs.m_size) {
      return false;
    }
    for (size_t i = 0; i < m_size; i++) {
      if (m_data[i] != rhs.m_data[i]) {
        return false;
      }
    }
    return true;
  }
};

// TEST(LockQueueTest, PushPoPTest3) {
//   Data data(10);
//   my::queue<Data> q;
//   q.push(data);
//   EXPECT_EQ(q.size(), 1);
//   EXPECT_EQ(q.front(), data);
// }
