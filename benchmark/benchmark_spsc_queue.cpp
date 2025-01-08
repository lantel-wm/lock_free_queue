#include <cstddef>
#include <queue>
#include <thread>

#include "benchmark/benchmark.hpp"
#include "queues/lock_free_queue.hpp"
#include "queues/thread_safe_queue.hpp"

template <typename T, typename Queue, int Iters = 100>
BenchmarkResult benchmark_spsc(const int push_item_count,
                               const int pop_item_count) {
  Timer timer;

  for (int t = 0; t < Iters; t++) {
    Queue queue;
    T value{};
    std::thread push_thread([&] {
      for (int i = 0; i < push_item_count; i++) {
        queue.push(value);
      }
    });

    std::thread pop_thread([&] {
      for (int i = 0; i < pop_item_count; i++) {
        queue.pop(value);
      }
    });

    push_thread.join();
    pop_thread.join();
  }

  BenchmarkResult result(timer.elapsed() / static_cast<float>(Iters),
                         push_item_count, pop_item_count);
  return result;
}

int main() {
  const size_t push_item_count = (1 << 20);
  const size_t pop_item_count = (1 << 20);

  std::cout << "push_item_count  : " << push_item_count << std::endl;
  std::cout << "pop_item_count   : " << pop_item_count << std::endl;

  BenchmarkResult lock_free_queue_result =
      benchmark_spsc<int, my::LockFreeQueue<int, push_item_count>>(
          push_item_count, pop_item_count);

  std::cout << "LockFreeQueue: " << std::endl;
  lock_free_queue_result.dump();
  std::cout << std::endl;

  BenchmarkResult thread_safe_queue_result =
      benchmark_spsc<int, my::ThreadSafeQueue<int, std::queue<int>>>(
          push_item_count, pop_item_count);

  std::cout << "ThreadSafeQueue: " << std::endl;
  thread_safe_queue_result.dump();
  std::cout << std::endl;
}