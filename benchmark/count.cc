#include "benchmark.h"
#include "bitvector.h"

typedef bitvector<uint8_t> bcv8_t;
typedef bitvector<uint16_t> bcv16_t;
typedef bitvector<uint32_t> bcv32_t;
typedef bitvector<uint64_t> bcv64_t;


void runWithSingleBitwidth() {
  
}

template<typename _bitvector_type>
void runWithEveryBitwidth(){
  const _bitvector_type::bit_count_t max_bit_width = 8 * sizeof(_bitvector_type::block_t);
  // for(_bitvector_type::bit_count_t bit_width = 1; bit_width <= max_bit_width; ++bit_width)
}

int main(int argc, char **argv) {
  Benchmark::initialize(argc, argv);

  runWithEveryBitwidth<bcv8_t>();
  runWithEveryBitwidth<bcv16_t>();
  runWithEveryBitwidth<bcv32_t>();
  runWithEveryBitwidth<bcv64_t>();
  
}