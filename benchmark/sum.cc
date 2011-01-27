#include "include/vector.h"
#include "benchmark/benchmark.h"

template <typename vector_type, typename size_type>
size_type sum_using_subscript(const vector_type& vector) {
  size_type result = 0;
  for (size_type i = 0; i < Config().num_elements(); ++i) {
    result += vector[i];
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
  Benchmark<vector_type, typename vector_type::size_type> benchmark(
      sum_using_subscript<vector_type, typename vector_type::size_type>,
      vector_name,
      "sum_using_subscript");
  for (int bit_width = 1; bit_width <= max_bit_width; ++bit_width) {
    vector_type vector;
    vector.reserve(Config().num_elements());
    FillVector<vector_type>(&vector, bit_width);
    typename vector_type::size_type result = benchmark.run(vector, bit_width);
    std::cout << "Result: " << result << std::endl;
  }
}

template <typename vector_type>
void SumUsingVarbit(const char* vector_name) {
  Benchmark<vector_type, typename vector_type::size_type> benchmark(
      sum_using_subscript<vector_type, typename vector_type::size_type>,
      vector_name,
      "sum_using_subscript");
  for (typename vector_type::bit_size_type bit_width = 1;
       bit_width <= vector_type::max_bit_width(); ++bit_width) {
    vector_type vector(bit_width, Config().num_elements());
    FillVector<vector_type>(&vector, bit_width);
    typename vector_type::size_type result = benchmark.run(vector, bit_width);
    std::cout << "Result: " << result << std::endl;
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
