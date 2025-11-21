#include <gtest/gtest.h>
#include <ByteSet/ByteSet.h>

TEST(Test_Alignment, as)
{ 
    //  B = 0xF0A5
    //  n = 0xFEDCB
    //  b = 0x1DCBA
    ByteSet<BYTE> B("0b1111000010100101", Bin), B1;         //<BYTE> =    240/165, <NIBBLE> =    15/00/10/05 
    ByteSet<NIBBLE> n("0b11111110110111001011", Bin), n1;   //<BYTE> = 15/237/203, <NIBBLE> = 15/14/13/12/11
    ByteSet<BIT> b("0b11101110010111010", Bin), b1;         //<BYTE> =  1/220/186, <NIBBLE> =  1/13/12/11/10

    B1 = B.as<BYTE>();
    ASSERT_EQ(B1, B);
    B1 = n.as<BYTE>();
    ASSERT_EQ(B1.asInteger(), 0xFEDCB);
    ASSERT_EQ(B1.getNbElements(), 3);
    B1 = b.as<BYTE>();
    ASSERT_EQ(B1.asInteger(), 0x1DCBA);
    ASSERT_EQ(B1.getNbElements(), 3);

    n1 = B.as<NIBBLE>();
    ASSERT_EQ(n1.asInteger(), 0xF0A5);
    ASSERT_EQ(n1.getNbElements(), 4);
    n1 = n.as<NIBBLE>();
    ASSERT_EQ(n1, n);
    n1 = b.as<NIBBLE>();
    ASSERT_EQ(n1.asInteger(), 0x1DCBA);
    ASSERT_EQ(n1.getNbElements(), 5);

    b1 = B.as<BIT>();
    ASSERT_EQ(b1.asInteger(), 0xF0A5);
    ASSERT_EQ(b1.getNbElements(), 16);
    b1 = n.as<BIT>();
    ASSERT_EQ(b1.asInteger(), 0xFEDCB);
    ASSERT_EQ(b1.getNbElements(), 20);
    b1 = b.as<BIT>();
    ASSERT_EQ(b1, b);
}