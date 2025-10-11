#include <gtest/gtest.h>
#include <data/ByteSet.h>
#include "Constructors_parameters.h"

TYPED_TEST_SUITE_P(MyStrTypedTest1);
TYPED_TEST_SUITE_P(MyStrTypedTest2);
TYPED_TEST_SUITE_P(MyStrTypedTest3);
TYPED_TEST_SUITE_P(MyStrTypedTest4);

//Only the 2 first parameters are used in Default_Constructor
using MyStrTypes1 = ::testing::Types<
        StrType<8, Hex, str_dummy, int_0, 0>,  
        StrType<1, Hex, str_dummy, int_0, 0>,
        StrType<8, Dec, str_dummy, int_0, 0>,
        StrType<1, Dec, str_dummy, int_0, 0>,
        StrType<8, Bin, str_dummy, int_0, 0>,
        StrType<1, Bin, str_dummy, int_0, 0>,
        StrType<8, UTF8, str_dummy, int_0, 0>,
        StrType<1, UTF8, str_dummy, int_0, 0>,
        StrType<8, Wei, str_dummy, int_0, 0>,
        StrType<1, Wei, str_dummy, int_0, 0>,
        StrType<8, Gwei, str_dummy, int_0, 0>,
        StrType<1, Gwei, str_dummy, int_0, 0>,
        StrType<8, Eth, str_dummy, int_0, 0>,
        StrType<1, Eth, str_dummy, int_0, 0>,
        StrType<4, Hex, str_dummy, int_0, 0>,  
        StrType<4, Dec, str_dummy, int_0, 0>,
        StrType<4, Bin, str_dummy, int_0, 0>,
        StrType<4, UTF8, str_dummy, int_0, 0>,
        StrType<4, Wei, str_dummy, int_0, 0>,
        StrType<4, Gwei, str_dummy, int_0, 0>,
        StrType<4, Eth, str_dummy, int_0, 0>
>;

TYPED_TEST_P(MyStrTypedTest1, Empty_Constructor)
{
    const uint8_t bits_per_elem = TestFixture::bits_per_elem;
    const ByteSetFormat& f = TestFixture::f;

    ///* empty constructors with no explicit format (=Hex) & no explicit size (=0) *

    ByteSet<bits_per_elem> b1("");  //default format (Hex)
    ASSERT_EQ(b1.bitSize(), 0);
    ASSERT_EQ(b1.asInteger(), -1);  //-1 is the conventional value for an empty ByteSet

    ///****** empty constructors with explicit format & no explicit size (=0) ******

    ByteSet<bits_per_elem> b2("", f);
    ASSERT_EQ(b2.bitSize(), 0);
    ASSERT_EQ(b2.asInteger(), -1);  //-1 is the conventional value for an empty ByteSet

     ///****** empty constructors with explicit format & explicit size (=256) *******

     ByteSet<bits_per_elem> b3("", f, 256);
    ASSERT_EQ(b3.bitSize(), 0);
    ASSERT_EQ(b3.asInteger(), -1);  //-1 is the conventional value for an empty ByteSet
}

TEST(StrConstructorsTest, UpperCase_LowerCase)
{
    ByteSet b_U("0xABCDEF");    //constructor (const char*)
    ByteSet b_l("0xabcdef");
    ASSERT_EQ(b_U.asInteger(), b_l.asInteger());

    b_U = "0xABCDEF";           //operator= const char*
    b_l = "0xabcdef";
    ASSERT_EQ(b_U, b_l);
}

using MyStrTypes2 = ::testing::Types<
        StrType<8, Hex, str_0x646973637634646973637635, int_0x646973637634646973637635, 96>,
        StrType<1, Hex, str_0x646973637634646973637635, int_0x646973637634646973637635, 96>,
        StrType<8, Hex, str_0x000646973637634646973637635, int_0x646973637634646973637635, 112>,
        StrType<1, Hex, str_0x000646973637634646973637635, int_0x646973637634646973637635, 108>,
        StrType<8, Dec, str_31075983100084779441602131509, int_0x646973637634646973637635, 96>,
        StrType<1, Dec, str_31075983100084779441602131509, int_0x646973637634646973637635, 95>,
        StrType<8, Dec, str_00031075983100084779441602131509, int_0x646973637634646973637635, 96>,
        StrType<1, Dec, str_00031075983100084779441602131509, int_0x646973637634646973637635, 95>,
        StrType<8, Bin, str_0b11001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101, int_0x646973637634646973637635, 96>,
        StrType<1, Bin, str_0b11001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101, int_0x646973637634646973637635, 95>,
        StrType<8, Bin, str_0b00011001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101, int_0x646973637634646973637635, 104>,
        StrType<1, Bin, str_0b00011001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101, int_0x646973637634646973637635, 98>,
        StrType<8, UTF8, str_discv4discv5, int_0x646973637634646973637635, 96>,
        StrType<1, UTF8, str_discv4discv5, int_0x646973637634646973637635, 96>,
        StrType<8, Wei, str_31075983100084779441602131509, int_0x646973637634646973637635, 96>,
        StrType<1, Wei, str_31075983100084779441602131509, int_0x646973637634646973637635, 95>,
        StrType<8, Wei, str_00031075983100084779441602131509, int_0x646973637634646973637635, 96>,
        StrType<1, Wei, str_00031075983100084779441602131509, int_0x646973637634646973637635, 95>,
        StrType<8, Gwei, str_31075983100084779441g602131509, int_0x646973637634646973637635, 96>,
        StrType<1, Gwei, str_31075983100084779441g602131509, int_0x646973637634646973637635, 95>,
        StrType<8, Gwei, str_00031075983100084779441g602131509, int_0x646973637634646973637635, 96>,
        StrType<1, Gwei, str_00031075983100084779441g602131509, int_0x646973637634646973637635, 95>,
        StrType<8, Eth, str_31075983100e084779441602131509, int_0x646973637634646973637635, 96>,
        StrType<1, Eth, str_31075983100e084779441602131509, int_0x646973637634646973637635, 95>,
        StrType<8, Eth, str_00031075983100e084779441602131509, int_0x646973637634646973637635, 96>,
        StrType<1, Eth, str_00031075983100e084779441602131509, int_0x646973637634646973637635, 95>,
        StrType<4, Hex, str_0x646973637634646973637635, int_0x646973637634646973637635, 96>,
        StrType<4, Hex, str_0x000646973637634646973637635, int_0x646973637634646973637635, 108>,
        StrType<4, Dec, str_31075983100084779441602131509, int_0x646973637634646973637635, 96>,
        StrType<4, Dec, str_00031075983100084779441602131509, int_0x646973637634646973637635, 96>,
        StrType<4, Bin, str_0b11001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101, int_0x646973637634646973637635, 96>,
        StrType<4, Bin, str_0b00011001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101, int_0x646973637634646973637635, 100>,
        StrType<4, UTF8, str_discv4discv5, int_0x646973637634646973637635, 96>,
        StrType<4, Wei, str_31075983100084779441602131509, int_0x646973637634646973637635, 96>,
        StrType<4, Wei, str_00031075983100084779441602131509, int_0x646973637634646973637635, 96>,
        StrType<4, Gwei, str_31075983100084779441g602131509, int_0x646973637634646973637635, 96>,
        StrType<4, Gwei, str_00031075983100084779441g602131509, int_0x646973637634646973637635, 96>,
        StrType<4, Eth, str_31075983100e084779441602131509, int_0x646973637634646973637635, 96>,
        StrType<4, Eth, str_00031075983100e084779441602131509, int_0x646973637634646973637635, 96>
>;

TYPED_TEST_P(MyStrTypedTest2, Constructor) {
    const uint8_t bits_per_elem = TestFixture::bits_per_elem;
    const ByteSetFormat& f = TestFixture::f;
    const string str = TestFixture::str;
    const Integer val = TestFixture::val;
    const int nb_bits = TestFixture::nb_bits;

    ByteSet<bits_per_elem> b1;
    if( f == Hex) {                             
        b1 = str.c_str();                                     //operator= (const char*), Hex only
        ASSERT_EQ(b1.bitSize(), nb_bits);
        ASSERT_EQ(b1.asInteger(), val);
        b1.clear();
        b1 = str;                                             //operator= (const string&), Hex only
        ASSERT_EQ(b1.bitSize(), nb_bits);
        ASSERT_EQ(b1.asInteger(), val);
        b1.clear();
    }
    b1 = ByteSet<bits_per_elem>(str.c_str(), f);          //constructor (const char*, const ByteSetFormat&) 
    ASSERT_EQ(b1.bitSize(), nb_bits);
    ASSERT_EQ(b1.asInteger(), val);
    b1.clear();
    b1 = ByteSet<bits_per_elem>(str, f);                  //constructor (const string&, const ByteSetFormat)&) 
    ASSERT_EQ(b1.bitSize(), nb_bits);
    ASSERT_EQ(b1.asInteger(), val);
}

TYPED_TEST_P(MyStrTypedTest2, Constructor_0padding) {
    const uint8_t bits_per_elem = TestFixture::bits_per_elem;
    const ByteSetFormat& f = TestFixture::f;
    const string str = TestFixture::str;
    const Integer val = TestFixture::val;

    ///************* Str Constructors with front 0-padding **************
    
    ByteSet<bits_per_elem> b1;                             
    b1 = ByteSet<bits_per_elem>(str.c_str(), f, 257);     //constructor (const char*, const ByteSetFormat&), 257) 
    ASSERT_EQ(b1.bitSize(), 257 * b1.getBitsPerElem());
    ASSERT_EQ(b1.asInteger(), val);
    b1.clear();
    b1 = ByteSet<bits_per_elem>(str, f, 257);             //constructor (const string&, const ByteSetFormat)&, 257)
    ASSERT_EQ(b1.bitSize(), 257 * b1.getBitsPerElem());
    ASSERT_EQ(b1.asInteger(), val);
    b1.clear();
}

using MyStrTypes3 = ::testing::Types<
        StrType<8, Hex, str_0x646973637634646973637635, int_0x646973637634646973637635_3x8, 0>,
        StrType<1, Hex, str_0x646973637634646973637635, int_0x646973637634646973637635_3x1, 0>,
        StrType<8, Dec, str_31075983100084779441602131509, int_0x646973637634646973637635_3x8, 0>,
        StrType<1, Dec, str_31075983100084779441602131509, int_0x646973637634646973637635_3x1, 0>,
        StrType<8, Bin, str_0b11001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101, int_0x646973637634646973637635_3x8, 0>,
        StrType<1, Bin, str_0b11001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101, int_0x646973637634646973637635_3x1, 0>,
        StrType<8, UTF8, str_discv4discv5, int_0x646973637634646973637635_3x8, 0>,
        StrType<1, UTF8, str_discv4discv5, int_0x646973637634646973637635_3x1, 0>,
        StrType<8, Wei, str_31075983100084779441602131509, int_0x646973637634646973637635_3x8, 0>,
        StrType<1, Wei, str_31075983100084779441602131509, int_0x646973637634646973637635_3x1, 0>,
        StrType<8, Gwei, str_31075983100084779441g602131509, int_0x646973637634646973637635_3x8, 0>,
        StrType<1, Gwei, str_31075983100084779441g602131509, int_0x646973637634646973637635_3x1, 0>,
        StrType<8, Eth, str_31075983100e084779441602131509, int_0x646973637634646973637635_3x8, 0>,
        StrType<1, Eth, str_31075983100e084779441602131509, int_0x646973637634646973637635_3x1, 0>,
        StrType<4, Hex, str_0x646973637634646973637635, int_0x646973637634646973637635_3x4, 0>,
        StrType<4, Dec, str_31075983100084779441602131509, int_0x646973637634646973637635_3x4, 0>,
        StrType<4, Bin, str_0b11001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101, int_0x646973637634646973637635_3x4, 0>,
        StrType<4, UTF8, str_discv4discv5, int_0x646973637634646973637635_3x4, 0>,
        StrType<4, Wei, str_31075983100084779441602131509, int_0x646973637634646973637635_3x4, 0>,
        StrType<4, Gwei, str_31075983100084779441g602131509, int_0x646973637634646973637635_3x4, 0>,
        StrType<4, Eth, str_31075983100e084779441602131509, int_0x646973637634646973637635_3x4, 0>
>;

TYPED_TEST_P(MyStrTypedTest3, Constructor_Truncation)
{
    const uint8_t bits_per_elem = TestFixture::bits_per_elem;
    const ByteSetFormat& f = TestFixture::f;
    const string str = TestFixture::str;
    const Integer val = TestFixture::val;

    ByteSet<bits_per_elem> b;
    b = ByteSet<bits_per_elem>(str, f, 3);              // 3-elem truncation
    ASSERT_EQ(b.bitSize(), 3 * b.getBitsPerElem());
    ASSERT_EQ(b.asInteger(), val);
}

TEST(StrConstructorsTest, Empty_toInteger)
{
    ASSERT_EQ(Hex.toInteger(""), -1);   //-1 is the conventional value for an empty ByteSet
    ASSERT_EQ(Hex.toInteger("0x"), -1);
    ASSERT_EQ(Hex.toInteger("discv5"), -1);
    ASSERT_EQ(Hex.toInteger("0.1"), -1);

    ASSERT_EQ(Dec.toInteger(""), -1);   //-1 is the conventional value for an empty ByteSet
    ASSERT_EQ(Dec.toInteger("0x"), -1);
    ASSERT_EQ(Dec.toInteger("0b"), -1);
    ASSERT_EQ(Dec.toInteger("discv5"), -1);
    ASSERT_EQ(Dec.toInteger("0.1"), -1);

    ASSERT_EQ(Bin.toInteger(""), -1);   //-1 is the conventional value for an empty ByteSet
    ASSERT_EQ(Bin.toInteger("0x"), -1);
    ASSERT_EQ(Bin.toInteger("0b"), -1);
    ASSERT_EQ(Bin.toInteger("discv5"), -1);
    ASSERT_EQ(Bin.toInteger("0.1"), -1);

    ASSERT_EQ(UTF8.toInteger(""), -1);   //-1 is the conventional value for an empty ByteSet

    ASSERT_EQ(Wei.toInteger(""), -1);   //-1 is the conventional value for an empty ByteSet
    ASSERT_EQ(Wei.toInteger("0x"), -1);
    ASSERT_EQ(Wei.toInteger("0b"), -1);
    ASSERT_EQ(Wei.toInteger("discv5"), -1);
    ASSERT_EQ(Wei.toInteger("0.1"), -1);

    ASSERT_EQ(Gwei.toInteger(""), -1);   //-1 is the conventional value for an empty ByteSet
    ASSERT_EQ(Gwei.toInteger("0x"), -1);
    ASSERT_EQ(Gwei.toInteger("0b"), -1);
    ASSERT_EQ(Gwei.toInteger("discv5"), -1);

    ASSERT_EQ(Eth.toInteger(""), -1);   //-1 is the conventional value for an empty ByteSet
    ASSERT_EQ(Eth.toInteger("0x"), -1);
    ASSERT_EQ(Eth.toInteger("0b"), -1);
    ASSERT_EQ(Eth.toInteger("discv5"), -1);
}

//The first parameters is not used in toInteger
using MyStrTypes4 = ::testing::Types<
        StrType<0, Hex, str_0x646973637634646973637635, int_0x646973637634646973637635, 0>,
        StrType<0, Hex, str_0x000646973637634646973637635, int_0x646973637634646973637635, 0>,
        StrType<0, Dec, str_31075983100084779441602131509, int_0x646973637634646973637635, 0>,
        StrType<0, Dec, str_00031075983100084779441602131509, int_0x646973637634646973637635, 0>,
        StrType<0, Bin, str_0b11001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101, int_0x646973637634646973637635, 0>,
        StrType<0, Bin, str_0b00011001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101, int_0x646973637634646973637635, 0>,
        StrType<0, UTF8, str_discv4discv5, int_0x646973637634646973637635, 0>,
        StrType<0, Wei, str_31075983100084779441602131509, int_0x646973637634646973637635, 0>,
        StrType<0, Wei, str_00031075983100084779441602131509, int_0x646973637634646973637635, 0>,
        StrType<0, Gwei, str_31075983100084779441g602131509, int_0x646973637634646973637635, 0>,
        StrType<0, Gwei, str_00031075983100084779441g602131509, int_0x646973637634646973637635, 0>,
        StrType<0, Eth, str_31075983100e084779441602131509, int_0x646973637634646973637635, 0>,
        StrType<0, Eth, str_00031075983100e084779441602131509, int_0x646973637634646973637635, 0>
>;

TYPED_TEST_P(MyStrTypedTest4, toInteger)
{
    const ByteSetFormat& f = TestFixture::f;
    const string str = TestFixture::str;
    const Integer val = TestFixture::val;

    ASSERT_EQ(f.toInteger(str), val);
}

TEST(StrConstructorsTest, Empty_asString)
{
    ByteSet b;
    ASSERT_EQ(b.asString(), "0x");              //Hex with header by default
    ASSERT_EQ(b.asString(Hex), "0x");
    ASSERT_EQ(b.asString(Hex, false), "");
    ASSERT_EQ(b.asString(Dec), "");
    ASSERT_EQ(b.asString(Dec, false), "");
    ASSERT_EQ(b.asString(Bin), "0b");
    ASSERT_EQ(b.asString(Bin, false), "");
    ASSERT_EQ(b.asString(UTF8), "");
    ASSERT_EQ(b.asString(UTF8, false), "");
    ASSERT_EQ(b.asString(Wei), "");
    ASSERT_EQ(b.asString(Wei, false), "");
    ASSERT_EQ(b.asString(Gwei), "");
    ASSERT_EQ(b.asString(Gwei, false), "");
    ASSERT_EQ(b.asString(Eth), "");
    ASSERT_EQ(b.asString(Eth, false), "");
}

TEST(StrConstructorsTest, asString)
{
    //string asString(const ByteSetFormat &f = Hex, bool with_header = true, bool upper_case = true) const;

    // Testing Gwei/Eth formating of small numbers

    ByteSet b_0(0);
    ASSERT_EQ(b_0.asString(Gwei), "0.000000000");
    ASSERT_EQ(b_0.asString(Eth), "0.000000000000000000");

    ByteSet<1> b1_0(0);
    ASSERT_EQ(b1_0.asString(Gwei), "0.000000000");
    ASSERT_EQ(b1_0.asString(Eth), "0.000000000000000000");

    // Testing LowerCase / UpperCase for Hexa chars

    ByteSet b_A(0xA);
    ASSERT_EQ(b_A.asString(Hex, false, false), "0a");
    ByteSet<1> b1_A(0xA);
    ASSERT_EQ(b1_A.asString(Hex, false, false), "a");

    // Main cases (8 bits element)

    ByteSet b(int_0x646973637634646973637635);
    ASSERT_EQ(b.asString(), "0x646973637634646973637635");
    ASSERT_EQ(b.asString(Hex, false), "646973637634646973637635");
    ASSERT_EQ(b.asString(Dec), "31075983100084779441602131509");
    ASSERT_EQ(b.asString(Bin), "0b011001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101");
    ASSERT_EQ(b.asString(Bin, false), "011001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101");
    ASSERT_EQ(b.asString(UTF8), "discv4discv5");
    ASSERT_EQ(b.asString(Wei), "31075983100084779441602131509");
    ASSERT_EQ(b.asString(Gwei), "31075983100084779441.602131509");
    ASSERT_EQ(b.asString(Eth), "31075983100.084779441602131509");

    // Main cases (1 bit element)

    ByteSet<1> b1(int_0x646973637634646973637635);
    ASSERT_EQ(b1.asString(), "0x646973637634646973637635");
    ASSERT_EQ(b1.asString(Hex, false), "646973637634646973637635");
    ASSERT_EQ(b1.asString(Dec), "31075983100084779441602131509");
    ASSERT_EQ(b1.asString(Bin), "0b11001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101");
    ASSERT_EQ(b1.asString(Bin, false), "11001000110100101110011011000110111011000110100011001000110100101110011011000110111011000110101");
    ASSERT_EQ(b1.asString(UTF8), "discv4discv5");
    ASSERT_EQ(b1.asString(Wei), "31075983100084779441602131509");
    ASSERT_EQ(b1.asString(Gwei), "31075983100084779441.602131509");
    ASSERT_EQ(b1.asString(Eth), "31075983100.084779441602131509");
}

//Test exponentiation:
// 1 / 1. / 1.0 / 1.000000000000000000000000

REGISTER_TYPED_TEST_SUITE_P(MyStrTypedTest1, Empty_Constructor);
INSTANTIATE_TYPED_TEST_SUITE_P(AllCombinations, MyStrTypedTest1, MyStrTypes1);

REGISTER_TYPED_TEST_SUITE_P(MyStrTypedTest2, Constructor, Constructor_0padding);
INSTANTIATE_TYPED_TEST_SUITE_P(AllCombinations, MyStrTypedTest2, MyStrTypes2);

REGISTER_TYPED_TEST_SUITE_P(MyStrTypedTest3, Constructor_Truncation);
INSTANTIATE_TYPED_TEST_SUITE_P(AllCombinations, MyStrTypedTest3, MyStrTypes3);

REGISTER_TYPED_TEST_SUITE_P(MyStrTypedTest4, toInteger);
INSTANTIATE_TYPED_TEST_SUITE_P(AllCombinations, MyStrTypedTest4, MyStrTypes4);
