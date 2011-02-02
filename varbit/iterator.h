#ifndef VARBIT_ITERATOR_H_
#define VARBIT_ITERATOR_H_
namespace varbit {
template <typename T>
class const_iterator {
 public:
  typedef T                          block_type;
  typedef block_type                 value_type;
  typedef const_iterator<block_type> const_iterator_type;

  const_iterator(
      const block_type* block_pointer,
      const block_type& bitmask,
      const bit_size_type& segment_width,
      const segment_count_type& segments_per_block,
      const segment_count_type segment_index = 0)
      : block_copy_(*block_pointer),
        block_pointer_(block_pointer),
        bitmask_(bitmask),
        segment_width_(segment_width),
        segments_per_block_(segments_per_block),
        segment_index_(segment_index) { }

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

  const_iterator_type& operator++(int) {
     const_iterator_type temp = *this;
     ++(*this);
     return temp;
  }

  value_type operator*() const {
    return block_copy_ & bitmask_;
  }

  bool operator!=(const const_iterator_type& other) {
    return block_pointer_ != other.block_pointer_ ||
           segment_index_ != other.segment_index_;
  }

  bool operator==(const const_iterator_type& other) {
    return !((*this) != other);
  }

 private:
  block_type block_copy_;
  const block_type* block_pointer_;
  const block_type bitmask_;
  const bit_size_type segment_width_;
  const segment_count_type segments_per_block_;
  segment_count_type segment_index_;
};
};
#endif  // VARBIT_ITERATOR_H_
