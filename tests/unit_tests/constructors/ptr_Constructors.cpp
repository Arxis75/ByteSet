#include <gtest/gtest.h>

#include <data/ByteSet.h>

// Helper function to test if a type can be constructed with a specific value
template <typename T, typename... Args>
constexpr bool is_constructible_with_value(Args&&... args) {
    return std::is_constructible_v<T, Args...>;
}

TEST(PtrConstructorsTest, Constructors)
{ 
    const unsigned int cui[] = {0xAA, 0xBB, 0xCC};
    unsigned int ui[] = {0xAA, 0xBB, 0xCC};

    const unsigned char cuc[] = {0xAA, 0xBB, 0xCC};
    unsigned char uc[] = {0xAA, 0xBB, 0xCC};

    //******* Verifies that the constructor won't accept a 1-bit-per-elem format ********
    ASSERT_DEATH(ByteSet<1> a(cuc, 3), "");
    ASSERT_DEATH(ByteSet<1> a(uc, 3), "");

    //******* Verifies that the constructor won't accept (const) unsigned int* argument ********
    ASSERT_EQ(true, !is_constructible_with_value<ByteSet<>>(cui, 3));
    ASSERT_EQ(true, !is_constructible_with_value<ByteSet<>>(ui, 3));

    //******* Verifies that the constructor argument can't be converted to integral/const char*/string ********
    ASSERT_EQ(true, !is_constructible_with_value<ByteSet<>>(cuc));
    ASSERT_EQ(true, !is_constructible_with_value<ByteSet<>>(uc));
    ASSERT_EQ(true, !is_constructible_with_value<ByteSet<>>(cui));
    ASSERT_EQ(true, !is_constructible_with_value<ByteSet<>>(ui));

    //******* Construction from a const unsigned char* ********
    ByteSet b1(cuc, 3);                      
    ASSERT_EQ(b1.byteSize(), 3);                    //is 3 Bytes
    ASSERT_EQ(b1.asInteger(), 0xAABBCC);
    b1.clear();

    //******* Construction from a unsigned char* ********
    ByteSet b2(uc, 3);                      
    ASSERT_EQ(b2.byteSize(), 3);                    //is 3 Bytes
    ASSERT_EQ(b2.asInteger(), 0xAABBCC);
    b2.clear();
}

void ccopy(const unsigned char* p, unsigned char* dest, uint64_t nb_elem) { memcpy(dest, p, nb_elem); }
void zero(unsigned char* p, uint64_t nb_elem) { memset(p, 0, sizeof(nb_elem)); }

TEST(PtrConstructorsTest, Ptr_Operator)
{ 
    ByteSet b(0xFFFFFF, 3);

    //******* Verifies that the ByteSet is converted to const unsigned char* ********
    unsigned char tmp[3] = {0, 0, 0};
    ccopy(b, tmp, b.byteSize());
    ASSERT_EQ(tmp[0], 0xFF);
    ASSERT_EQ(tmp[1], 0xFF);
    ASSERT_EQ(tmp[2], 0xFF);
    
    //******* Verifies that the ByteSet is converted to unsigned char* ********
    zero(b, b.byteSize());
    ASSERT_EQ(b.asInteger(), Integer::zero);
}

TEST(PtrConstructorsTest, Brackets_Operator)
{ 
    unsigned char uc[] = {0xAA, 0xBB, 0xCC};
    ByteSet b(0xFFFFFF, 3);

    //******* Verifies that the operator[] won't accept index over bondaries ********
    ASSERT_DEATH(uc[2] = b[10], "");
    ASSERT_DEATH(const_cast<unsigned char&>(b[10]) = uc[2], "");

    const_cast<unsigned char&>(b[1]) = uc[1];
    ASSERT_EQ(b.asInteger(), 0xFFBBFF);

    uc[2] = b[2];
    ASSERT_EQ(uc[2], 0xFF);
}

TEST(PtrConstructorsTest, Value_Truncation)
{ 
    const unsigned char cuc[] = {0xAA, 0xBB, 0xCC};
    unsigned char uc[] = {0xAA, 0xBB, 0xCC};

    //************* Constructors from explicit value (0xFFF) **************
    ByteSet b1(cuc, 2);                         //Force truncation to 2-Bytes
    ASSERT_EQ(b1.byteSize(), 2);                //is 2 Bytes
    ASSERT_EQ(b1.asInteger(), 0xAABB);          //has truncated left-value
    b1.clear();

   //******* Construction from a unsigned char* ********
    ByteSet b2(uc, 2);                      
    ASSERT_EQ(b2.byteSize(), 2);                    //is 2 Bytes
    ASSERT_EQ(b2.asInteger(), 0xAABB);              //has truncated left-value
    b2.clear();

    //******* Truncation from oversized-int copy ********
    unsigned int ui[] = {0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC};
    ByteSet b3(0xFFFFFF, 3);

    const_cast<unsigned char&>(b3[1]) = ui[1];
    ASSERT_EQ(b3.asInteger(), 0xFFBBFF);

    ui[2] = b3[2];
    ASSERT_EQ(ui[2], 0x000000FF);
}
