#ifndef BENCHMARK_BENCHMARK_H_
#define BENCHMARK_BENCHMARK_H_

#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <papi.h>
#include <iostream>

#define PAPI_COUNTERS {PAPI_TOT_CYC, PAPI_TOT_INS}
#define PAPI_NUM_COUNTERS 2

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

Configuration::Configuration(int argc, char **argv) {
  if (!mlockall(MCL_CURRENT & MCL_FUTURE)) {
    std::cerr << "mlockall failed: Couldn't lock memory." << std::endl;
    exit(1);
  }
  int counters[] = PAPI_COUNTERS;
  const int status = PAPI_start_counters(counters, PAPI_NUM_COUNTERS);
  if(status != PAPI_OK) {
    std::cout << "Can't start PAPI Counters (" << status << ")." << std::endl;
    exit(1);
  }
  if(argc != 2) {
    std::cout << "Please specify the number of elements as a parameter. Kthxbai." << std::endl;
    exit(1);
  }
  num_elements_ = atoi(argv[1]) * 1024 * 1024;
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
  long long *papi_values_;
  static int papi_counters_[];
};

template<typename Input, typename Result>
int Benchmark<Input, Result>::papi_counters_[] = PAPI_COUNTERS;

template<typename Input, typename Result>
Benchmark<Input, Result>::Benchmark(Result(*function)(const Input&),
                                    const char* data_structure_name,
                                    const char* function_name)
    : function_(function),
      function_name_(function_name),
      data_structure_name_(data_structure_name),
      papi_values_(new long long[PAPI_NUM_COUNTERS]) {
    std::cout << "#function, data structure, bit width";
    for(unsigned int i = 0; i < PAPI_NUM_COUNTERS; ++i) {
      char event_name[PAPI_MAX_STR_LEN] = "";
      PAPI_event_code_to_name(papi_counters_[i], event_name);
      std::cout << ", " << event_name;
    }
    std::cout << std::endl;
  }

template<typename Input, typename Result>
Result Benchmark<Input, Result>::run(const Input& input, const int bit_width) {
  PAPI_read_counters(papi_values_, PAPI_NUM_COUNTERS);
  Result result = function_(input);
  PAPI_read_counters(papi_values_, PAPI_NUM_COUNTERS);
  
  std::cout << function_name_
            << ", "
            << data_structure_name_
            << ", "
            << bit_width
            << ", ";
  for(unsigned int i = 0; i < PAPI_NUM_COUNTERS; ++i) {
    std::cout << papi_values_[i] << (i < PAPI_NUM_COUNTERS - 1 ? ", " : "");
  }
  std::cout << std::endl;
            
  return result;
}
#endif  // BENCHMARK_BENCHMARK_H_
