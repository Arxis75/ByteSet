#include <ByteSet/Tools.h>
#include <System.h>

/*string base64_url_encode(const string & in)
{
    string out;
    int val =0, valb=-6;
    size_t len = in.length();
    unsigned int i = 0;
    for (i = 0; i < len; i++)
    {
        unsigned char c = in[i];
        val = (val<<8) + c;
        valb += 8;
        while (valb >= 0)
        {
            out.push_back(base64_url_alphabet[(val>>valb)&0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6)
        out.push_back(base64_url_alphabet[((val<<8)>>(valb+8))&0x3F]);
    return out;
}

string base64_url_decode(const string & in)
{
    string out;
    vector<int> T(256, -1);
    unsigned int i;
    for (i =0; i < 64; i++)
        T[base64_url_alphabet[i]] = i;

    int val = 0, valb = -8;
    for (i = 0; i < in.length(); i++)
    {
        unsigned char c = in[i];
        if (T[c] == -1)
            break;
        val = (val<<6) + T[c];
        valb += 6;
        if (valb >= 0)
        {
            out.push_back(char((val>>valb)&0xFF));
            valb -= 8;
        }
    }
    return out;
}*/

vector<string> split(const string& list, const string& separator)
{
    vector<string> v;   
    int start = 0;
    int end = list.find(separator);
    while (end != -1) {
        if(end > start ) v.push_back(list.substr(start, end - start));
        start = end + separator.size();
        end = list.find(separator, start);
    }
    if(start != list.size()) v.push_back(list.substr(start, list.size() - start));
    return v;
}

/*bool isPrimeNumber(const Integer& n)
{
   bool isPrime = true;

   for(Integer i = 2; i*i <= n; i++) {
      if (n%i == 0) {
         isPrime = false;
         break;
      }
   }  
   return isPrime;
}

bool is_number(const std::string& s) {
    //Checks if the string is a valid base-10 number
    return !s.empty() && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}*/

string removeCharsFromString(const string &val, const char* charsToRemove)
{
    string str_out = val;
    for (uint8_t i = 0; i < strlen(charsToRemove); ++i ) {
        str_out.erase( remove(str_out.begin(), str_out.end(), charsToRemove[i]), str_out.end() );
    }
    return str_out;
}