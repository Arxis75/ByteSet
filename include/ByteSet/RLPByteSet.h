#pragma once
/*#include <ByteSet/VectorNode.h>
#include <ByteSet/ByteSet.h>

template <BitsPerElem BitsPerElement = BYTE>
class RLPByteSet : protected ByteSet<BitsPerElement>
{
    public:
        inline static const RLPByteSet EMPTY = ByteSet().RLPSerialize(false);

        RLPByteSet() = default;
        
        inline explicit RLPByteSet(const std::string &str, const ByteSetFormat &f = Hex, uint64_t target_nb_elem = 0) : RLPByteSet(str.c_str(), f, target_nb_elem) {}
        explicit RLPByteSet(const char *str, const ByteSetFormat &f = Hex, uint64_t target_nb_elem = 0) : ByteSet<BitsPerElement>(str, f, target_nb_elem) {}
    
        removeBrackets() { pop_front_rlp(true); }
};*/