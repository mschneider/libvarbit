#ifndef VARBIT_REFERENCE_H_
#define VARBIT_REFERENCE_H_
#include <varbit/types.h>

namespace varbit {
template <typename T>
class reference {
 public:
  typedef T block_type;
  reference(block_type* block, const block_type shifted_bitmask,
            const bit_size_type offset_in_block)
      : block_(block),
        shifted_bitmask_(shifted_bitmask),
        offset_in_block_(offset_in_block) {}

  operator block_type() const {
    return (*block_ & shifted_bitmask_) >> offset_in_block_;
  }

  reference& operator=(const block_type new_value) {
    const block_type shifted_value = new_value << offset_in_block_;
    *block_ = *block_ ^ ((*block_ ^ shifted_value) & shifted_bitmask_);
    return *this;
  }

 private:
  block_type* block_;
  const block_type shifted_bitmask_;
  const bit_size_type offset_in_block_;
};
}
#endif  // VARBIT_REFERENCE_H_
