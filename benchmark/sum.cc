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

// Fill needs to maks bits correctly, so that the sum of all elements
// is always the same - independent of the used data-structure.
template <typename vector_type>
void FillVector(vector_type *vector, const int bit_width) {
  const uint64_t bitmask = ~(~0 << bit_width);
  for (typename vector_type::size_type i = 0; i < Config().num_elements();
       ++i) {
    vector->push_back(i & bitmask);
  }
}

template <typename vector_type>
void SumUsingVarbit(const std::vector<varbit::bit_size_type>& bit_sizes,
         const char* vector_name) {
  Benchmark<const vector_type&, uint64_t> benchmark_subscript(
      SumUsingSubscript<vector_type>,
      vector_name,
      "SumUsingSubscript");
  Benchmark<const vector_type&, uint64_t> benchmark_iterator(
      SumUsingIterator<vector_type>,
      vector_name,
      "SumUsingIterator");
  for (std::vector<varbit::bit_size_type>::const_iterator bit_width =
       bit_sizes.begin();
       bit_width != bit_sizes.end();
       ++bit_width) {
    if (*bit_width > vector_type::max_bit_width()) continue;
    vector_type vector(*bit_width, Config().num_elements());
    FillVector<vector_type>(&vector, *bit_width);
    uint64_t result_subscript = benchmark_subscript.run(vector, *bit_width);
    uint64_t result_iterator = benchmark_iterator.run(vector, *bit_width);
    assert(result_subscript == result_iterator);
  }
}

template <typename vector_type>
void SumUsingSTL(const std::vector<varbit::bit_size_type>& bit_sizes,
         const char* vector_name) {
  Benchmark<const vector_type&, uint64_t> benchmark_subscript(
      SumUsingSubscript<vector_type>,
      vector_name,
      "SumUsingSubscript");
  Benchmark<const vector_type&, uint64_t> benchmark_iterator(
      SumUsingIterator<vector_type>,
      vector_name,
      "SumUsingIterator");
  for (std::vector<varbit::bit_size_type>::const_iterator bit_width =
       bit_sizes.begin();
       bit_width != bit_sizes.end();
       ++bit_width) {
    if (*bit_width > sizeof(typename vector_type::value_type) * 8) continue;
    vector_type vector;
    vector.reserve(Config().num_elements());
    FillVector<vector_type>(&vector, *bit_width);
    uint64_t result_subscript = benchmark_subscript.run(vector, *bit_width);
    uint64_t result_iterator = benchmark_iterator.run(vector, *bit_width);
    assert(result_subscript == result_iterator);
  }
}

int main(int argc, char** argv) {
  Configuration config = Config(argc, argv);
  std::vector<varbit::bit_size_type> bit_sizes;
  bit_sizes.push_back(1);
  bit_sizes.push_back(2);
  bit_sizes.push_back(4);
  bit_sizes.push_back(8);
  bit_sizes.push_back(16);
  bit_sizes.push_back(32);
  bit_sizes.push_back(64);
  SumUsingVarbit<varbit::vector<uint8_t> >(bit_sizes, "varbit8");
  SumUsingVarbit<varbit::vector<uint16_t> >(bit_sizes, "varbit16");
  SumUsingVarbit<varbit::vector<uint32_t> >(bit_sizes, "varbit32");
  SumUsingVarbit<varbit::vector<uint64_t> >(bit_sizes, "varbit64");
  SumUsingSTL<std::vector<uint8_t> >(bit_sizes, "std8");
  SumUsingSTL<std::vector<uint16_t> >(bit_sizes, "std16");
  SumUsingSTL<std::vector<uint32_t> >(bit_sizes, "std32");
  SumUsingSTL<std::vector<uint64_t> >(bit_sizes, "std64");
  return 0;
}
