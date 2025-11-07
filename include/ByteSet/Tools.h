#pragma once

#include <givaro/modular-integer.h>
#include <ethash/keccak.hpp>
#include <openssl/sha.h>

#include <sstream>
/*#include <vector>*/

using Givaro::Integer;

/*using std::string;*/
using std::stringstream;
/*using std::vector;*/

std::vector<std::string> split(const std::string& list, const std::string& separator);

std::string removeCharsFromString(const std::string &val, const char* charsToRemove);