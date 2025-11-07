#pragma once

#include <givaro/modular-integer.h>
#include <ethash/keccak.hpp>
#include <openssl/sha.h>

#include <memory>
#include <sstream>

using Givaro::Integer;

std::vector<std::string> split(const std::string& list, const std::string& separator);

std::string removeCharsFromString(const std::string &val, const char* charsToRemove);