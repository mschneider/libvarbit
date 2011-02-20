#ifndef VARBIT_ITERATOR_H_
#define VARBIT_ITERATOR_H_
#include <varbit/types.h>
#include <varbit/reference.h>

namespace varbit {
template <typename T>
class iterator : public base_iterator<T> {
 public:
  typedef T                         block_type;
  typedef block_type                value_type;
  typedef base_iterator<block_type> parent_type;
  typedef reference<block_type>     reference_type;
  typedef iterator<block_type>      iterator_type;

  iterator(
      block_type* block_pointer,
      const block_type bitmask,
      const bit_size_type segment_width,
      const segment_count_type segments_per_block,
      const segment_count_type segment_index = 0)
      : parent_type(block_pointer, bitmask, segment_width, segments_per_block,
            segment_index) { }

  iterator_type& operator++() {
    ++segment_index_;
    if (segment_index_ == segments_per_block_) {
      segment_index_ = 0;
      ++block_pointer_;
    }
    return *this;
  }

  iterator_type operator++(int) { //NOLINT: unused
     iterator_type temp = *this;
     this->operator++();
     return temp;
  }

  value_type operator*() const {
    return *block_pointer_ & (bitmask_ << OffsetInBlock());
  }

  reference_type operator*() {
    return reference_type(block_pointer_, this->bitmask_ << OffsetInBlock(),
        OffsetInBlock());
  }

  using parent_type::OffsetInBlock;
  using parent_type::block_pointer_;
  using parent_type::bitmask_;
  using parent_type::segment_width_;
  using parent_type::segments_per_block_;
  using parent_type::segment_index_;
};
};
#endif  // VARBIT_ITERATOR_H_
