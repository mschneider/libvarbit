#include "benchmark/benchmark.h"
#include "varbit/vector.h"
#include "benchmark/fast_rand.h"

template <typename vector_type>
uint64_t RandomUsingSubscript(const vector_type& vector) {
  uint64_t result = 0;
  for (typename vector_type::size_type i = 0; i < Config().num_elements();
       ++i) {
    unsigned int res[4];
    rand_sse(res);
    uint64_t pos = (uint64_t)*res;
    result += vector[pos % Config().num_elements()];
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

uint64_t RandomGenerator(void*) { //NOLINT: unused
  uint64_t result = 0;
  for (uint64_t i = 0; i < Config().num_elements();
       ++i) {
    unsigned int res[4];
    rand_sse(res);
    uint64_t pos = (uint64_t)*res;
    result += pos;
  }
  return result;
};

template <typename vector_type>
void RandomUsingVarbit(const std::vector<varbit::bit_size_type>& bit_sizes,
         const char* vector_name) {
  Benchmark<const vector_type&, uint64_t> benchmark_subscript(
      RandomUsingSubscript<vector_type>,
      vector_name,
      "RandomUsingSubscript");
  for (std::vector<varbit::bit_size_type>::const_iterator bit_width =
       bit_sizes.begin();
       bit_width != bit_sizes.end();
       ++bit_width) {
    if (*bit_width > vector_type::max_bit_width()) continue;
    vector_type vector(*bit_width, Config().num_elements());
    FillVector<vector_type>(&vector, *bit_width);
    uint64_t result_subscript = benchmark_subscript.run(vector, *bit_width);
  }
}

template <typename vector_type>
void RandomUsingSTL(const std::vector<varbit::bit_size_type>& bit_sizes,
         const char* vector_name) {
  Benchmark<const vector_type&, uint64_t> benchmark_subscript(
      RandomUsingSubscript<vector_type>,
      vector_name,
      "RandomUsingSubscript");
  for (std::vector<varbit::bit_size_type>::const_iterator bit_width =
       bit_sizes.begin();
       bit_width != bit_sizes.end();
       ++bit_width) {
    if (*bit_width > sizeof(typename vector_type::value_type) * 8) continue;
    vector_type vector;
    vector.reserve(Config().num_elements());
    FillVector<vector_type>(&vector, *bit_width);
    uint64_t result_subscript = benchmark_subscript.run(vector, *bit_width);
  }
}

void CostOfRandom() {
  Benchmark<void*, uint64_t> benchmark_random(
      RandomGenerator,
      "*",
      "RandomGenerator");
  volatile uint64_t result = benchmark_random.run(NULL, NULL);
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
  srand_sse(static_cast<unsigned int>(time(0)));

  CostOfRandom();

  RandomUsingVarbit<varbit::vector<uint8_t> >(bit_sizes, "varbit8");
  RandomUsingVarbit<varbit::vector<uint16_t> >(bit_sizes, "varbit16");
  RandomUsingVarbit<varbit::vector<uint32_t> >(bit_sizes, "varbit32");
  RandomUsingVarbit<varbit::vector<uint64_t> >(bit_sizes, "varbit64");
  RandomUsingSTL<std::vector<uint8_t> >(bit_sizes, "std8");
  RandomUsingSTL<std::vector<uint16_t> >(bit_sizes, "std16");
  RandomUsingSTL<std::vector<uint32_t> >(bit_sizes, "std32");
  RandomUsingSTL<std::vector<uint64_t> >(bit_sizes, "std64");
  return 0;
}
