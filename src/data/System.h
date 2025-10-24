#pragma once 

#include <givaro/modular-integer.h>

/*#include <chrono>
#include <deque>*/
#include <string>
#include <vector>
/*#include <map>
#include <sstream>
#include <iostream>*/
#include <regex>
/*#include <iomanip>
#include <stdexcept>
#include <algorithm>*/

#include <ethash/keccak.hpp>
#include <openssl/sha.h>

using Givaro::Integer;

using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;
using std::make_pair;
using std::enable_shared_from_this;
using std::dynamic_pointer_cast;
using std::const_pointer_cast;
using std::static_pointer_cast;
//using std::move;    //Generates Warning! Must be explicit std::move

/*using std::time_t;

using std::deque;*/
using std::string;
using std::vector;
/*using std::map;

using std::stoi;*/
using std::to_string;
/*using std::setfill;
using std::setw;*/

using std::regex;

using std::uppercase;
using std::nouppercase;
using std::stringstream;
using std::ostream;
using std::dec;
using std::hex;
using std::min;
using std::endl;
using std::cout;
using std::cin;
using std::cerr;