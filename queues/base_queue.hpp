#ifndef BASE_QUEUE_HPP
#define BASE_QUEUE_HPP

#include <atomic>
#include <cstddef>

namespace my {

template <typename T>
class BaseQueue {
 private:
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

 public:
  BaseQueue() : m_head(new Node()), m_tail(m_head), m_size(0) {}
  BaseQueue(BaseQueue&) = delete;
  BaseQueue(BaseQueue&&) = delete;
  BaseQueue& operator=(BaseQueue&) = delete;
  BaseQueue& operator=(BaseQueue&&) = delete;
  ~BaseQueue() {
    while (Node* node = m_head) {
      m_head = m_head->next;
      delete node;
    }
  }

  void push(T& value) {
    m_tail->next = new Node(value);
    m_tail = m_tail->next;
    m_size++;
  }

  void push(T&& value) {
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

  size_t size() const { return m_size.load(); }

  bool empty() const { return m_size.load() == 0; }
};

}  // namespace my

#endif  // QUEUE_HPP
