#ifndef VARBIT_VECTOR_H_
#define VARBIT_VECTOR_H_
#include <varbit/types.h>
#include <varbit/reference.h>
#include <varbit/iterator.h>
#include <assert.h>
#include <stdint.h>
#include <vector>

namespace varbit {
template <typename T>
class vector {
 public:
  // capabale of addressing all elements
  typedef uint64_t                   size_type;
  typedef T                          block_type;
  typedef block_type                 value_type;
  typedef std::vector<block_type>    vector_type;
  typedef reference<block_type>      value_reference_type;
  typedef const_iterator<block_type> const_iterator;

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
  }

  value_reference_type operator[](const size_type n) {
    block_type &block = Block(n);
    const bit_size_type offset_in_block = OffsetInBlock(n);
    return value_reference_type(&block, bitmask_ << offset_in_block,
                                offset_in_block);
  }

  const_iterator begin() const {
    return const_iterator(&blocks_[0], bitmask_, segment_width_,
                                                 segments_per_block_);
  }

  const_iterator end() const {
    return const_iterator(
        &blocks_[BlockIndex(size())],
        bitmask_ << OffsetInBlock(size()),
        segment_width_,
        segments_per_block_,
        SegmentIndex(size()));
  }

  size_type capacity() const { return capacity_; }
  size_type size() const { return size_; }
  static bit_size_type max_bit_width() { return sizeof(block_type) * 8; }

 private:
  typename vector_type::size_type BlockIndex(const size_type n) const {
    return n / segments_per_block_;
  }
  segment_count_type SegmentIndex(const size_type n) const {
    return n % segments_per_block_;
  }
  // returns the block containing the nth element
  block_type& Block(const size_type n) {
    return blocks_[BlockIndex(n)];
  }  // double definition for const access
  const block_type& Block(const size_type n) const {
    return blocks_[BlockIndex(n)];
  }
  // returns the offset of the segment containing the nth element
  bit_size_type OffsetInBlock(const size_type n) const {
    return SegmentIndex(n) * segment_width_;
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
