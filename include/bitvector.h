#ifndef LIBVARBIT_BITVECTOR_H_
#define LIBVARBIT_BITVECTOR_H_

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <stdint.h>
#include <sys/time.h>
#include <sys/mman.h>

template<typename _block_type>    
class block_caching_iterator;

template<typename _block_type>
class bitvector {
    friend class block_caching_iterator<_block_type>;
public:
    typedef uint64_t                        size_t; // type capabale of addressing all elements
    typedef _block_type                     block_t; 
    typedef block_t                         value_t; // sizeof(value_t) should be <= sizeof(block_t)
    typedef std::vector<block_t>            vector_t;
    typedef typename vector_t::size_type    block_count_t;
    typedef unsigned int                    bit_count_t;
    typedef unsigned int                    block_segement_count_t;
    typedef block_caching_iterator<block_t> iterator;
        
protected:    
    const bit_count_t            segment_width;
    const block_segement_count_t segments_per_block;
    const block_t                bitmask;
    vector_t                     blocks;
    size_t                       reserved_size;
    size_t                       _size;
    
public:

    bitvector(
        bit_count_t segment_width,
        size_t      reserved_size = 0)
    :
        segment_width(segment_width),
        segments_per_block(sizeof(block_t) * 8 / segment_width),
        bitmask(~(~0 << segment_width)),
        reserved_size(reserved_size),
        _size(0)
    {
        assert(segment_width <= sizeof(block_t) * 8);
        const size_t reserved_blocks = (reserved_size / segments_per_block) + (reserved_size % segments_per_block > 0);
        blocks = vector_t(reserved_blocks, 0);
        assert(reserved_blocks < blocks.max_size());
        
    };
    
    void push_back(const value_t value) {
        if(size() == reserved_size) {
            blocks.push_back(0);
            reserved_size += segments_per_block;
        }
        block_t& destination = get_block(size());
        const bit_count_t offset_in_block = this->offset_in_block(size());
        // assumes that block is zero-initialized b/c this is push_back, not set
        destination |= ((value & bitmask) << offset_in_block);
        ++_size;
    };
    
    // has to be const - setting values via [] would fuck up the memory
    const value_t operator[](const size_t n) const {
        const block_t& block = get_block(n);
        const bit_count_t offset_in_block = this->offset_in_block(n);
        return ((block >> offset_in_block) & bitmask);
    };
    
    void set(const size_t n, const value_t value) {
        block_t &block = get_block(n);
        const bit_count_t offset_in_block = this->offset_in_block(n);
        // TODO: Test performance impact if caching bitmask
        const block_t shifted_bitmask = bitmask << offset_in_block;
        const block_t shifted_segment = value << offset_in_block;
        // bitmerge using bitmask - idiom
        block = block ^ ((block ^ shifted_segment) & shifted_bitmask);
    };
    
    inline       block_t& get_block(const size_t n)       {return blocks[n / segments_per_block];};
    inline const block_t& get_block(const size_t n) const {return blocks[n / segments_per_block];};
    // number of bits from the right    
    inline const bit_count_t offset_in_block(const size_t n) const {return (n % segments_per_block) * segment_width;};
    inline const size_t size() const {return _size;};
    
    void inspect(bool print_values = false) {
        std::cout << std::endl;
        std::cout << "segment width:      " << segment_width << " bits" << std::endl;
        std::cout << "block size:         " << sizeof(block_t) * 8 << " bits" << std::endl;
        std::cout << "segments per block: " << segments_per_block << std::endl << std::endl;

        std::cout << "size:               " << size() << " stored values" << std::endl;
        std::cout << "reserved size:      " << reserved_size << " possible values" << std::endl;
        std::cout << "reserved blocks:    " << blocks.capacity() << std::endl;
        std::cout << "memory usage:       " << blocks.capacity() * sizeof(block_t) << " bytes" << std::endl;

        if(print_values) {
            std::cout << std::endl << "values:" << std::endl;
            for(size_t i = 0; i < size(); ++i) {
                std::cout << operator[](i) << "\t";
            }
        }
        std::cout << std::endl;
    };
};

template<typename _block_type>
class block_caching_iterator {    
public:
    typedef bitvector<_block_type>      bitvector_t;
    
protected:
    const bitvector_t&                           bit_vector; //gcc complains over similiar naming WTF
    typename bitvector_t::block_t                cached_block;
    typename bitvector_t::block_count_t          current_block;
    typename bitvector_t::block_segement_count_t current_segment;
    
public:
    block_caching_iterator(const bitvector_t& bitvector, typename bitvector_t::size_t position = 0)
    :
        bit_vector(bitvector),
        current_block(position / bit_vector.segments_per_block),
        current_segment(position % bit_vector.segments_per_block)
    {
        fetch_current_block();
    };
    
    inline void fetch_current_block() {
        // the end - index aka. bit_vector.blocks[bit_vector.size()] should never be dereferenced it would produce a segfault.
        // gcc -O3 is intelligent enough to never do.. -O0 not
        // therefore the cached_block is set to 0 after hitting the end.
        // guarding inside the vector and outside is an overhead which could be avoided via java-style iterators.
        if(current_block * bit_vector.segments_per_block < bit_vector.size())
            cached_block = bit_vector.blocks[current_block];
        else
            cached_block = 0;
    }
    
    inline void operator++() {
        if(++current_segment == bit_vector.segments_per_block) {
            ++current_block;

            fetch_current_block();
            current_segment = 0;
        } else {
            cached_block >>= bit_vector.segment_width;
        }
    };
    
    inline const typename bitvector_t::value_t operator*() const {
        return cached_block & bit_vector.bitmask;
    }; 
    
    inline bool operator!=(const typename bitvector_t::iterator& other) const {
        return (this->current_block != other.current_block) || (this->current_segment != other.current_segment);
    };
};

#endif // LIBVARBIT_BITVECTOR_H_