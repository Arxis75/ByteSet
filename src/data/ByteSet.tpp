#include <data/ByteSet.h>
#include <data/Tools.h>

template <uint8_t BitsPerElement>
ByteSet<BitsPerElement>::ByteSet(const unsigned char *p, uint64_t source_nb_bytes)
{ 
    assert(isByteAligned());

    for(uint64_t i=0;i<source_nb_bytes;i++)
        push_back_elem(p[i]);
}
   
template <uint8_t BitsPerElement>
ByteSet<BitsPerElement>::ByteSet(const Integer &val, uint64_t nb_elem)
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

template <uint8_t BitsPerElement>
ByteSet<BitsPerElement>::ByteSet(const char *str, const ByteSetFormat &f, uint64_t target_nb_elem)
{
    string s = f.toCanonicalString(str);
    if(s.size()) {
        if(f.getBase() == 10)
            push_back(ByteSet(Integer(s.c_str()), target_nb_elem));
        else if(f.isCharAligned()) {
            int elem_mask = pow(2, getBitsPerElem()) - 1;
            uint64_t nb_elem = (target_nb_elem ? min(target_nb_elem, getStrNbElem(s, f, true)) : getStrNbElem(s, f, true));
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

template <uint8_t BitsPerElement>
Integer ByteSet<BitsPerElement>::asInteger() const
{
    Integer ret_value = (getNbElements() ? 0 : -1);     //-1 is the conventional value for an empty ByteSet
    for(uint64_t i=0;i<getNbElements();i++)
        ret_value += (Integer(getElem(getNbElements()-1-i)) << (i*getBitsPerElem()));
    return ret_value;     
}

template <uint8_t BitsPerElement>
string ByteSet<BitsPerElement>::asString(const ByteSetFormat &f, bool with_header, bool upper_case) const
{
    string str_result;
    if(f.getBase() == 10) {
        Integer val = asInteger();
        if(val >=0) {  //Do not display -1 when the ByteSet is empty
            stringstream ss;
            ss << dec << val;
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
                    if(f.getElemsPerChar(BitsPerElement))
                        elem--;
                    ne++;
                } while(elem >= 0 && ne < f.getElemsPerChar(BitsPerElement));
                str_result.insert(0, 1, f.digitToChar(c, upper_case));
                nc++;                       
            } while(elem >= 0 && nc < f.getCharsPerElem(BitsPerElement));
            if(f.getCharsPerElem(BitsPerElement) > 1)
                elem--;
        }
    }
    return f.toUserString(str_result, with_header);
}

template <uint8_t BitsPerElement>
uint64_t ByteSet<BitsPerElement>::getStrNbElem(const string &str, const ByteSetFormat &f, bool is_already_Canonical) const
{   
    uint64_t nb_elem = 0;
    string s = (is_already_Canonical ? str : f.toCanonicalString(str));
    if(f.getBase() == 10) {
        ByteSet b(Integer(s.c_str()));
        nb_elem = b.getNbElements();
    }
    else if(f.isCharAligned())
        nb_elem = ceil(float(s.size()*f.getBitsPerChar())/getBitsPerElem());
    return nb_elem; 
}

template <uint8_t BitsPerElement>
ByteSet<BitsPerElement> ByteSet<BitsPerElement>::keccak256() const
{
    ByteSet<BitsPerElement> result;
    if(BitsPerElement == 8)
        result = ByteSet(ethash::keccak256((unsigned char*)this, byteSize()).bytes, 32);
    else {
        assert(!bitSize()%8);
        ByteSet<8> this_8(asInteger(), byteSize());
        ByteSet<8> digest_8(ethash::keccak256(this_8, this_8.byteSize()).bytes, 32);
        result = ByteSet<BitsPerElement>(digest_8.asInteger(), 256/BitsPerElement);
    }
    return result;
}

template <uint8_t BitsPerElement>
ByteSet<BitsPerElement> ByteSet<BitsPerElement>::sha256() const
{
    ByteSet<BitsPerElement> result(0, 32);
    if(BitsPerElement == 8)
        SHA256((unsigned char*)this, byteSize(), result);   //result needs to be already initialized at 32 Bytes
    else {
        assert(!bitSize()%8);
        ByteSet<8> this_8(asInteger(), byteSize());
        ByteSet<8> digest_8(0, 32);
        SHA256((unsigned char*)this_8, this_8.byteSize(), digest_8);
        result = ByteSet<BitsPerElement>(digest_8.asInteger(), 256/BitsPerElement);
    }
    return result;
}

template <uint8_t bb>
ostream& operator<<(ostream& out, const ByteSet<bb>& b)
{
    //Dumps the vvalue vector as a raw Hex multi-bytes (or multi-bits) string
    //For more visual options, use asString()...
    bool needs_padding = (b.isByteAligned());   //FIXME: not quite true, but ok for byte/bit
    out << "0x ";
    for(int i : b.vvalue)
        out << hex << uppercase << (needs_padding && i < 0x10 ? "0" : "") << i << " ";
    return out;
}