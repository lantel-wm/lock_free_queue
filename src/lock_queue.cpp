#include <atomic>
#include <mutex>

template <typename T>
class LockQueue {
 private:
  std::mutex m_mutex;
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
  LockQueue() : m_size(0), m_head(new Node()), m_tail(m_head) {}
  LockQueue(LockQueue&) = delete;
  LockQueue(LockQueue&&) = delete;
  LockQueue& operator=(LockQueue&) = delete;
  LockQueue& operator=(LockQueue&&) = delete;
  ~LockQueue() {
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
      return T();
    }
  }

  void pop() {
    if (m_head->next == nullptr) {
      return;
    }
    Node* node = m_head;
    m_head = m_head->next;
    delete node;
  }

  size_t size() const { return m_size; }

  bool empty() const { return m_size == 0; }
};
