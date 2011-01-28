#include "benchmark/benchmark.h"
#include "varbit/vector.h"

template <typename vector_type>
uint64_t SumUsingSubscript(const vector_type& vector) {
  uint64_t result = 0;
  for (typename vector_type::size_type i = 0; i < Config().num_elements();
       ++i) {
    result += vector[i];
  }
  return result;
};

template <typename vector_type>
uint64_t SumUsingIterator(const vector_type& vector) {
  uint64_t result = 0;
  for (typename vector_type::const_iterator it = vector.begin();
       it != vector.end(); ++it) {
    result += *it;
  }
  return result;
};

template <typename vector_type>
void FillVector(vector_type *vector, const int bit_width) {
  const uint64_t bitmask = ~(~0 << bit_width);
  for (typename vector_type::size_type i = 0; i < Config().num_elements();
       ++i) {
    vector->push_back(i & bitmask);
  }
}

template <typename vector_type>
void SumUsingSTL(const char* vector_name, const int max_bit_width) {
  Benchmark<const vector_type&, uint64_t> benchmark_subscript(
      SumUsingSubscript<vector_type>,
      vector_name,
      "SumUsingSubscript");
  Benchmark<const vector_type&, uint64_t> benchmark_iterator(
      SumUsingIterator<vector_type>,
      vector_name,
      "SumUsingIterator");
  for (int bit_width = 1; bit_width <= max_bit_width; ++bit_width) {
    vector_type vector;
    vector.reserve(Config().num_elements());
    FillVector<vector_type>(&vector, bit_width);
    volatile uint64_t result_subscript = benchmark_subscript.run(vector,
                                                                 bit_width);
    volatile uint64_t result_iterator = benchmark_iterator.run(vector,
                                                               bit_width);
    assert(result_subscript == result_iterator);
  }
}

template <typename vector_type>
void SumUsingVarbit(const char* vector_name) {
  Benchmark<const vector_type&, uint64_t> benchmark_subscript(
      SumUsingSubscript<vector_type>,
      vector_name,
      "SumUsingSubscript");
  for (typename vector_type::bit_size_type bit_width = 1;
       bit_width <= vector_type::max_bit_width(); ++bit_width) {
    vector_type vector(bit_width, Config().num_elements());
    FillVector<vector_type>(&vector, bit_width);
    volatile uint64_t result = benchmark_subscript.run(vector, bit_width);
  }
}

int main(int argc, char** argv) {
  Configuration config = Config(argc, argv);
  SumUsingVarbit<varbit::vector<uint8_t> >("varbit8");
  SumUsingVarbit<varbit::vector<uint16_t> >("varbit16");
  SumUsingVarbit<varbit::vector<uint32_t> >("varbit32");
  SumUsingVarbit<varbit::vector<uint64_t> >("varbit64");
  SumUsingSTL<std::vector<uint8_t> >("std8", 8);
  SumUsingSTL<std::vector<uint16_t> >("std16", 16);
  SumUsingSTL<std::vector<uint32_t> >("std32", 32);
  SumUsingSTL<std::vector<uint64_t> >("std64", 64);
  return 0;
}
