#ifndef RING_QUEUE_HPP
#define RING_QUEUE_HPP

#include <cassert>
#include <cstdlib>

namespace my {

template <typename T, size_t RingBufferSize = 64>
class RingQueue {
 private:
  T m_ring_buffer[RingBufferSize];
  size_t m_head{};
  size_t m_tail{};
  const size_t m_ring_buffer_size;

  size_t next_power_of_two(size_t n) {
    if (n == 0) return 1;
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return n + 1;
  }

 public:
  RingQueue() : m_ring_buffer_size(next_power_of_two(RingBufferSize)) {
    for (size_t i = 0; i < m_ring_buffer_size; i++) {
      m_ring_buffer[i] = T();
    }
  }
  RingQueue(RingQueue&) = delete;
  RingQueue(RingQueue&&) = delete;
  RingQueue& operator=(RingQueue&) = delete;
  RingQueue& operator=(RingQueue&&) = delete;

  bool size() const noexcept {
    assert(m_head <= m_tail);
    return m_tail - m_head;
  }

  size_t capacity() const { return m_ring_buffer_size; }

  bool empty() const { return size() == 0; }

  bool full() const { return size() == capacity(); }

  bool push(const T& value) {
    if (full()) {
      return false;
    }
    m_ring_buffer[m_tail & (m_ring_buffer_size - 1)] = value;
    ++m_tail;
    return true;
  }

  bool push(T&& value) {
    if (full()) {
      return false;
    }
    m_ring_buffer[m_tail & (m_ring_buffer_size - 1)] = value;
    ++m_tail;
    return true;
  }

  T& front() { return m_ring_buffer[m_head]; }

  bool pop() {
    if (empty()) {
      return false;
    }
    m_ring_buffer[m_head] = T();
    ++m_head;
    return true;
  }
};

}  // namespace my

#endif  // RING_QUEUE_HPP