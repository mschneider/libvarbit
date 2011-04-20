#ifndef BENCHMARK_TIMER_H_
#define BENCHMARK_TIMER_H_

#ifdef USE_PAPI
#include <papi.h>

const int num_papi_counters = 2;
// Must not be const, because PAPI's interface is weird.
int papi_counters[NUM_PAPI_COUNTERS] = {PAPI_TOT_CYC, PAPI_TOT_INS};

class Timer {
  long long int papi_values_[num_papi_counters]; // NOLINT PAPI interface again

 public:
  static void Init() {
    const int status = PAPI_start_counters(papi_counters, num_papi_counters);
    if (status != PAPI_OK) {
      std::cout << "Can't start PAPI Counters (" << status << ")." << std::endl;
      exit(1);
    }
  }

  void PrintHeader() const {
    for (int i = 0; i < num_papi_counters; ++i) {
      char event_name[PAPI_MAX_STR_LEN];
      PAPI_event_code_to_name(papi_counters[i], event_name);
      std::cout << ',' << event_name;
    }
  }

  void PrintMeasurements() const {
    for (int i = 0; i < num_papi_counters; ++i) {
      std::cout << ',' << papi_values_[i];
    }
  }

  void Start() {
    PAPI_read_counters(papi_values_, num_papi_counters);
  }

  void Stop() {
    PAPI_read_counters(papi_values_, num_papi_counters);
  }
};

#else  // USE_PAPI was not defined - using gettimeofday instead.

#include <sys/time.h>

class Timer {
  struct timeval begin_, end_;

 public:
  static void Init() { }

  void PrintHeader() const { std::cout << ",seconds"; }

  void PrintMeasurements() const {
    std::cout << ',' << end_.tv_sec - begin_.tv_sec +
        0.000001 * (end_.tv_usec - begin_.tv_usec);
  }

  void Start() { gettimeofday(&begin_, NULL); }

  void Stop() { gettimeofday(&end_, NULL); }
};
#endif

#endif  // BENCHMARK_TIMER_H_
