#include "include/vector.h"
#include "benchmark/benchmark.h"

typedef varbit::vector<uint8_t> bcv8;
typedef varbit::vector<uint16_t> bcv16;
typedef varbit::vector<uint32_t> bcv32;
typedef varbit::vector<uint64_t> bcv64;

template <typename vector_type, typename size_type>
size_type sum_using_subscript(const vector_type& vector) {
  size_type result = 0;
  for (size_type i = 0; i < Config().num_elements(); ++i) {
    result += vector[i];
  }
  return result;
};

int main(int argc, char** argv) {
  Configuration config = Config(argc, argv);
  for (bcv8::bit_size_type bit_width = 1; bit_width <= 8*sizeof(uint8_t);
                                         ++bit_width) {
    bcv8 vector(bit_width, Config().num_elements());
    for (bcv8::size_type i = 0; i < Config().num_elements(); ++i) {
     vector.push_back(i);
    }
    Benchmark<bcv8, bcv8::size_type> benchmark(
        sum_using_subscript<bcv8, bcv8::size_type>);
    bcv8::size_type result = benchmark.run(vector, "bcv8:sum_using_subscript");
    std::cout << "Result: " << result << std::endl;
  }
  return 0;
}
