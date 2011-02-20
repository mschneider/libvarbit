#ifndef VARBIT_CONST_ITERATOR_H_
#define VARBIT_CONST_ITERATOR_H_
#include <varbit/types.h>
#include <varbit/reference.h>
#include <varbit/base_iterator.h>

namespace varbit {
template <typename T>
class const_iterator : public base_iterator<T> {
 public:
  typedef T                          block_type;
  typedef block_type                 value_type;
  typedef base_iterator<block_type>  parent_type;
  typedef const_iterator<block_type> const_iterator_type;

  const_iterator(
      const block_type* block_pointer,
      const block_type bitmask,
      const bit_size_type segment_width,
      const segment_count_type segments_per_block,
      const segment_count_type segment_index = 0)
      : parent_type(const_cast<block_type*>(block_pointer), bitmask,
            segment_width, segments_per_block, segment_index),
        block_copy_(*block_pointer) { }

  const_iterator_type& operator++() {
    ++segment_index_;
    if (segment_index_ == segments_per_block_) {
      segment_index_ = 0;
      ++block_pointer_;
      block_copy_ = *block_pointer_;
    } else {
      block_copy_ = block_copy_ >> segment_width_;
    }
    return *this;
  }

  const_iterator_type operator++(int) { //NOLINT: unused
     const_iterator_type temp = *this;
     this->operator++();
     return temp;
  }

  value_type operator*() const {
    return block_copy_ & bitmask_;
  }

 protected:
  using parent_type::OffsetInBlock;

  block_type block_copy_;
  using parent_type::block_pointer_;
  using parent_type::bitmask_;
  using parent_type::segment_width_;
  using parent_type::segments_per_block_;
  using parent_type::segment_index_;
};
};
#endif  // VARBIT_CONST_ITERATOR_H_
