#pragma once

#include <data/System.h>

class ByteSetFormat
{
    public:
        ByteSetFormat( const string &UIName,
                       const string &Header,
                       uint16_t Base,
                       uint8_t BitsPerChar,
                       const string &Regex,
                       uint64_t BitSize,
                       uint8_t Exp );

        inline bool operator==(const ByteSetFormat& b) const { return m_ui_name == b.m_ui_name && m_header == b.m_header && m_base == b.m_base && m_bits_per_char == b.m_bits_per_char && m_regex == b.m_regex && m_bit_size == b.m_bit_size && m_exp == b.m_exp; }
        inline bool operator!=(const ByteSetFormat& b) const { return !(*this == b); }

        inline const string &getUIName() const { return m_ui_name; }
        inline const string &getHeader() const { return m_header; }
        inline uint16_t getBase() const { return m_base; }
        inline uint8_t getBitsPerChar() const { return m_bits_per_char; }
        /// @brief Function returning the number of chars fitting in an element
        /// @param bits_per_elem number of bits of the element to hosts chars
        /// @return The number of complete chars fitting in a elem of size bits_per_elem (0 if char size > element size)
        inline uint8_t getCharsPerElem(uint8_t bits_per_elem) const { return (isCharAligned() ? (bits_per_elem / getBitsPerChar()) : 0); }
        /// @brief Function returning the number of elements fitting in a char size
        /// @param bits_per_elem number of bits of the element to be hosted in a char size
        /// @return The number of complete elements fitting in a char (0 if element size > char size)
        inline uint8_t getElemsPerChar(uint8_t bits_per_elem) const { return (isCharAligned() ? (getBitsPerChar() / bits_per_elem) : 0); }
        inline const string &getRegex() const { return m_regex; }
        inline uint8_t getMaxBitSize() const { return m_bit_size; }
        inline uint8_t getExp() const { return m_exp; }
        inline bool isCharAligned() const { return (getBitsPerChar() > 0); }

        string toCanonicalString(const string& val) const;
        string toUserString(const string& val, bool with_header = true) const;

        template<typename U>
            Integer toInteger(const string &str, U) const = delete; //force bool as 2nd parameter, avoid confusion with int

        Integer toInteger(const string &str, bool is_already_Canonical = false) const;

        char digitToChar(const char d, bool upper_case = true) const;
        char charToDigit(const char c) const;

    protected:
        string addHeader(const string& val) const;
        string removeHeader(const string& val) const;
        string addExp(const string& val) const;
        string removeExp(const string& val) const;

    private:
        const string m_ui_name;
        const string m_header;
        uint16_t m_base;
        uint8_t m_bits_per_char;
        const string m_regex;
        uint8_t m_bit_size;
        uint8_t m_exp;
};

/// @brief Main Bases: 16 / 10 / 2 / Raw ASCII
static const ByteSetFormat Hex(     "Hex",  "0x",  16, 4, "^(0x)?[0-9a-fA-F]+",             0,  0);
static const ByteSetFormat Dec(     "Dec",    "",  10, 0, "^[0-9]+",                        0,  0);
static const ByteSetFormat Bin(     "Bin",  "0b",   2, 1, "^(0b)?[0-1]+",                   0,  0);
static const ByteSetFormat UTF8(  "UTF-8",    "", 256, 8, ".*",                             0,  0);

/// @brief Alternative Decimals
static const ByteSetFormat Wei(     "wei",    "",  10, 0, "^[0-9]+",                        0,  0);
static const ByteSetFormat Gwei(   "Gwei",    "",  10, 0, "^\\d+(\\.\\d{1,9})?$",           0,  9);
static const ByteSetFormat Eth(     "Eth",    "",  10, 0, "^\\d+(\\.\\d{1,18})?$",          0, 18);

/// @brief Alternative Hexadecimals
static const ByteSetFormat Hex20(   "Hex",  "0x",  16, 4, "^(0x)?[0-9a-fA-F]{0,40}",      160,  0); // Addresses
static const ByteSetFormat Hex32(   "Hex",  "0x",  16, 4, "^(0x)?[0-9a-fA-F]{0,64}",      256,  0); // Private Keys, Signature R, Signature S...
static const ByteSetFormat Hex33(   "Hex",  "0x",  16, 4, "^(0x)?0[2|3][0-9a-fA-F]{64}",  264,  0); // Compressed PubKeys
static const ByteSetFormat Hex64(   "Hex",  "0x",  16, 4, "^(0x)?[0-9a-fA-F]{0,128}",     512,  0); // Uncompressed PubKeys

/// @brief Alternative Binaries (BIP39 Entropy / Checksum)
static const ByteSetFormat Ent128(  "Bin",  "0b",   2, 1, "^(0b)?[0-1]{0,128}",           128,  0);  //12-words Seed entropy
static const ByteSetFormat Ent160(  "Bin",  "0b",   2, 1, "^(0b)?[0-1]{0,160}",           160,  0);  //15-words Seed entropy
static const ByteSetFormat Ent192(  "Bin",  "0b",   2, 1, "^(0b)?[0-1]{0,192}",           192,  0);  //18-words Seed entropy
static const ByteSetFormat Ent224(  "Bin",  "0b",   2, 1, "^(0b)?[0-1]{0,224}",           224,  0);  //21-words Seed entropy
static const ByteSetFormat Ent256(  "Bin",  "0b",   2, 1, "^(0b)?[0-1]{0,256}",           256,  0);  //24-words Seed entropy
static const ByteSetFormat Ent11(   "Bin",  "0b",   2, 1, "^(0b)?[0-1]{0,11}",             11,  0);  //Single word entropy (11 bits)
static const ByteSetFormat LEnt128( "Bin",  "0b",   2, 1, "^(0b)?[0-1]{0,7}",               7,  0);  //12-words Seed last word entropy
static const ByteSetFormat LEnt160( "Bin",  "0b",   2, 1, "^(0b)?[0-1]{0,6}",               6,  0);  //15-words Seed last word entropy
static const ByteSetFormat LEnt192( "Bin",  "0b",   2, 1, "^(0b)?[0-1]{0,5}",               5,  0);  //18-words Seed last word entropy
static const ByteSetFormat LEnt224( "Bin",  "0b",   2, 1, "^(0b)?[0-1]{0,4}",               4,  0);  //21-words Seed last word entropy
static const ByteSetFormat LEnt256( "Bin",  "0b",   2, 1, "^(0b)?[0-1]{0,3}",               3,  0);  //24-words Seed last word entropy
