#include <queue>

#include "benchmark/benchmark.hpp"
#include "queues/base_queue.hpp"
#include "queues/ring_queue.hpp"

template <typename T, typename Queue, int Iters = 10>
BenchmarkResult benchmark(const int push_item_count, const int pop_item_count,
                          Queue queue) {
  Timer timer;

  for (int t = 0; t < Iters; t++) {
    for (int i = 0; i < push_item_count; i++) {
      queue.push(T());
    }

    for (int i = 0; i < pop_item_count; i++) {
      queue.pop();
    }
  }

  BenchmarkResult result(timer.elapsed() / static_cast<float>(Iters),
                         push_item_count, pop_item_count);
  return result;
}

int main() {
  const int push_item_count = 1000000;
  const int pop_item_count = 1000000;

  std::cout << "push_item_count  : " << push_item_count << std::endl;
  std::cout << "pop_item_count   : " << pop_item_count << std::endl;

  BenchmarkResult result_base_queue = benchmark<int, my::BaseQueue<int>>(
      push_item_count, pop_item_count, my::BaseQueue<int>());

  BenchmarkResult result_ring_queue =
      benchmark<int, my::RingQueue<int, push_item_count>>(
          push_item_count, pop_item_count,
          my::RingQueue<int, push_item_count>());

  BenchmarkResult result_std_queue = benchmark<int, std::queue<int>>(
      push_item_count, pop_item_count, std::queue<int>());

  std::cout << "BaseQueue: " << std::endl;
  result_base_queue.dump();
  std::cout << std::endl;

  std::cout << "RingQueue: " << std::endl;
  result_ring_queue.dump();
  std::cout << std::endl;

  std::cout << "std::queue: " << std::endl;
  result_std_queue.dump();
  std::cout << std::endl;
}