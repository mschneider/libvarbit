#ifdef USE_PAPI
    #include <papi.h>
#endif

class BenchmarkHelper {
    void setColor() {
        #ifdef USE_COLORS
            std::cout << "\033[34m";
        #endif
    }
    
    void resetColor() {
        #ifdef USE_COLORS
            std::cout << "\033[0m";
        #endif
    }

#ifdef USE_PAPI
    int *events;
    unsigned int num_events;
    long long *values;
    
  public:
    BenchmarkHelper(int events[], int num_events)
    :
        events(events),
        num_events(num_events),
        values(new long long[num_events])
        
    {
        const int status = PAPI_start_counters(events, num_events);
        if(status != PAPI_OK) {
            std::cout << "Can't start PAPI Counters (" << status << ")." << std::endl;
            exit(1);
        }
    };
      
    void resetBenchmark() {
        const int status = PAPI_read_counters(values, num_events) != PAPI_OK;
        if(status != PAPI_OK) {
            std::cout << "Can't read PAPI Counters (" << status << ")." << std::endl;
            exit(1);
        }
    }
        
    
    void printBenchmark(std::string str) {
        if(PAPI_read_counters(values, num_events) != PAPI_OK) {
            std::cout << "Can't read PAPI Counters." << std::endl;
            exit(1);
        }
        setColor();
        std::cout << "[BENCHMARK] " << str << " = {";
        for(unsigned int i = 0; i < num_events; ++i) {
            char event_name[PAPI_MAX_STR_LEN] = "";
            PAPI_event_code_to_name(events[i], event_name);
            std::cout << event_name << ": " << values[i] << (i < num_events - 1 ? ", " : "");
        }
        std::cout << "}" << std::endl;
        resetColor();
    }

#else
    timeval startTime;

  public:
    BenchmarkHelper() {
        gettimeofday(&startTime, NULL);
    }
    
    void resetBenchmark() {
        gettimeofday(&startTime, NULL);
    }
    
    void printBenchmark(std::string str) {
        timeval currentTime;
        gettimeofday(&currentTime, NULL);
        setColor();
        std::cout << "[BENCHMARK] " << str << " = " << currentTime.tv_sec - startTime.tv_sec + (currentTime.tv_usec - startTime.tv_usec) / 1000000.0 << std::endl;
        resetColor();
        resetBenchmark();
    }
#endif

};
