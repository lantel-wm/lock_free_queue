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

 public:
  RingQueue() {
    assert((RingBufferSize & (RingBufferSize - 1)) == 0);
    for (size_t i = 0; i < RingBufferSize; i++) {
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

  size_t capacity() const { return RingBufferSize; }

  bool empty() const { return size() == 0; }

  bool full() const { return size() == capacity(); }

  bool push(const T& value) {
    if (full()) {
      return false;
    }
    m_ring_buffer[m_tail & (RingBufferSize - 1)] = value;
    // m_ring_buffer[m_tail % RingBufferSize] = value;
    ++m_tail;
    return true;
  }

  bool push(T&& value) {
    if (full()) {
      return false;
    }
    // compiler also do this optimization (% 2^k -> &(2^k - 1))
    m_ring_buffer[m_tail & (RingBufferSize - 1)] = value;
    // m_ring_buffer[m_tail % RingBufferSize] = value;
    ++m_tail;
    return true;
  }

  T& front() { return m_ring_buffer[m_head & (RingBufferSize - 1)]; }
  // T& front() { return m_ring_buffer[m_head % RingBufferSize]; }

  bool pop() {
    if (empty()) {
      return false;
    }
    // m_ring_buffer[m_head & (RingBufferSize - 1)] = T();
    // m_ring_buffer[m_head % RingBufferSize] = T();
    ++m_head;
    return true;
  }
};

}  // namespace my

#endif  // RING_QUEUE_HPP