#ifndef VARBIT_BASE_ITERATOR_H_
#define VARBIT_BASE_ITERATOR_H_
#include <varbit/types.h>
#include <varbit/reference.h>

namespace varbit {
template <typename T>
class base_iterator {
 public:
  typedef T                         block_type;
  typedef block_type                value_type;
  typedef base_iterator<block_type> base_iterator_type;

  base_iterator(
      block_type* block_pointer,
      const block_type bitmask,
      const bit_size_type segment_width,
      const segment_count_type segments_per_block,
      const segment_count_type segment_index)
      : block_pointer_(block_pointer),
        bitmask_(bitmask),
        segment_width_(segment_width),
        segments_per_block_(segments_per_block),
        segment_index_(segment_index) { }

  bool operator!=(const base_iterator_type& other) const {
    return block_pointer_ != other.block_pointer_ ||
           segment_index_ != other.segment_index_;
  }

  bool operator==(const base_iterator_type& other) const {
    return !this->operator!=(other);
  }

 protected:
  bit_size_type OffsetInBlock() const {
    return segment_index_ * segment_width_;
  }

  block_type* block_pointer_;
  block_type bitmask_;
  bit_size_type segment_width_;
  segment_count_type segments_per_block_;
  segment_count_type segment_index_;
};
};
#endif  // VARBIT_BASE_ITERATOR_H_
