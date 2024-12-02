#ifndef LOCK_QUEUE_HPP
#define LOCK_QUEUE_HPP

#include <atomic>
#include <condition_variable>
#include <mutex>

namespace my {

template <typename T>
class queue {
 private:
  std::mutex m_mutex;
  std::condition_variable m_cond_var;
  struct Node {
    T value;
    Node* next;

    Node() : value(T()), next(nullptr) {}
    Node(T val) : value(val), next(nullptr) {}
    Node(T&& val) : value(val), next(nullptr) {}
  };
  Node* m_head;
  Node* m_tail;
  std::atomic<size_t> m_size;
  const size_t m_max_size;

 public:
  queue() : m_head(new Node()), m_tail(m_head), m_size(0), m_max_size(64) {}
  queue(size_t max_size)
      : m_head(new Node()), m_tail(m_head), m_size(0), m_max_size(max_size) {}
  queue(queue&) = delete;
  queue(queue&&) = delete;
  queue& operator=(queue&) = delete;
  queue& operator=(queue&&) = delete;
  ~queue() {
    while (Node* node = m_head) {
      m_head = m_head->next;
      delete node;
    }
  }

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
    m_tail->next = new Node(value);
    m_tail = m_tail->next;
    m_size++;
  }

  void push(T&& value) {
    if (m_size == m_max_size) {
      return;
    }
    m_tail->next = new Node(value);
    m_tail = m_tail->next;
    m_size++;
  }

  T& front() {
    if (!empty()) {
      return m_head->next->value;
    } else {
      return m_head->value;
    }
  }

  void pop() {
    if (empty()) {
      return;
    }
    Node* node = m_head;
    m_head = m_head->next;
    delete node;
    m_size--;
  }

  size_t size() const { return m_size; }

  bool empty() const { return m_size == 0; }
};

}  // namespace my

#endif  // LOCK_QUEUE_HPP
