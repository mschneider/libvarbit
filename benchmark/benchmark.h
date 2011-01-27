#ifndef BENCHMARK_BENCHMARK_H_
#define BENCHMARK_BENCHMARK_H_

#include <sys/mman.h>
#include <sys/time.h>  // TODO(mrks): switch to PAPI
#include <iostream>

// Simple Configuration class handling command lines parameters and benchmark
// global setup. Aways create an instance of this in your main:
// Configuration x = Config(argc, argv);
// Subsequent argumentless calls to Config() will return the same Configuration.
class Configuration {
 public:
  Configuration(int argc, char **argv);
  const uint64_t num_elements() { return num_elements_; }
 private:
  uint64_t num_elements_;
};

Configuration::Configuration(int argc, char **) {
  if (!mlockall(MCL_CURRENT & MCL_FUTURE)) {
    std::cerr << "mlockall failed: Couldn't lock memory." << std::endl;
    exit(1);
  }
  num_elements_ = argc * 1024 * 1024;
}

Configuration Config(int argc = 0, char **argv = NULL) {
  static Configuration instance(argc, argv);
  return instance;
}

// Sample usage to measure the function int foo(int) with int bar as parameter:
// Benchmark<int, int> benchmark(&foo);
// int result = benchmark.run(bar);
// Make sure to initialize Configuration before!
template <typename Input, typename Result>
class Benchmark {
 public:
  Benchmark(Result(*function)(const Input&), const char* function_name,
            const char* data_structure_name);
  Result run(const Input& input, const int bit_width);
 private:
  Result(*function_)(const Input&);
  const char* function_name_;
  const char* data_structure_name_;
};

template<typename Input, typename Result>
Benchmark<Input, Result>::Benchmark(Result(*function)(const Input&),
                                    const char* data_structure_name,
                                    const char* function_name)
    : function_(function),
      function_name_(function_name),
      data_structure_name_(data_structure_name) { }

template<typename Input, typename Result>
Result Benchmark<Input, Result>::run(const Input& input, const int bit_width) {
  timeval t_start, t_end;
  gettimeofday(&t_start, NULL);
  Result result = function_(input);
  gettimeofday(&t_end, NULL);
  std::cout << function_name_
            << ','
            << data_structure_name_
            << ','
            << bit_width
            << ','
            << t_end.tv_sec - t_start.tv_sec +
               (t_end.tv_usec - t_start.tv_usec) / 1000000.0
            << std::endl;
  return result;
}
#endif  // BENCHMARK_BENCHMARK_H_
