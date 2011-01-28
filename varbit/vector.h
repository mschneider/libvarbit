#ifndef VARBIT_VECTOR_H_
#define VARBIT_VECTOR_H_
#include <varbit/reference.h>

#include <stdint.h>

#include <vector>
#include <cstdlib>
#include <cassert>

namespace varbit {
template <typename T>
class vector {
 public:
  // capabale of addressing all elements
  typedef uint64_t                        size_type;
  typedef T                               block_type;
  typedef std::vector<block_type>         vector_type;
  // capabale of addressing all blocks
  typedef typename vector_type::size_type block_size_type;
  typedef unsigned int                    bit_size_type;
  // capabale of addressing all segments in a block
  typedef bit_size_type                   segment_count_type;
  // sizeof(value_type) should be <= sizeof(block_type)
  typedef block_type                      value_type;

  vector(bit_size_type segment_width, size_type capacity = 0)
      : segment_width_(segment_width),
        segments_per_block_(sizeof(block_type) * 8 / segment_width_),
        bitmask_(~(~0 << segment_width_)),
        capacity_(capacity),
        size_(0) {
    assert(segment_width_ <= max_bit_width());
    const size_t reserved_blocks = (capacity_ / segments_per_block_) +
                                   (capacity_ % segments_per_block_ > 0);
    blocks_ = vector_type(reserved_blocks, 0);
  }

  void push_back(const value_type value) {
    if (size() == capacity()) {
        blocks_.push_back(0);
        capacity_ += segments_per_block_;
    }
    block_type& destination = Block(size());
    const bit_size_type offset_in_block = OffsetInBlock(size());
    // assumes that block is zero-initialized b/c this is push_back, not Set
    destination |= ((value & bitmask_) << offset_in_block);
    ++size_;
  }

  value_type operator[](const size_type n) const {
      const block_type &block = Block(n);
      const bit_size_type offset_in_block = OffsetInBlock(n);
      return ((block >> offset_in_block) & bitmask_);
  };

  reference<block_type> operator[](const size_type n) {
      block_type &block = Block(n);
      const bit_size_type offset_in_block = OffsetInBlock(n);
      return reference<block_type>(&block, bitmask_ << offset_in_block,
                                   offset_in_block);
  };

  const size_type capacity() const { return capacity_; }
  const size_type size() const { return size_; }
  static bit_size_type max_bit_width() { return sizeof(block_type) * 8; }

 private:
  // returns the block containing the nth element
  block_type& Block(const size_type n) {
    return blocks_[n / segments_per_block_];
  }  // double definition for const access
  const block_type& Block(const size_type n) const {
    return blocks_[n / segments_per_block_];
  }
  // returns the offset of the segment containing the nth element
  const bit_size_type OffsetInBlock(const size_type n) const {
    return (n % segments_per_block_) * segment_width_;
  }

  const bit_size_type      segment_width_;
  const segment_count_type segments_per_block_;
  const block_type         bitmask_;
  size_type                capacity_;
  size_type                size_;
  vector_type              blocks_;
};
};
#endif  // VARBIT_VECTOR_H_
