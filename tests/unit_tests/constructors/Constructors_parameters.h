#pragma once 

//********************************* int_Constructors *******************************/

// Template parameters
template <typename T, const uint8_t B>
struct MyIntType {
    using type = T;                                 // int, double, etc.
    static constexpr const uint8_t nb_bits_per_elem = B;
};

// Template test
template <typename Param>
class MyIntTypedTest : public ::testing::Test {
public:
    using T = typename Param::type;
    static constexpr const uint8_t nb_bits_per_elem = Param::nb_bits_per_elem;
};

//********************************* str_Constructors *******************************/

// Template parameters
template <const uint8_t A, const ByteSetFormat &B, const string &C, const Integer &D, const int E>
struct StrType {
    static constexpr const uint8_t bits_per_elem = A;
    static constexpr const ByteSetFormat &f = B;
    static constexpr const string &str = C;
    static constexpr const Integer &val = D;
    static constexpr const int nb_bits = E;
};

// Template test
template <typename Param>
class MyStrTypedTest1 : public ::testing::Test {
public:
    static constexpr const uint8_t bits_per_elem = Param::bits_per_elem;
    static constexpr const ByteSetFormat& f = Param::f;
    static constexpr const string& str = Param::str;
    static constexpr const Integer& val = Param::val;
    static constexpr const int nb_bits = Param::nb_bits;
};

template <typename Param>
    using MyStrTypedTest2 = MyStrTypedTest1<Param>;
template <typename Param>
    using MyStrTypedTest3 = MyStrTypedTest1<Param>;
template <typename Param>
    using MyStrTypedTest4 = MyStrTypedTest1<Param>;

// Static strings
static const string str_dummy("dummy");
static const string str_0x646973637634646973637635("0x646973637634646973637635");
static const string str_31075983100084779441602131509("31075983100084779441602131509");
static const string str_0b11001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101("0b11001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101");
static const string str_discv4discv5("discv4discv5");
static const string str_31075983100084779441g602131509("31075983100084779441.602131509");
static const string str_31075983100e084779441602131509("31075983100.084779441602131509");

static const string str_0x000646973637634646973637635("0x000646973637634646973637635");
static const string str_0b00011001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101("0b00011001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101");
static const string str_00031075983100084779441602131509("00031075983100084779441602131509");
static const string str_00031075983100084779441g602131509("00031075983100084779441.602131509");
static const string str_00031075983100e084779441602131509("00031075983100.084779441602131509");

// Static values
static const Integer int_0(Integer::zero);
static const Integer int_0x646973637634646973637635 = Integer("31075983100084779441602131509");
static const Integer int_0x646973637634646973637635_3x8 = 0x637635;
static const Integer int_0x646973637634646973637635_3x4 = 0x635;
static const Integer int_0x646973637634646973637635_3x1 = 0x5;