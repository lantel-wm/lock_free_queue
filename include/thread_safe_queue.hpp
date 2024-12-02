#ifndef THREAD_SAFE_QUEUE
#define THREAD_SAFE_QUEUE

#include <condition_variable>
#include <mutex>

namespace my {

template <typename T, typename Container>
class ThreadSafeQueue {
 private:
  Container m_container;
  std::mutex m_mutex;
  std::condition_variable m_cond_var;
  const std::size_t m_max_size;

 public:
  ThreadSafeQueue() : m_max_size(64) {}
  ThreadSafeQueue(std::size_t max_size) : m_max_size(max_size) {}
  ThreadSafeQueue(const ThreadSafeQueue&) = delete;
  ThreadSafeQueue(ThreadSafeQueue&&) = delete;
  ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;
  ThreadSafeQueue& operator=(ThreadSafeQueue&&) = delete;

  void enqueue(T& value) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond_var.wait(lock, [this] { return m_container.size() < m_max_size; });
    m_container.push(value);
    m_cond_var.notify_all();
  }

  void enqueue(T&& value) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond_var.wait(lock, [this] { return m_container.size() < m_max_size; });
    m_container.push(std::move(value));
    m_cond_var.notify_all();
  }

  T& dequeue() {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_cond_var.wait(lock, [this] { return m_container.size() > 0; });
    T item = m_container.front();
    m_container.pop();
    return item;
  }
};

}  // namespace my

#endif  // THREAD_SAFE_QUEUE