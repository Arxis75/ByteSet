#include <data/ByteSetFormat.h>
#include <data/Tools.h>

ByteSetFormat::ByteSetFormat(
        const string &UIName,
        const string &Header,
        uint16_t Base,
        uint8_t BitsPerChar,
        const string &Regex,
        uint64_t aligned_bit_size,
        uint8_t Exp)
    : m_ui_name(UIName)
    , m_header(Header)
    , m_base(Base)
    , m_bits_per_char(BitsPerChar)
    , m_regex(Regex)
    , m_bit_size(aligned_bit_size)
    , m_exp(Exp)
{ }

Integer ByteSetFormat::toInteger(const string &str, bool is_already_Canonical) const {
    Integer result = -1;    //-1 is the conventional result for an empty String
    string s = (is_already_Canonical ? str : toCanonicalString(str));
    if(s.length()) {
        if(getBase() == 10)
            result = Integer(s.c_str());
        else if(isCharAligned()) {
            result = 0;
            uint8_t nBitsPerChar = log2(getBase());
            for(uint64_t i = 0; i < s.length(); i++) {
                Integer byte = charToDigit(s[i]);
                result += (byte << (nBitsPerChar * (s.length() - i - 1)));
            }
        }
    }
    return result;
}

string ByteSetFormat::toCanonicalString(const string& val) const
{
    string str_ret;
    if( regex_match(val, regex(m_regex)) )
        str_ret = removeExp(removeHeader(val));
    return str_ret;
}

string ByteSetFormat::toUserString(const string& val, bool with_header) const
{
    string exp_str = (val.size() ? addExp(val) : val);

    return with_header ? addHeader(exp_str) : exp_str;
}

string ByteSetFormat::addHeader(const string& val) const
{
    return m_header + val;
}

string ByteSetFormat::removeHeader(const string& val) const
{
    string ret_val = val;
    //Removes the Header
    if(val.size() >= m_header.size() && val.substr(0, m_header.size()) == m_header)
        ret_val = val.substr(m_header.size(), val.size() - m_header.size());
    return ret_val;
}

string ByteSetFormat::addExp(const string& val) const
{
    string str_exp = val;
    if(m_exp) {
        while(str_exp.size() <= m_exp)
            str_exp = "0" + str_exp;
        str_exp = str_exp.substr(0, str_exp.size() - m_exp) + "." + str_exp.substr(str_exp.size() - m_exp, m_exp);
    }
    return str_exp;
}

string ByteSetFormat::removeExp(const string& val) const
{
    string ret_val = val;
    if(m_exp) {
        vector<string> v = split(ret_val, ".");
        if(v.size() == 1)
            //Create fractional part if missing
            v.push_back("");
        if(v.size() == 2 && v[1].size() < m_exp)
            for(int i=v[1].size();i<m_exp;i++)
                //Add right-side 0-padding if needed
                ret_val += "0";
        //Removes the Exponentiation separator
        ret_val = removeCharsFromString(ret_val.c_str(), ".");
    }
    return ret_val;
}

char ByteSetFormat::digitToChar(const char d, bool upper_case) const
{
    assert(isCharAligned());

    char c = d;
    if(getBase() != 256) {
        if (d >= 0 && d <= 9)
            c = 48 + d;
        else if (d >= 0xA && d <= 0xF)
            c = (upper_case ? 55 : 87) + d;
    }
    return c;
}

char ByteSetFormat::charToDigit(const char c) const
{
    assert(isCharAligned());

    char d = c;
    if(getBase() != 256) {
        if (c >= '0' && c <= '9')
            d = c - 48;
        else if (c >= 'a' && c <= 'f')
            d = c - 87;
        else if (c >= 'A' && c <= 'F')
            d = c - 55;
    }
    return d;
}