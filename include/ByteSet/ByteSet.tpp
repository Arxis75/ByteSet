#pragma once
#include <ByteSet/ByteSet.h>
#include <ByteSet/Tools.h>
#include <cstdint>

template <BitsPerElem BitsPerElement>
ByteSet<BitsPerElement>::ByteSet(const unsigned char *p, uint64_t source_nb_bytes)
    : IComponent()
{ 
    assert(isByteAligned());

    for(uint64_t i=0;i<source_nb_bytes;i++)
        push_back_elem(p[i]);
}
   
template <BitsPerElem BitsPerElement>
ByteSet<BitsPerElement>::ByteSet(const Integer &val, uint64_t nb_elem)
    : IComponent()
{
    if(val >= 0)   //-1 is the conventional value for an empty ByteSet
    {
        uint64_t nb_elem_for_val = getIntNbElem(val);
        uint64_t i_extra = (nb_elem > nb_elem_for_val ? nb_elem - nb_elem_for_val : 0);
        for(uint64_t i=0;i<i_extra;i++)
            push_back_elem(0x00);
        uint64_t i_start = (nb_elem ? (nb_elem < nb_elem_for_val ? nb_elem_for_val - nb_elem : 0) : 0);
        for(uint64_t i=i_start;i<nb_elem_for_val;i++)
            push_back_elem(getIntElem(val, i));
    }
}

template <BitsPerElem BitsPerElement>
ByteSet<BitsPerElement>::ByteSet(const char *str, const ByteSetFormat &f, uint64_t target_nb_elem)
    : IComponent()
{
    std::string s = f.toCanonicalString(str);
    if(s.size()) {
        if(f.getBase() == 10)
            push_back(ByteSet(Integer(s.c_str()), target_nb_elem));
        else if(f.isCharAligned()) {
            int elem_mask = pow(2, getBitsPerElem()) - 1;
            uint64_t nb_elem = (target_nb_elem ? std::min(target_nb_elem, getStrNbElem(s, f, true)) : getStrNbElem(s, f, true));
            for(uint64_t i = 0; i < nb_elem; i++) {
                uint8_t elem_value = 0;
                if( f.getBitsPerChar() > getBitsPerElem()) {
                    // 1 char spreads over several elements
                    int64_t index = s.size() - (i * getBitsPerElem() / f.getBitsPerChar()) - 1;
                    elem_value = (elem_mask & (f.charToDigit(s[index]) >> ((i*getBitsPerElem())%f.getBitsPerChar())));
                }
                else {
                    // 1 element hosts 1+ chars
                    for(uint64_t j = 0; j < f.getCharsPerElem(getBitsPerElem()); j++) {
                        //Parsing the string from right to left (LSB to MSB)
                        int64_t index = s.size() - i * f.getCharsPerElem(getBitsPerElem()) - j - 1;
                        if(index >= 0)  //if index drops below 0 (no more char left to parse to fill element), 0-padding would be inserted
                            elem_value += ((f.charToDigit(s[index])) << (f.getBitsPerChar()*j));
                        else
                            break;
                    }
                }
                push_front_elem(elem_value );
            }
            while( getNbElements() < target_nb_elem)
                push_front_elem(0x00);                      // 0-padding inserted
        }
    }
}

template <BitsPerElem BitsPerElement>
template <BitsPerElem T>
ByteSet<T> ByteSet<BitsPerElement>::as() const {
    if constexpr(T == BitsPerElement)
        return *this;
    else {
        ByteSet<T> result;
        if(uint(T) > getBitsPerElem()) {
            uint nb_origin_elem_for_one_target_elem = uint(T) / getBitsPerElem();
            uint8_t target_elem = 0;
            for(uint i = 0; i<getNbElements(); i++) {
                target_elem += (getElem(getNbElements()-i-1) << ((i%nb_origin_elem_for_one_target_elem) * getBitsPerElem()));
                if((i%nb_origin_elem_for_one_target_elem) == nb_origin_elem_for_one_target_elem - 1 || i == getNbElements() - 1) {
                    result.push_front_elem(target_elem);
                    target_elem = 0;    
                }
            }
        }
        else {
            uint nb_target_elem_for_one_origin_elem = getBitsPerElem() / uint(T);
            uint8_t target_elem = 0;
            for(uint i = 0; i<getNbElements(); i++) {
                target_elem = getElem(i);
                uint mask = pow(2, uint(T))-1;
                for(uint j = 0; j<nb_target_elem_for_one_origin_elem; j++) {
                    result.push_back_elem(mask & (target_elem >> (uint(T)*(nb_target_elem_for_one_origin_elem-j-1))));
                }
            }  
        }
        return result;
    }
}

template <BitsPerElem BitsPerElement>
Integer ByteSet<BitsPerElement>::asInteger() const
{
    Integer ret_value = (getNbElements() ? 0 : -1);     //-1 is the conventional value for an empty ByteSet
    for(uint64_t i=0;i<getNbElements();i++)
        ret_value += (Integer(getElem(getNbElements()-1-i)) << (i*getBitsPerElem()));
    return ret_value;     
}

template <BitsPerElem BitsPerElement>
std::string ByteSet<BitsPerElement>::asString(const ByteSetFormat &f, bool with_header, bool upper_case) const
{
    std::string str_result;
    if(f.getBase() == 10) {
        Integer val = asInteger();
        if(val >=0) {  //Do not display -1 when the ByteSet is empty
            std::stringstream ss;
            ss << std::dec << val;
            str_result = ss.str();
        }
    }
    else if(f.isCharAligned()) {
        uint8_t char_mask = f.getBase() - 1;
        int64_t elem = getNbElements() - 1;
        while(elem >= 0) {
            int nc = 0;     //Char counter
            do {
                char c = 0; //Char being parsed
                int ne = 0; //Element counter
                do {
                    c += ((vvalue[elem] >> (nc*f.getBitsPerChar())) & char_mask) << ne*getBitsPerElem();
                    if(f.getElemsPerChar(getBitsPerElem()))
                        elem--;
                    ne++;
                } while(elem >= 0 && ne < f.getElemsPerChar(getBitsPerElem()));
                str_result.insert(0, 1, f.digitToChar(c, upper_case));
                nc++;                       
            } while(elem >= 0 && nc < f.getCharsPerElem(getBitsPerElem()));
            if(f.getCharsPerElem(getBitsPerElem()) > 1)
                elem--;
        }
    }
    return f.toUserString(str_result, with_header);
}

template <BitsPerElem BitsPerElement>
uint64_t ByteSet<BitsPerElement>::getStrNbElem(const std::string &str, const ByteSetFormat &f, bool is_already_Canonical) const
{   
    uint64_t nb_elem = 0;
    std::string s = (is_already_Canonical ? str : f.toCanonicalString(str));
    if(f.getBase() == 10) {
        ByteSet b(Integer(s.c_str()));
        nb_elem = b.getNbElements();
    }
    else if(f.isCharAligned())
        nb_elem = ceil(float(s.size()*f.getBitsPerChar())/getBitsPerElem());
    return nb_elem; 
}

template <BitsPerElem BitsPerElement>
uint8_t ByteSet<BitsPerElement>::pop_front_elem()
{
    assert(getNbElements());
    uint8_t ret_value = vvalue[0];
    vvalue.erase(vvalue.begin());
    return ret_value;
}

template <BitsPerElem BitsPerElement>
uint8_t ByteSet<BitsPerElement>::pop_back_elem()
{
    assert(getNbElements());
    uint8_t ret_val = vvalue[getNbElements()-1];
    vvalue.pop_back();
    return ret_val;
}

template <BitsPerElem BitsPerElement>
ByteSet<BitsPerElement> ByteSet<BitsPerElement>::pop_front(uint64_t nb_element) {
    ByteSet ret_value;
    if(nb_element <= getNbElements())
        for(uint64_t i=0;i<nb_element;i++)
            ret_value.push_back_elem(pop_front_elem());
    return ret_value;
}

template <BitsPerElem BitsPerElement>
ByteSet<BitsPerElement> ByteSet<BitsPerElement>::pop_back(uint64_t nb_element) {
    ByteSet ret_value;
    if(nb_element <= getNbElements())
        for(uint64_t i=0;i<nb_element;i++)
            ret_value.push_front_elem(pop_back_elem());
    return ret_value;
}

template <BitsPerElem BitsPerElement>
ByteSet<BitsPerElement> ByteSet<BitsPerElement>::at(const uint64_t elem_offset, const uint64_t nb_element) const
{
    ByteSet ret_value(*this);
    ret_value.pop_front(elem_offset);
    ret_value.pop_back(getNbElements() - (elem_offset + nb_element));
    return ret_value;
}

template <BitsPerElem BitsPerElement>
const RLPByteSet<BYTE> ByteSet<BitsPerElement>::serialize() const {
    return RLPByteSet<BYTE>(as<BYTE>()); 
}

template <BitsPerElem BitsPerElement>
void ByteSet<BitsPerElement>::parse(RLPByteSet<BYTE> &b) {
    b.pop_brackets();
    *this = b.as<BitsPerElement>();
}

template <BitsPerElem BitsPerElement>
ByteSet<BitsPerElement> ByteSet<BitsPerElement>::keccak256() const
{
    ByteSet<BitsPerElement> result;
    if(BitsPerElement == 8)
        result = ByteSet(ethash::keccak256(*this, byteSize()).bytes, 32);
    else {
        ByteSet<BYTE> this_8(asInteger(), byteSize());
        ByteSet<BYTE> digest_8(ethash::keccak256(this_8, this_8.byteSize()).bytes, 32);
        result = ByteSet<BitsPerElement>(digest_8.asInteger(), 256/getBitsPerElem());
    }
    return result;
}

template <BitsPerElem BitsPerElement>
ByteSet<BitsPerElement> ByteSet<BitsPerElement>::sha256() const
{
    ByteSet<BitsPerElement> result(0, 32);
    if(BitsPerElement == 8)
        SHA256(*this, byteSize(), result);              //result needs to be already initialized at 32 Bytes
    else {
        ByteSet<BYTE> this_8(asInteger(), byteSize());
        ByteSet<BYTE> digest_8(0, 32);
        SHA256(this_8, this_8.byteSize(), digest_8);
        result = ByteSet<BitsPerElement>(digest_8.asInteger(), 256/BitsPerElement);
    }
    return result;
}

template <BitsPerElem BitsPerElement>
const ByteSet<BitsPerElement> ByteSet<BitsPerElement>::withTerminator() const {
    ByteSet result(*this);
    if(!result.getNbElements() || result.getElem(result.getNbElements()-1) != 0x10)
        result.push_back_elem(0x10);
    return result;
}

template <BitsPerElem BitsPerElement>
const ByteSet<BitsPerElement> ByteSet<BitsPerElement>::withoutTerminator() const {
    ByteSet result(*this);
    if(result.getNbElements() && result.getElem(result.getNbElements()-1) == 0x10)
        result.pop_back_elem();
    return result;
}

template <BitsPerElem BitsPerElement>
ByteSet<BYTE> ByteSet<BitsPerElement>::HexToCompact() const
{
    assert(getBitsPerElem() == 4);

    ByteSet result(withoutTerminator());
    if(result.getNbElements()%2)
        result.push_front_elem(0x1+2*hasTerminator());
    else {
        result.push_front_elem(0x0);
        result.push_front_elem(2*hasTerminator());
    }
    return result.as<BYTE>();
}

template <BitsPerElem bb>
std::ostream& operator<<(std::ostream& out, const ByteSet<bb>& b)
{
    //Dumps the vvalue vector as a raw Hex multi-bytes (or multi-bits) string
    //For more visual options, use asString()...
    bool needs_padding = (b.isByteAligned());   //FIXME: not quite true, but ok for byte/bit
    out << "0x ";
    for(int i : b.vvalue)
        out << std::hex << std::uppercase << (needs_padding && i < 0x10 ? "0" : "") << i << " ";
    return out;
}