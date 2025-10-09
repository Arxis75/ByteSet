#pragma once

#include <data/System.h>

/**
 * @brief 
 *      Map of the hexa chars, providing iterator capability
 */
//struct cmp_str { bool operator()(char const *a, char const *b) const { return strcmp(a, b) < 0; } };

/*static const map<const char*, const uint8_t, cmp_str> map_hexa_chars = { {"0", 0}, {"1", 1}, {"2", 2}, {"3", 3},
                                                            {"4", 4}, {"5", 5}, {"6", 6}, {"7", 7},
                                                            {"8", 8}, {"9", 9}, {"a", 10}, {"A", 10},
                                                            {"b", 11}, {"B", 11}, {"c", 12}, {"C", 12},
                                                            {"d", 13}, {"D", 13}, {"e", 14}, {"E", 14},
                                                            {"f", 15}, {"F", 15} };*/

/*static uint64_t getUnixTimeStamp(const time_t* t = nullptr)
{
    //if specific time is not passed then get current time
    time_t st = t == nullptr ? std::time(nullptr) : *t;
    auto secs = static_cast<std::chrono::seconds>(st).count();
    return static_cast<uint64_t>(secs);
}*/

/**
 * @brief Base64 translates 24 bits into 4 ASCII characters at a time. First,
 *        3 8-bit bytes are treated as 4 6-bit groups. Those 4 groups are
 *        translated into ASCII characters. That is, each 6-bit number is treated
 *        as an index into the ASCII character array.
 *        
 *        If the final set of bits is less 8 or 16 instead of 24, traditional base64
 *        would add a padding character. However, if the length of the data is
 *        known, then padding can be eliminated.
 *        
 *        One difference between the "standard" Base64 is two characters are different.
 *        See RFC 4648 for details.
 *        This is how we end up with the Base64 URL encoding.
 */
/*const char base64_url_alphabet[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'
};*/

//string base64_url_encode(const string & in);

//string base64_url_decode(const string & in);

/*static inline uint64_t log2_64(const uint64_t x)
{
  uint64_t y;
  asm ( "\tbsr %1, %0\n"
      : "=r"(y)
      : "r" (x)
  );
  return y;
}*/

/*static inline uint64_t sizeInBytes64(const uint64_t& value)
{
    return 1 + (log2_64(value)>>3);
}*/

/*static inline uint64_t sizeInBytes(const Integer& value)
{
    uint64_t tmp = value.size_in_base(2);
    return (tmp>>3) + (tmp%8);
}*/

/**
 * @brief 
 *      This function basically removes all separators and spreads the remaining words inside a vector
 *      The strict sequence (n x "1 word / 1 separator") is not verified (several consecutive separators
 *      are not interpreted as empty word(s); they are just removed)
 * @param list 
 * @param separator 
 * @return vector<string> 
 */
vector<string> split(const string& list, const string& separator);

//bool isPrimeNumber(const Integer& n);

string removeCharsFromString(const string &val, const char* charsToRemove);