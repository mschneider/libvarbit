#ifndef LIBVARBIT_BENCHMARK_H_
#define LIBVARBIT_BENCHMARK_H_

#include <sys/mman.h>
#include <iostream>

// The Benchmark class should be initialized at the beginning of every program
// using the main parameters.
// Sample usage to measure the function int foo(int) with int bar as parameter:
// Benchmark<int, int> benchmark(&foo);
// int result = benchmark.run(bar);
template<typename Input, typename Result>
class Benchmark {
 public:
  Benchmark(Result(*function)(Input)) : function_(function) { }
  Result run(String label, Input input);
  static void initialize(int argc, char **argv);
 private:
  Result(*function_)(Input);
};

void Benchmark::initialize(int, char **) {
  if (!mlockall(MCL_CURRENT & MCL_FUTURE)) {
    std::cerr << "mlockall failed: Couldn't lock memory." << std::endl;
    exit(1);
  }
}

Result Benchmark::run(Input input) {
  Result result = _function(input);
  std::cout<<"Benchmarked: "<<label<<std::endl;
  return result;
}
#endif  // LIBVARBIT_BENCHMARK_H_
