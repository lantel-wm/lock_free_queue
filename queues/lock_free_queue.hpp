// Single producer single consumer wait-free lock-free queue
#ifndef LOCK_FREE_QUEUE
#define LOCK_FREE_QUEUE

#include <atomic>
#include <cassert>

namespace my {
template <typename T, std::size_t RingBufferSize = 64>
class LockFreeQueue {
 private:
  using CursorType = std::atomic<std::size_t>;
  static_assert(CursorType::is_always_lock_free);

  T m_ring_buffer[RingBufferSize];
  CursorType m_pop_cursor{};
  CursorType m_push_cursor{};

 public:
  LockFreeQueue() {
    for (std::size_t i = 0; i < RingBufferSize; i++) {
      m_ring_buffer[i] = T();
    }
  }
  LockFreeQueue(LockFreeQueue&) = delete;
  LockFreeQueue(LockFreeQueue&&) = delete;
  LockFreeQueue& operator=(LockFreeQueue&) = delete;
  LockFreeQueue& operator=(LockFreeQueue&&) = delete;

  std::size_t size() const noexcept {
    assert(m_pop_cursor <= m_push_cursor);
    return m_push_cursor - m_pop_cursor;
  }

  std::size_t capacity() const noexcept { return RingBufferSize; }

  bool empty() const noexcept { return size() == 0; }

  bool full() const noexcept { return size() == capacity(); }

  bool push(const T& value) {
    if (full()) {
      return false;
    }
    m_ring_buffer[m_push_cursor % RingBufferSize] = value;
    ++m_push_cursor;
    return true;
  }

  bool pop(T& value) {
    if (empty()) {
      return false;
    }
    value = m_ring_buffer[m_pop_cursor % RingBufferSize];
    ++m_pop_cursor;
    return true;
  }
};
}  // namespace my

#endif  // LOCK_FREE_QUEUE