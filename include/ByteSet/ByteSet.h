#pragma once
#include <ByteSet/ByteSetFormat.h>

enum RLPType {LIST, INT, BYTE, STR};

template <uint8_t BitsPerElement = 8>   //Should work for 1, 2, 4, 8 only
class ByteSet
{
    public: 
        ByteSet() = default;
        ~ByteSet() = default;

        //****************************** Array Constructors/Operators ****************************************//

        ByteSet(const unsigned char *p, uint64_t source_nb_bytes);
        
        /// @brief Should only be used when calling third-party cryptographic functions (OpenSSL, etc...),
        /// thus f.getBitsPerElem() = 8 is required.
        inline operator const unsigned char*() const { assert(isByteAligned()); return reinterpret_cast<const unsigned char*>(vvalue.data()); } 
        inline operator unsigned char*() { assert(isByteAligned()); return reinterpret_cast<unsigned char*>(vvalue.data()); } 

        //******************************* Int Constructors/Operators ***************************************//

        inline explicit ByteSet(int32_t val, uint64_t target_nb_elem = 0) : ByteSet(Integer(val), target_nb_elem) {}
        inline explicit ByteSet(uint32_t val, uint64_t target_nb_elem = 0) : ByteSet(Integer(val), target_nb_elem) {}
        inline explicit ByteSet(int64_t val, uint64_t target_nb_elem = 0) : ByteSet(Integer(val), target_nb_elem) {}
        inline explicit ByteSet(uint64_t val, uint64_t target_nb_elem = 0) : ByteSet(Integer(val), target_nb_elem) {}
        explicit ByteSet(const Integer &val, uint64_t target_nb_elem = 0);

        Integer asInteger() const;
        
        inline ByteSet& operator=(int32_t val) { ByteSet b(val); swap(vvalue, b.vvalue); return *this; }
        inline ByteSet& operator=(uint32_t val) { ByteSet b(val); swap(vvalue, b.vvalue); return *this; }
        inline ByteSet& operator=(int64_t val) { ByteSet b(val); swap(vvalue, b.vvalue); return *this; }
        inline ByteSet& operator=(uint64_t val) { ByteSet b(val); swap(vvalue, b.vvalue); return *this; }
        inline ByteSet& operator=(const Integer &val) { ByteSet b(val); swap(vvalue, b.vvalue); return *this; }

        //******************************* String Constructors/Operators *************************************//
        
        inline explicit ByteSet(const string &str, const ByteSetFormat &f = Hex, uint64_t target_nb_elem = 0) : ByteSet(str.c_str(), f, target_nb_elem) {}
        explicit ByteSet(const char *str, const ByteSetFormat &f = Hex, uint64_t target_nb_elem = 0);

        string asString(const ByteSetFormat &f = Hex, bool with_header = true, bool upper_case = true) const;

        inline ByteSet& operator=(const string &str) { ByteSet b(str); swap(vvalue, b.vvalue); return *this; }
        inline ByteSet& operator=(const char *str) { ByteSet b(str); swap(vvalue, b.vvalue); return *this; }

        //ostream
        template <uint8_t BPE>
            friend std::ostream& operator<<(std::ostream& out, const ByteSet<BPE>& b);

        //****************************************** PUSH/POP METHODS **************************************************//
        
        inline void push_front_elem(uint8_t elem) { vvalue.insert(vvalue.begin(), elem); }
        inline void push_back_elem(uint8_t elem) { vvalue.push_back(elem); }
        uint8_t pop_front_elem();
        uint8_t pop_back_elem();

        inline void push_front(const ByteSet& subset) { vvalue.insert(vvalue.begin(), subset.vvalue.begin(), subset.vvalue.end()); }
        inline void push_back(const ByteSet& subset) { vvalue.insert(vvalue.end(), subset.vvalue.begin(), subset.vvalue.end()); }
        ByteSet pop_front(uint64_t nb_element);
        ByteSet pop_back(uint64_t nb_element);

        //************************************** Accessors ****************************************//

        inline bool operator==(const ByteSet &b) const { return vvalue == b.vvalue; }

        /// @brief 

        /// @brief Access in read mode an element of the ByteSet (Might be const_cast<unsigned char &> for write mode access)
        /// @param elem_index 
        /// @return the reference of the element
        inline const unsigned char &operator[](uint64_t elem_index) const { assert(elem_index < getNbElements()); return vvalue[elem_index]; }
        /// @brief extract a sub-ByteSet
        /// @param elem_offset offset in elements of the beginning of the sub-ByteSet
        /// @param nb_element number of elements of the sub-ByteSet
        /// @return The sub-ByteSet
        ByteSet at(const uint64_t elem_offset, const uint64_t nb_element) const;

        inline uint8_t getElem(uint64_t elem_index) const { return vvalue[elem_index]; }
        inline uint64_t getIntNbElem(Integer val) const { return ceil(logtwo(1 + val) / getBitsPerElem()); }
        uint64_t getStrNbElem(const string &str, const ByteSetFormat &f = Hex, bool is_already_Canonical = false) const;
        inline uint8_t getIntElem(const Integer &val, uint64_t elem_offset) const { return uint8_t((Givaro::pow(2, getBitsPerElem()) - 1) & (val >> (getIntNbElem(val) - 1 - elem_offset) * getBitsPerElem())); }

        inline uint8_t getBitsPerElem() const { return BitsPerElement; }
        inline uint8_t getNbElemPerByte() const { return 8/BitsPerElement; }
        inline bool isByteAligned() const { return !(getBitsPerElem()%8);}

        inline bool isEmpty() const { return vvalue.size() == 0; }

        /// @brief Gets the size in bits of the stored value.
        /// @return The logical size in  bits of the container, not the vector size.
        inline uint64_t bitSize() const { return getNbElements() * getBitsPerElem(); }
        /// @brief Gets the size in bytes of the stored value (rounded to the upper byte if isAligned = false).
        /// @return The logical size in bytes of the container, not the vector size.
        inline uint64_t byteSize() const { return (bitSize() >> 3) + (bitSize()%8 ? 1 : 0); }
        /// @brief Gets the number of th ByteSet
        /// @return the size of the underlying vector
        inline uint64_t getNbElements() const { return vvalue.size(); }

        //********************************************* RLP  Helpers *************************************************/

        RLPType getRLPType() const { return m_rlp_type; }
        void setRLPType(RLPType t) { if(t == RLPType::INT) removefront0padding(); m_rlp_type = t; }
        inline void removefront0padding() { while(getNbElements() && !getElem(0)) pop_front_elem(); }
        bool hasRLPListHeader() const { return getNbElements() && at(0, 8/getBitsPerElem()).asInteger() >= 0xC0; }
        ByteSet buildRLPSizeHeader() const { return ByteSet(byteSize(), ByteSet(byteSize()).byteSize() * getNbElemPerByte()); }   // the size needs to be byte-aligned

        //********************************** Container manipulation interface ***************************************//

        inline void clear() { vvalue.clear(); }

        ByteSet RLPserialize(bool as_list) const;
        ByteSet RLPparse();
        ByteSet keccak256() const;
        ByteSet sha256() const;

    private:
        /// @brief Vector is used to store the data with the Big-Endian convention
        std::vector<uint8_t> vvalue;
        RLPType m_rlp_type;
};

#include <ByteSet/ByteSet.tpp>