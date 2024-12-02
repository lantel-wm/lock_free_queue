#include <thread>
#include <vector>

#include "thread_safe_queue.hpp"

int main() {
  const int num_threads = 4;
  const int num_pops_per_thread = 4;
  std::vector<std::thread> threads;
  my::ThreadSafeQueue<int, my::queue<int>> queue(
      num_threads * num_pops_per_thread + 1);

  for (int i = 0; i < num_threads * num_pops_per_thread; i++) {
    queue.enqueue(i);
  }

  for (int tid = 0; tid < num_threads; tid++) {
    threads.emplace_back([&] {
      for (int i = 0; i < num_pops_per_thread; i++) {
        queue.dequeue();
      }
    });
  }

  for (auto& thread : threads) {
    thread.join();
  }
}