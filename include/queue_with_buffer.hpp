#ifndef QUEUE_WITH_BUFFER_HPP
#define QUEUE_WITH_BUFFER_HPP

#include <atomic>
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <utility>

namespace my {

template <typename T>
class queue {
 private:
  std::mutex m_mutex;
  std::condition_variable m_cond_var;
  std::atomic<size_t> m_size;
  const size_t m_max_size;
  T* m_buffer;
  size_t m_head;
  size_t m_tail;

  size_t inc(size_t index) { return (index + 1) % m_max_size; }
  size_t dec(size_t index) { return (index - 1 + m_max_size) % m_max_size; }

 public:
  queue()
      : m_size(0),
        m_max_size(64),
        m_buffer(new T[m_max_size]),
        m_head(0),
        m_tail(0) {
    for (size_t i = 0; i < m_max_size; i++) {
      m_buffer[i] = T();
    }
  }
  queue(size_t max_size)
      : m_size(0),
        m_max_size(max_size),
        m_buffer(new T[m_max_size]),
        m_head(0),
        m_tail(0) {
    for (size_t i = 0; i < m_max_size; i++) {
      m_buffer[i] = T();
    }
  }
  queue(queue&) = delete;
  queue(queue&&) = delete;
  queue& operator=(queue&) = delete;
  queue& operator=(queue&&) = delete;
  ~queue() { delete[] m_buffer; }

  void enqueue(T& value) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond_var.wait(lock, [this] { return m_size < m_max_size; });
    push(std::move(value));
    m_cond_var.notify_all();
  }

  void push(T& value) {
    if (m_size == m_max_size) {
      return;
    }
    m_buffer[m_tail] = value;
    m_tail = inc(m_tail);
    m_size++;
  }

  void push(T&& value) {
    if (m_size == m_max_size) {
      return;
    }
    m_buffer[m_tail] = std::move(value);
    m_tail = inc(m_tail);
    m_size++;
  }

  T& front() { return m_buffer[m_head]; }

  void pop() {
    if (empty()) {
      return;
    }
    m_head = inc(m_head);
    m_size--;
  }

  size_t size() const { return m_size; }

  bool empty() const { return m_size == 0; }
};

}  // namespace my

#endif  // QUEUE_WITH_BUFFER_HPP
