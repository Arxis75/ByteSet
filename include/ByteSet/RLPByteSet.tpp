#pragma once
#include <ByteSet/ByteSet.h>

template <BitsPerElem BitsPerElement>
ByteSet<BitsPerElement> RLPByteSet<BitsPerElement>::buildRLPSizeHeader(const ByteSet<BitsPerElement> &b) {
    return ByteSet<BitsPerElement>(b.byteSize(), ByteSet<BitsPerElement>(b.byteSize()).byteSize() * b.getNbElemPerByte());  // the size needs to be byte-aligned
}

template <BitsPerElem BitsPerElement>
ByteSet<BitsPerElement> RLPByteSet<BitsPerElement>::RLPSerialize(const ByteSet<BitsPerElement> &b, bool as_list)
{
    ByteSet<BitsPerElement> result(b);
    while(result.getNbElements() % (result.getNbElemPerByte()))
        //May need to be byte-aligned for RLP
        // => adds front 0-padding if necessary
        result.push_front_elem(0);

    if( as_list ||
        result.byteSize() != 1 ||
        (result.byteSize() == 1 && (result.asInteger() > 0x7F))) {
        ByteSet<BitsPerElement> header(result.byteSize() < 56 ? 0x80 + 0x40*as_list + result.byteSize() : 0xB7 + 0x40*as_list + buildRLPSizeHeader(result).byteSize());
        if(result.byteSize() >= 56)
            header.push_back(RLPByteSet::buildRLPSizeHeader(result));
        result.ByteSet<BitsPerElement>::push_front(header);
    }
    return result;
}

template <BitsPerElem BitsPerElement>
RLPByteSet<BitsPerElement> RLPByteSet<BitsPerElement>::pop_rlp(bool remove_brackets)
{
    assert(this->byteSize());
    
    uint8_t header = (this->isByteAligned() ? this->getElem(0) : uint8_t(this->at(0, 1*this->getNbElemPerByte()).asInteger()));
    bool has_header = (header >= 0x80);
    bool is_list = (header >= 0xC0);
    uint8_t list_modifier = 0x40 * is_list;
    bool is_long = (header > 0xB7 + list_modifier);
    uint64_t size_size = is_long ? header - 0xB7 - list_modifier : 0;
    uint64_t size = is_long ? uint64_t(this->at(1*this->getNbElemPerByte(), size_size*this->getNbElemPerByte()).asInteger()) : (has_header ? header - 0x80 - list_modifier : 1);

    uint64_t total_header_size = header > 0x7F ? 1 + size_size : 0;
    assert(this->getNbElements() >= (total_header_size + size) * this->getNbElemPerByte());
    
    if(remove_brackets) {
        this->pop_front(total_header_size*this->getNbElemPerByte());
        return *this;
    }
    else
        return this->pop_front((total_header_size + size) * this->getNbElemPerByte());
}

template <BitsPerElem BitsPerElement>
void RLPByteSet<BitsPerElement>::push_back_rlp(const RLPByteSet& rlp) { 
    if(hasRLPListHeader())
        pop_brackets();
    this->push_back(rlp);
    push_brackets();
}