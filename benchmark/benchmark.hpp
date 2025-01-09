#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <chrono>
#include <iostream>

class Timer {
 public:
  Timer() : start_(std::chrono::high_resolution_clock::now()) {}

  void reset() { start_ = std::chrono::high_resolution_clock::now(); }

  template <typename Duration = std::chrono::microseconds>
  auto elapsed() const {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<Duration>(end - start_).count();
  }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

struct BenchmarkResult {
  double duration_s;
  int push_item_count;
  int pop_item_count;

  double push_qps;
  double pop_qps;
  double total_qps;

  BenchmarkResult(double duration_us, int push_item_count, int pop_item_count)
      : duration_s(duration_us / 1e6),
        push_item_count(push_item_count),
        pop_item_count(pop_item_count),
        push_qps(push_item_count / duration_s),
        pop_qps(pop_item_count / duration_s),
        total_qps((push_item_count + pop_item_count) / duration_s) {}

  void dump() {
    std::cout << "push QPS: " << push_qps << std::endl;
    std::cout << "pop QPS: " << pop_qps << std::endl;
    std::cout << "total QPS: " << total_qps << std::endl;
  }
};

#endif  // BENCHMARK_HPP