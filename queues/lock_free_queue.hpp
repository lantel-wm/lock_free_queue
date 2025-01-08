// Single producer single consumer wait-free lock-free queue
#ifndef LOCK_FREE_QUEUE
#define LOCK_FREE_QUEUE

#include <atomic>
#include <cassert>

namespace my {
template <typename T, std::size_t RingBufferSize = 64>
class LockFreeQueue {
 private:
  T m_ring_buffer[RingBufferSize];

  using CursorType = std::atomic<std::size_t>;
  static_assert(CursorType::is_always_lock_free);

  static constexpr auto hardware_destructive_interference_size =
      std::size_t{64};

  /// Loaded and stored by the push thread; loaded by the pop thread
  alignas(hardware_destructive_interference_size) CursorType m_push_cursor;

  /// Loaded and stored by the pop thread; loaded by the push thread
  alignas(hardware_destructive_interference_size) CursorType m_pop_cursor;

  // Padding to avoid false sharing with adjacent objects
  char m_padding[hardware_destructive_interference_size - sizeof(std::size_t)];

 private:
  bool full(std::size_t pushCursor, std::size_t popCursor) const noexcept {
    return (pushCursor - popCursor) == RingBufferSize;
  }
  static bool empty(std::size_t pushCursor, std::size_t popCursor) noexcept {
    return pushCursor == popCursor;
  }

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
    auto push_cursor = m_push_cursor.load(std::memory_order_relaxed);
    auto pop_cursor = m_pop_cursor.load(std::memory_order_acquire);
    if (full(push_cursor, pop_cursor)) {
      return false;
    }
    m_ring_buffer[push_cursor % RingBufferSize] = value;
    m_push_cursor.store(m_push_cursor + 1, std::memory_order_release);
    return true;
  }

  bool pop(T& value) {
    auto push_cursor = m_push_cursor.load(std::memory_order_acquire);
    auto pop_cursor = m_pop_cursor.load(std::memory_order_relaxed);
    if (empty(push_cursor, pop_cursor)) {
      return false;
    }
    value = m_ring_buffer[pop_cursor % RingBufferSize];
    m_pop_cursor.store(m_pop_cursor + 1, std::memory_order_release);
    return true;
  }
};
}  // namespace my

#endif  // LOCK_FREE_QUEUE