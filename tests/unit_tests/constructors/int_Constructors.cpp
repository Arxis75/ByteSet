#include <gtest/gtest.h>
#include <data/ByteSet.h>

#include "Constructors_parameters.h"

TEST(IntConstructorsTest, Empty_Constructor)
{ 
    ///************* empty constructor **************
    ByteSet b1;                     //Aligned by default
    ASSERT_EQ(b1.getBitsPerElem(), 8);
    ASSERT_EQ(b1.bitSize(), 0);
    ASSERT_EQ(b1.byteSize(), 0);
    ASSERT_EQ(b1.asInteger(), -1);  //-1 is the conventional value for an empty ByteSet
    
    ByteSet<1> b2;                  //not Aligned
    ASSERT_EQ(b2.getBitsPerElem(), 1);
    ASSERT_EQ(b2.bitSize(), 0);
    ASSERT_EQ(b2.byteSize(), 0);
    ASSERT_EQ(b2.asInteger(), -1);  //-1 is the conventional value for an empty ByteSet

    ///************* empty Int Constructors from explicit value < 0 **************
    
    ByteSet b3;                     //Aligned by default
    b3 = -1;                        //operator= 
    ASSERT_EQ(b3.bitSize(), 0);
    ASSERT_EQ(b3.byteSize(), 0);
    ASSERT_EQ(b3.asInteger(), -1);  //-1 is the conventional value for an empty ByteSet
    b3.clear();
    b3 = ByteSet(-2);               //constructor 
    ASSERT_EQ(b3.bitSize(), 0);
    ASSERT_EQ(b3.byteSize(), 0);
    ASSERT_EQ(b3.asInteger(), -1);  //-1 is the conventional value for an empty ByteSet
    b3.clear();
    b3 = ByteSet(-3, 256);          //256 elem-wide constructor
    ASSERT_EQ(b3.bitSize(), 0);
    ASSERT_EQ(b3.byteSize(), 0);
    ASSERT_EQ(b3.asInteger(), -1);  //-1 is the conventional value for an empty ByteSet
    b3.clear();

    ///************* empty Bin Constructors from explicit value < 0 **************
    
    ByteSet<1> b4;               //Not Aligned
    b4 = -1;                        //operator= 
    ASSERT_EQ(b4.bitSize(), 0);
    ASSERT_EQ(b4.byteSize(), 0);
    ASSERT_EQ(b4.asInteger(), -1);  //-1 is the conventional value for an empty ByteSet
    b4.clear();
    b4 = ByteSet<1>(-2);         //constructor 
    ASSERT_EQ(b4.bitSize(), 0);
    ASSERT_EQ(b4.byteSize(), 0);
    ASSERT_EQ(b4.asInteger(), -1);  //-1 is the conventional value for an empty ByteSet
    b4.clear();
    b4 = ByteSet<1>(-3, 256);    //256 elem-wide constructor
    ASSERT_EQ(b4.bitSize(), 0);
    ASSERT_EQ(b4.byteSize(), 0);
    ASSERT_EQ(b4.asInteger(), -1);  //-1 is the conventional value for an empty ByteSet
    b4.clear();

    ///************* Int Constructors from explicit value (0) **************
    
    ByteSet b5;                     //Aligned by default
    b5 = 0;                         //operator= 
    ASSERT_EQ(b5.bitSize(), 8);
    ASSERT_EQ(b5.byteSize(), 1);
    ASSERT_EQ(b5.asInteger(), 0);
    b5.clear();
    b5 = ByteSet(0);                 //constructor 
    ASSERT_EQ(b5.bitSize(), 8);
    ASSERT_EQ(b5.byteSize(), 1);
    ASSERT_EQ(b5.asInteger(), 0);
    b5.clear();
    b5 = ByteSet(0, 256);            //256 elem-wide constructor
    ASSERT_EQ(b5.bitSize(), 2048);
    ASSERT_EQ(b5.byteSize(), 256);
    ASSERT_EQ(b5.asInteger(), 0);
    b5.clear();

    ///************* Bin Constructors from explicit value (0) **************
    
    ByteSet<1> b6;               //Not Aligned
    b6 = 0;                         //operator= 
    ASSERT_EQ(b6.bitSize(), 1);
    ASSERT_EQ(b6.byteSize(), 1);
    ASSERT_EQ(b6.asInteger(), 0);
    b6.clear();
    b6 = ByteSet<1>(0);         //constructor 
    ASSERT_EQ(b6.bitSize(), 1);
    ASSERT_EQ(b6.byteSize(), 1);
    ASSERT_EQ(b6.asInteger(), 0);
    b6.clear();
    b6 = ByteSet<1>(0, 256);    //256 elem-wide constructor
    ASSERT_EQ(b6.bitSize(), 256);
    ASSERT_EQ(b6.byteSize(), 32);
    ASSERT_EQ(b6.asInteger(), 0);
    b6.clear();
}

using MyIntTypes = ::testing::Types<
    MyIntType<int8_t, 8>,
    MyIntType<int8_t, 1>,
    MyIntType<uint8_t, 1>,
    MyIntType<uint8_t, 8>,
    MyIntType<int16_t, 1>,
    MyIntType<int16_t, 8>,
    MyIntType<uint16_t, 1>,
    MyIntType<uint16_t, 8>,
    MyIntType<int32_t, 1>,
    MyIntType<int32_t, 8>,
    MyIntType<uint32_t, 1>,
    MyIntType<uint32_t, 8>,
    MyIntType<int64_t, 1>,
    MyIntType<int64_t, 8>,
    MyIntType<uint64_t, 1>,
    MyIntType<uint64_t, 8>,
    MyIntType<Integer, 8>,
    MyIntType<Integer, 1>,

    MyIntType<int8_t, 4>,
    MyIntType<uint8_t, 4>,
    MyIntType<int16_t, 4>,
    MyIntType<uint16_t, 4>,
    MyIntType<int32_t, 4>,
    MyIntType<uint32_t, 4>,
    MyIntType<int64_t, 4>,
    MyIntType<uint64_t, 4>,
    MyIntType<Integer, 4>
>;

TYPED_TEST_SUITE(MyIntTypedTest, MyIntTypes);
TYPED_TEST(MyIntTypedTest, Constructors) {
    using T = typename TestFixture::T;
    const uint8_t nb = TestFixture::nb_bits_per_elem;

    //Fills the value with 1, except for the sign bit if present
    T val = (std::is_signed_v<T> ? 0x7F : 0xFF);
    for(int i=1;i<sizeof(T);i++)
        val = (val<<8) + 0xFF;

    ByteSet<nb> b;

    ///************* Constructors **************
    b = val;                                //operator= 
    ASSERT_EQ(b.bitSize(), 8 * sizeof(T) - (b.getBitsPerElem() == 1 && std::is_signed_v<T>));   // skip the sign bit if not aligned && present
    ASSERT_EQ(b.asInteger(), val);
    b.clear();
    b = ByteSet<nb>(val);                   //constructor 
    ASSERT_EQ(b.bitSize(), 8 * sizeof(T) - (b.getBitsPerElem() == 1 && std::is_signed_v<T>));   // skip the sign bit if not aligned && present
    ASSERT_EQ(b.asInteger(), val);
    b.clear();
    b = ByteSet<nb>(val, 131);              //131 elem-wide constructor
    ASSERT_EQ(b.bitSize(), 131 * b.getBitsPerElem());
    ASSERT_EQ(b.asInteger(), val);                                      //Big-Endian
    b.clear();
}

TEST(IntConstructorsTest, BigIntegerConstructor) {
    //Test with a Big Int
    Integer val("31075983100084779441602131509");   // 95 bits integer (11.875 Bytes)
    
    ///************* Constructors from <Integer, 8> **************
    ByteSet b1;

    b1 = val;                           //operator= 
    ASSERT_EQ(b1.byteSize(), 12);
    ASSERT_EQ(b1.asInteger(), val);
    b1.clear();
    b1 = ByteSet(val);                  //constructor 
    ASSERT_EQ(b1.byteSize(), 12);
    ASSERT_EQ(b1.asInteger(), val);
    b1.clear();
    b1 = ByteSet(val, 131);              //131 bytes constructor
    ASSERT_EQ(b1.byteSize(), 131);
    ASSERT_EQ(b1.asInteger(), val); 
    b1.clear();

    ///************* Constructors from <Integer, 1> **************
    ByteSet<1> b2;

    b2 = val;                           //operator= 
    ASSERT_EQ(b2.bitSize(), 95);
    ASSERT_EQ(b2.asInteger(), val);
    b2.clear();
    b2 = ByteSet<1>(val);               //constructor 
    ASSERT_EQ(b2.bitSize(), 95);
    ASSERT_EQ(b2.asInteger(), val);
    b2.clear();
    b2 = ByteSet<1>(val, 131);          //131 bits constructor
    ASSERT_EQ(b2.bitSize(), 131);
    ASSERT_EQ(b2.asInteger(), val); 
    b2.clear();
}

TEST(IntConstructorsTest, Value_Truncation)
{ 
    ///************* Int Constructor from explicit value (0x1000F) **************
    
    ByteSet b1(0x1000F, 2);                     //Force truncation to 2-Bytes
    ASSERT_EQ(b1.bitSize(), 16);                //is 16 bits (instead of 17)
    ASSERT_EQ(b1.byteSize(), 2);                //is 2 Bytes
    ASSERT_EQ(b1.asInteger(), 0xF);             //has truncated right-value
    b1.clear();

    ///************* Bin0 Constructor from explicit value (0x2000F) **************
    
    ByteSet<1> b2(0x2000F, 17);             //Force truncation to 17-bits
    ASSERT_EQ(b2.bitSize(), 17);                //is 17 bits (instead of 18)
    ASSERT_EQ(b2.byteSize(), 3);                //is 3 Bytes
    ASSERT_EQ(b2.asInteger(), 0xF);             //has truncated right-value
    b2.clear();
}
