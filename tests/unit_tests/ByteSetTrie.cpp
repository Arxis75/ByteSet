#include <gtest/gtest.h>
#include <ByteSet/ByteSetTrie.h>

TEST(ByteSetTrieTest, hex_encoded_securetrie_test)
{
    //https://github.com/ethereum/tests/blob/develop/TrieTests/hex_encoded_securetrie_test.json

    BlockTransactionsTrie btt;
    ByteSet<NIBBLE> key;
    ByteSet<BYTE> value;

    /*key = ByteSet<NIBBLE>("0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b");
    value = ByteSet<BYTE>("0xf848018405f446a7a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x095e7baea6a6c7c4c2dfeb977efac326af552d87");
    value = ByteSet<BYTE>("0xf8440101a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a004bccc5d94f4d1f99aab44369a910179931772f2a5c001c3229f57831c102769");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xd2571607e241ecf590ed94b12d87c94babe36db6");
    value = ByteSet<BYTE>("0xf8440180a0ba4b47865c55a341a4a78759bb913cd15c3ee8eaf30a62fa8d1c8863113d84e8a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x62c01474f089b07dae603491675dc5b5748f7049");
    value = ByteSet<BYTE>("0xf8448080a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x2adc25665018aa1fe0e6bc666dac8fc2697ff9ba");
    value = ByteSet<BYTE>("0xf8478083019a59a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x730a444e08ab4b8dee147c9b232fc52d34a223d600031c1e9d25bfc985cbd797"));

    btt.clear();*/

    /*key = ByteSet<NIBBLE>("0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b");
    value = ByteSet<BYTE>("0xf84c01880de0b6b3a7622746a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x095e7baea6a6c7c4c2dfeb977efac326af552d87");
    value = ByteSet<BYTE>("0xf84780830186b7a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0501653f02840675b1aab0328c6634762af5d51764e78f9641cccd9b27b90db4f");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x2adc25665018aa1fe0e6bc666dac8fc2697ff9ba");
    value = ByteSet<BYTE>("0xf8468082521aa056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0xa7c787bf470808896308c215e22c7a580a0087bb6db6e8695fb4759537283a83"));

    btt.clear();*/

    /*key = ByteSet<NIBBLE>("0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b");
    value = ByteSet<BYTE>("0xf84c01880de0b6b3a7614bc3a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x095e7baea6a6c7c4c2dfeb977efac326af552d87");
    value = ByteSet<BYTE>("0xf84880840132b3a0a065fee2fffd7a68488cf7ef79f35f7979133172ac5727b5e0cf322953d13de492a06e5d8fec8b6b9bf41c3fb9b61696d5c87b66f6daa98d5f02ba9361b0c6916467");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0000000000000000000000000000000000000001");
    value = ByteSet<BYTE>("0xf8448080a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x2adc25665018aa1fe0e6bc666dac8fc2697ff9ba");
    value = ByteSet<BYTE>("0xf8478083012d9da056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x40b37be88a49e2c08b8d33fcb03a0676ffd0481df54dfebd3512b8ec54f40cad"));*/
}

TEST(ByteSetTrieTest, trieanyorder)
{
    //https://github.com/ethereum/tests/blob/develop/TrieTests/trieanyorder.json

    BlockTransactionsTrie btt;
    ByteSet<NIBBLE> key;
    ByteSet<BYTE> value;

    // Working RLP is:
    // ["0x2041","0x6161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161"]
    // = 0xf6822041b26161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161
    key = ByteSet<NIBBLE>("A", UTF8);
    value = ByteSet<BYTE>("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0xd23786fb4a010da3ce639d66d5e904a11dbc02746d1ce25029e53290cabf28ab"));

    btt.clear();

    /*key = ByteSet<NIBBLE>("doe", UTF8);
    value = ByteSet<BYTE>("reindeer", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("dog", UTF8);
    value = ByteSet<BYTE>("puppy", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("dogglesworth", UTF8);
    value = ByteSet<BYTE>("cat", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x8aad789dff2f538bca5d8ea56e8abe10f4c7ba3a5dea95fea4cd6e7c3a1168d3"));*/

    /*btt.clear();
    
    key = ByteSet<NIBBLE>("do", UTF8);
    value = ByteSet<BYTE>("verb", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("horse", UTF8);
    value = ByteSet<BYTE>("stallion", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("doge", UTF8);
    value = ByteSet<BYTE>("coin", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("dog", UTF8);
    value = ByteSet<BYTE>("puppy", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x5991bb8c6514148a29db676a14ac506cd2cd5775ace63c30a4fe457715e9ac84"));*/

    btt.clear();
    
    key = ByteSet<NIBBLE>("foo", UTF8);
    value = ByteSet<BYTE>("bar", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("food", UTF8);
    value = ByteSet<BYTE>("bass", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x17beaa1648bafa633cda809c90c04af50fc8aed3cb40d16efbddee6fdf63c4c3"));

    /*btt.clear();
    
    key = ByteSet<NIBBLE>("be", UTF8);
    value = ByteSet<BYTE>("e", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("dog", UTF8);
    value = ByteSet<BYTE>("puppy", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("bed", UTF8);
    value = ByteSet<BYTE>("d", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x3f67c7a47520f79faa29255d2d3c084a7a6df0453116ed7232ff10277a8be68b"));*/

    btt.clear();
    
    key = ByteSet<NIBBLE>("test", UTF8);
    value = ByteSet<BYTE>("test", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("te", UTF8);
    value = ByteSet<BYTE>("testy", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x8452568af70d8d140f58d941338542f645fcca50094b20f3c3d8c3df49337928"));

    btt.clear();
    
    key = ByteSet<NIBBLE>("0x0045");
    value = ByteSet<BYTE>("0x0123456789");
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("0x4500");
    value = ByteSet<BYTE>("0x9876543210");
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x285505fcabe84badc8aa310e2aae17eddc7d120aabec8a476902c8184b3a3503"));
}