#include <gtest/gtest.h>
#include <ByteSet/TrieNode.h>

TEST(Test_TrieNode, Mainnet_TxTrie_Block_10593417)
{
    TrieRoot btt;
    ByteSet<BYTE>* value;
    ByteSet<NIBBLE> key;

    key = ByteSet<NIBBLE>(0x80, 2);
    value = new ByteSet<BYTE>("f8ab81a5852e90edd00083012bc294a3bed4e1c75d00fa6f4e5e6922db7261b5e9acd280b844a9059cbb0000000000000000000000008bda8b9823b8490e8cf220dc7b91d97da1c54e250000000000000000000000000000000000000000000000056bc75e2d6310000026a06c89b57113cf7da8aed7911310e03d49be5e40de0bd73af4c9c54726c478691ba056223f039fab98d47c71f84190cf285ce8fc7d9181d6769387e5efd0a970e2e9");
    btt.store(key, value);

    key = ByteSet<NIBBLE>(0x01, 2);
    value = new ByteSet<BYTE>("f8ab81a6852e90edd00083012bc294a3bed4e1c75d00fa6f4e5e6922db7261b5e9acd280b844a9059cbb0000000000000000000000008bda8b9823b8490e8cf220dc7b91d97da1c54e250000000000000000000000000000000000000000000000056bc75e2d6310000026a0d77c66153a661ecc986611dffda129e14528435ed3fd244c3afb0d434e9fd1c1a05ab202908bf6cbc9f57c595e6ef3229bce80a15cdf67487873e57cc7f5ad7c8a");
    btt.store(key, value);

    key = ByteSet<NIBBLE>(0x02, 2);
    value = new ByteSet<BYTE>("f86d8229f185199c82cc008252089488e9a2d38e66057e18545ce03b3ae9ce4fc360538702ce7de1537c008025a096e7a1d9683b205f697b4073a3e2f0d0ad42e708f03e899c61ed6a894a7f916aa05da238fbb96d41a4b5ec0338c86cfcb627d0aa8e556f21528e62f31c32f7e672");
    btt.store(key, value);

    key = ByteSet<NIBBLE>(0x03, 2);
    value = new ByteSet<BYTE>("f86f826b2585199c82cc0083015f9094e955ede0a3dbf651e2891356ecd0509c1edb8d9c8801051fdc4efdc0008025a02190f26e70a82d7f66354a13cda79b6af1aa808db768a787aeb348d425d7d0b3a06a82bd0518bc9b69dc551e20d772a1b06222edfc5d39b6973e4f4dc46ed8b196");
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0xab41f886be23cd786d8a69a72b0f988ea72e0b2e03970d0798f5e03763a442cc"));
}
    
TEST(Test_TrieNode, hex_encoded_securetrie_test)
{
    //https://github.com/ethereum/tests/blob/develop/TrieTests/hex_encoded_securetrie_test.json

    TrieRoot btt(true);
    ByteSet<NIBBLE> key;
    ByteSet<BYTE>* value;

    key = ByteSet<NIBBLE>("0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b");
    value = new ByteSet<BYTE>("0xf848018405f446a7a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x095e7baea6a6c7c4c2dfeb977efac326af552d87");
    value = new ByteSet<BYTE>("0xf8440101a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a004bccc5d94f4d1f99aab44369a910179931772f2a5c001c3229f57831c102769");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xd2571607e241ecf590ed94b12d87c94babe36db6");
    value = new ByteSet<BYTE>("0xf8440180a0ba4b47865c55a341a4a78759bb913cd15c3ee8eaf30a62fa8d1c8863113d84e8a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x62c01474f089b07dae603491675dc5b5748f7049");
    value = new ByteSet<BYTE>("0xf8448080a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x2adc25665018aa1fe0e6bc666dac8fc2697ff9ba");
    value = new ByteSet<BYTE>("0xf8478083019a59a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x730a444e08ab4b8dee147c9b232fc52d34a223d600031c1e9d25bfc985cbd797"));

    btt.clear();

    key = ByteSet<NIBBLE>("0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b");
    value = new ByteSet<BYTE>("0xf84c01880de0b6b3a7622746a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x095e7baea6a6c7c4c2dfeb977efac326af552d87");
    value = new ByteSet<BYTE>("0xf84780830186b7a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0501653f02840675b1aab0328c6634762af5d51764e78f9641cccd9b27b90db4f");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x2adc25665018aa1fe0e6bc666dac8fc2697ff9ba");
    value = new ByteSet<BYTE>("0xf8468082521aa056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0xa7c787bf470808896308c215e22c7a580a0087bb6db6e8695fb4759537283a83"));

    btt.clear();

    key = ByteSet<NIBBLE>("0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b");
    value = new ByteSet<BYTE>("0xf84c01880de0b6b3a7614bc3a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x095e7baea6a6c7c4c2dfeb977efac326af552d87");
    value = new ByteSet<BYTE>("0xf84880840132b3a0a065fee2fffd7a68488cf7ef79f35f7979133172ac5727b5e0cf322953d13de492a06e5d8fec8b6b9bf41c3fb9b61696d5c87b66f6daa98d5f02ba9361b0c6916467");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0000000000000000000000000000000000000001");
    value = new ByteSet<BYTE>("0xf8448080a056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x2adc25665018aa1fe0e6bc666dac8fc2697ff9ba");
    value = new ByteSet<BYTE>("0xf8478083012d9da056e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421a0c5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470");
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x40b37be88a49e2c08b8d33fcb03a0676ffd0481df54dfebd3512b8ec54f40cad"));
}

TEST(Test_TrieNode, trieanyorder)
{
    //https://github.com/ethereum/tests/blob/develop/TrieTests/trieanyorder.json

    TrieRoot btt;
    ByteSet<NIBBLE> key;
    ByteSet<BYTE>* value;

    // Working RLP is:
    // ["0x2041","0x6161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161"]
    // = 0xf6822041b26161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161
    
    key = ByteSet<NIBBLE>("A", UTF8);
    value = new ByteSet<BYTE>("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0xd23786fb4a010da3ce639d66d5e904a11dbc02746d1ce25029e53290cabf28ab"));

    btt.clear();

    /******************************************************************************************************************* */
    /*                                     NEXT TEST INTERNAL STRUCTURE FOR DEBUGGING
    
    32 Bytes Hash nodes are RLP encoded inside parent node => 33 Bytes
    < 32 Bytes Value Nodes being already RLP encoded, are not re-RLP-encoded => still < 32 Bytes

    L0:
        key: 0x
        value: "reindeer" (0x7265696E64656572)
        ["0x20", "0x7265696e64656572"] = 0xca20887265696e64656572

    L1:
        key: 0x76C6573776F727468
        value: "cat" (0x636174)
        ["0x376c6573776f727468", "0x636174"] = 0xce89376c6573776f72746883636174

    B1:
        value: "puppy" (0x7075707079)
        ["0x", "0x", "0x", "0x", "0x", "0x", "0xce89376c6573776f72746883636174", "0x", "0x", "0x", "0x", "0x", "0x", "0x", "0x", "0x", "0x7075707079"]
            = 0xe4808080808080ce89376c6573776f72746883636174808080808080808080857075707079
        keccak256 = 0x37efd11993cb04a54048c25320e9f29c50a432d28afdf01598b2978ce1ca3068

    B0:
        value: "" (0x)
        ["0x", "0x", "0x", "0x", "0x", "0xca20887265696e64656572", "0x", "0x37efd11993cb04a54048c25320e9f29c50a432d28afdf01598b2978ce1ca3068", "0x", "0x", "0x", "0x", "0x", "0x", "0x", "0x", "0x"]
            = 0xf83b8080808080ca20887265696e6465657280A037efd11993cb04a54048c25320e9f29c50a432d28afdf01598b2978ce1ca3068808080808080808080
        keccak256 = 0xdb6ae1fda66890f6693f36560d36b4dca68b4d838f17016b151efe1d4c95c453

    E0:
        key: 0x646F6
        ["0x1646f6", "0xdb6ae1fda66890f6693f36560d36b4dca68b4d838f17016b151efe1d4c95c453"]
            = 0xe5831646f6A0db6ae1fda66890f6693f36560d36b4dca68b4d838f17016b151efe1d4c95c453
        keccak256 = 0x8aad789dff2f538bca5d8ea56e8abe10f4c7ba3a5dea95fea4cd6e7c3a1168d3
    ------------------------------------------------------------------------------------------------------------------*/

    key = ByteSet<NIBBLE>("doe", UTF8);
    value = new ByteSet<BYTE>("reindeer", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("dog", UTF8);
    value = new ByteSet<BYTE>("puppy", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("dogglesworth", UTF8);
    value = new ByteSet<BYTE>("cat", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x8aad789dff2f538bca5d8ea56e8abe10f4c7ba3a5dea95fea4cd6e7c3a1168d3"));

    /*btt.clear();
    
    key = ByteSet<NIBBLE>("do", UTF8);
    value = new ByteSet<BYTE>("verb", UTF8);
    btt.store(key, value);
   
    key = ByteSet<NIBBLE>("horse", UTF8);
    value = new ByteSet<BYTE>("stallion", UTF8);
    btt.store(key, value);
  
    key = ByteSet<NIBBLE>("doge", UTF8);
    value = new ByteSet<BYTE>("coin", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("dog", UTF8);
    value = new ByteSet<BYTE>("puppy", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x5991bb8c6514148a29db676a14ac506cd2cd5775ace63c30a4fe457715e9ac84"));*/

    /******************************************************************************************************************* */
    /*                                     NEXT TEST INTERNAL STRUCTURE FOR DEBUGGING
    
    32 Bytes Hash nodes are RLP encoded inside parent node => 33 Bytes
    < 32 Bytes Value Nodes being already RLP encoded, are not re-RLP-encoded => still < 32 Bytes

    E0:
        key: 0x666F6F
        ["0x00666F6F", "0xda808080808080c634846261737380808080808080808083626172"]
            = e08400666f6fda808080808080c634846261737380808080808080808083626172
        keccak256 = 0x17beaa1648bafa633cda809c90c04af50fc8aed3cb40d16efbddee6fdf63c4c3

    B0:
        value: "bar" (0x626172)
        ["0x", "0x", "0x", "0x", "0x", "0x", "0xc6348462617373", "0x", "0x", "0x", "0x", "0x", "0x", "0x", "0x", "0x", "0x626172"]
            = 0xda808080808080c634846261737380808080808080808083626172      
        keccak256 = 0x

    L0:
        key: 0x4
        value: "bass" (0x62617373)
        ["0x34", "0x62617373"] = 0xc6348462617373
        keccak256 = 0x
    *******************************************************************************************************************/

    btt.clear();
    
    key = ByteSet<NIBBLE>("foo", UTF8);
    value = new ByteSet<BYTE>("bar", UTF8);
    btt.store(key, value);
   
    key = ByteSet<NIBBLE>("food", UTF8);
    value = new ByteSet<BYTE>("bass", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x17beaa1648bafa633cda809c90c04af50fc8aed3cb40d16efbddee6fdf63c4c3"));

    btt.clear();
    
    key = ByteSet<NIBBLE>("be", UTF8);
    value = new ByteSet<BYTE>("e", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("dog", UTF8);
    value = new ByteSet<BYTE>("puppy", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("bed", UTF8);
    value = new ByteSet<BYTE>("d", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x3f67c7a47520f79faa29255d2d3c084a7a6df0453116ed7232ff10277a8be68b"));

    btt.clear();
    
    key = ByteSet<NIBBLE>("test", UTF8);
    value = new ByteSet<BYTE>("test", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("te", UTF8);
    value = new ByteSet<BYTE>("testy", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x8452568af70d8d140f58d941338542f645fcca50094b20f3c3d8c3df49337928"));

    btt.clear();
    
    key = ByteSet<NIBBLE>("0x0045");
    value = new ByteSet<BYTE>("0x0123456789");
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("0x4500");
    value = new ByteSet<BYTE>("0x9876543210");
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x285505fcabe84badc8aa310e2aae17eddc7d120aabec8a476902c8184b3a3503"));
}

TEST(Test_TrieNode, trieanyorder_secureTrie)
{
    //https://github.com/ethereum/tests/blob/develop/TrieTests/trieanyorder_secureTrie.json

    TrieRoot btt(true);
    ByteSet<NIBBLE> key;
    ByteSet<BYTE>* value;

    // Working RLP is:
    // ["0x2041","0x6161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161"]
    // = 0xf6822041b26161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161
    
    key = ByteSet<NIBBLE>("A", UTF8);
    value = new ByteSet<BYTE>("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0xe9e2935138352776cad724d31c9fa5266a5c593bb97726dd2a908fe6d53284df"));

    btt.clear();

    key = ByteSet<NIBBLE>("doe", UTF8);
    value = new ByteSet<BYTE>("reindeer", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("dog", UTF8);
    value = new ByteSet<BYTE>("puppy", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("dogglesworth", UTF8);
    value = new ByteSet<BYTE>("cat", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0xd4cd937e4a4368d7931a9cf51686b7e10abb3dce38a39000fd7902a092b64585"));

    btt.clear();
    
    key = ByteSet<NIBBLE>("do", UTF8);
    value = new ByteSet<BYTE>("verb", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("horse", UTF8);
    value = new ByteSet<BYTE>("stallion", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("doge", UTF8);
    value = new ByteSet<BYTE>("coin", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("dog", UTF8);
    value = new ByteSet<BYTE>("puppy", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x29b235a58c3c25ab83010c327d5932bcf05324b7d6b1185e650798034783ca9d"));

    btt.clear();
    
    key = ByteSet<NIBBLE>("foo", UTF8);
    value = new ByteSet<BYTE>("bar", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("food", UTF8);
    value = new ByteSet<BYTE>("bass", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x1385f23a33021025d9e87cca5c66c00de06178807b96a9acc92b7d651ccde842"));

    btt.clear();
    
    key = ByteSet<NIBBLE>("be", UTF8);
    value = new ByteSet<BYTE>("e", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("dog", UTF8);
    value = new ByteSet<BYTE>("puppy", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("bed", UTF8);
    value = new ByteSet<BYTE>("d", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x826a4f9f9054a3e980e54b20da992c24fa20467f1ca635115ef4917be66e746f"));

    btt.clear();
    
    key = ByteSet<NIBBLE>("test", UTF8);
    value = new ByteSet<BYTE>("test", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("te", UTF8);
    value = new ByteSet<BYTE>("testy", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0xaea54fb6c80499674248a462864c420c9d9f3b3d38c879c12425bade1ad76552"));

    btt.clear();
    
    key = ByteSet<NIBBLE>("0x0045");
    value = new ByteSet<BYTE>("0x0123456789");
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("0x4500");
    value = new ByteSet<BYTE>("0x9876543210");
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0xbc11c02c8ab456db0c4d2728b6a2a6210d06f26a2ace4f7d8bdfc72ddf2630ab"));
}

TEST(Test_TrieNode, trietest)
{
    //https://github.com/ethereum/tests/blob/develop/TrieTests/trietest.json

    TrieRoot btt;
    ByteSet<NIBBLE> key;
    ByteSet<BYTE>* value;
    
    key = ByteSet<NIBBLE>("do", UTF8);
    value = new ByteSet<BYTE>("verb", UTF8);
    btt.store(key, value);
  
    key = ByteSet<NIBBLE>("ether", UTF8);
    value = new ByteSet<BYTE>("wookiedoo", UTF8);
    btt.store(key, value);
   
    key = ByteSet<NIBBLE>("horse", UTF8);
    value = new ByteSet<BYTE>("stallion", UTF8);
    btt.store(key, value);
   
    key = ByteSet<NIBBLE>("shaman", UTF8);
    value = new ByteSet<BYTE>("horse", UTF8);
    btt.store(key, value);
   
    key = ByteSet<NIBBLE>("doge", UTF8);
    value = new ByteSet<BYTE>("coin", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("ether", UTF8);
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("dog", UTF8);
    value = new ByteSet<BYTE>("puppy", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("shaman", UTF8);
    value = nullptr;
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x5991bb8c6514148a29db676a14ac506cd2cd5775ace63c30a4fe457715e9ac84"));

    btt.clear();
    
    key = ByteSet<NIBBLE>("0x04110d816c380812a427968ece99b1c963dfbce6");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x095e7baea6a6c7c4c2dfeb977efac326af552d87");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0a517d755cebbf66312b30fff713666a9cb917e0");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x24dd378f51adc67a50e339e8031fe9bd4aafab36");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x293f982d000532a7861ab122bdc4bbfd26bf9030");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x2cf5732f017b0cf1b1f13a1478e10239716bf6b5");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x31c640b92c21a1f1465c91070b4b3b4d6854195f");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x37f998764813b136ddf5a754f34063fd03065e36");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x37fa399a749c121f8a15ce77e3d9f9bec8020d7a");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x4f36659fa632310b6ec438dea4085b522a2dd077");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x62c01474f089b07dae603491675dc5b5748f7049");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x729af7294be595a0efd7d891c9e51f89c07950c7");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x83e3e5a16d3b696a0314b30b2534804dd5e11197");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x8703df2417e0d7c59d063caa9583cb10a4d20532");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x8dffcd74e5b5923512916c6a64b502689cfa65e1");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x95a4d7cccb5204733874fa87285a176fe1e9e240");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x99b2fcba8120bedd048fe79f5262a6690ed38c39");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xa4202b8b8afd5354e3e40a219bdc17f6001bf2cf");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xa9647f4a0a14042d91dc33c0328030a7157c93ae");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xaa6cffe5185732689c18f37a7f86170cb7304c2a");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xaae4a2e3c51c04606dcb3723456e58f3ed214f45");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xc37a43e940dfb5baf581a0b82b351d48305fc885");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xd2571607e241ecf590ed94b12d87c94babe36db6");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xf735071cbee190d76b704ce68384fc21e389fbe7");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x04110d816c380812a427968ece99b1c963dfbce6");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x095e7baea6a6c7c4c2dfeb977efac326af552d87");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0a517d755cebbf66312b30fff713666a9cb917e0");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x24dd378f51adc67a50e339e8031fe9bd4aafab36");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x293f982d000532a7861ab122bdc4bbfd26bf9030");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x2cf5732f017b0cf1b1f13a1478e10239716bf6b5");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x31c640b92c21a1f1465c91070b4b3b4d6854195f");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x37f998764813b136ddf5a754f34063fd03065e36");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x37fa399a749c121f8a15ce77e3d9f9bec8020d7a");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x4f36659fa632310b6ec438dea4085b522a2dd077");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x62c01474f089b07dae603491675dc5b5748f7049");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x729af7294be595a0efd7d891c9e51f89c07950c7");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x83e3e5a16d3b696a0314b30b2534804dd5e11197");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x8703df2417e0d7c59d063caa9583cb10a4d20532");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x8dffcd74e5b5923512916c6a64b502689cfa65e1");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x95a4d7cccb5204733874fa87285a176fe1e9e240");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x99b2fcba8120bedd048fe79f5262a6690ed38c39");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xa4202b8b8afd5354e3e40a219bdc17f6001bf2cf");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xa9647f4a0a14042d91dc33c0328030a7157c93ae");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xaa6cffe5185732689c18f37a7f86170cb7304c2a");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xaae4a2e3c51c04606dcb3723456e58f3ed214f45");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xc37a43e940dfb5baf581a0b82b351d48305fc885");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xd2571607e241ecf590ed94b12d87c94babe36db6");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xf735071cbee190d76b704ce68384fc21e389fbe7");
    value = nullptr;
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421"));

    btt.clear();

    key = ByteSet<NIBBLE>("0x0000000000000000000000000000000000000000000000000000000000000045");
    value = new ByteSet<BYTE>("0x22b224a1420a802ab51d326e29fa98e34c4f24ea");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0000000000000000000000000000000000000000000000000000000000000046");
    value = new ByteSet<BYTE>("0x67706c2076330000000000000000000000000000000000000000000000000000");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0000000000000000000000000000000000000000000000000000001234567890");
    value = new ByteSet<BYTE>("0x697c7b8c961b56f675d570498424ac8de1a918f6");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x000000000000000000000000697c7b8c961b56f675d570498424ac8de1a918f6");
    value = new ByteSet<BYTE>("0x1234567890");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0000000000000000000000007ef9e639e2733cb34e4dfc576d4b23f72db776b2");
    value = new ByteSet<BYTE>("0x4655474156000000000000000000000000000000000000000000000000000000");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x000000000000000000000000ec4f34c97e43fbb2816cfd95e388353c7181dab1");
    value = new ByteSet<BYTE>("0x4e616d6552656700000000000000000000000000000000000000000000000000");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x4655474156000000000000000000000000000000000000000000000000000000");
    value = new ByteSet<BYTE>("0x7ef9e639e2733cb34e4dfc576d4b23f72db776b2");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x4e616d6552656700000000000000000000000000000000000000000000000000");
    value = new ByteSet<BYTE>("0xec4f34c97e43fbb2816cfd95e388353c7181dab1");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0000000000000000000000000000000000000000000000000000001234567890");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x000000000000000000000000697c7b8c961b56f675d570498424ac8de1a918f6");
    value = new ByteSet<BYTE>("0x6f6f6f6820736f2067726561742c207265616c6c6c793f000000000000000000");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x6f6f6f6820736f2067726561742c207265616c6c6c793f000000000000000000");
    value = new ByteSet<BYTE>("0x697c7b8c961b56f675d570498424ac8de1a918f6");
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x9f6221ebb8efe7cff60a716ecb886e67dd042014be444669f0159d8e68b42100"));

    btt.clear();

    key = ByteSet<NIBBLE>("key1aa", UTF8);
    value = new ByteSet<BYTE>("0123456789012345678901234567890123456789xxx", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("key1", UTF8);
    value = new ByteSet<BYTE>("0123456789012345678901234567890123456789Very_Long", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("key2bb", UTF8);
    value = new ByteSet<BYTE>("aval3", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("key2", UTF8);
    value = new ByteSet<BYTE>("short", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("key3cc", UTF8);
    value = new ByteSet<BYTE>("aval3", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("key3", UTF8);
    value = new ByteSet<BYTE>("1234567890123456789012345678901", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0xcb65032e2f76c48b82b5c24b3db8f670ce73982869d38cd39a624f23d62a9e89"));

    btt.clear();

    key = ByteSet<NIBBLE>("abc", UTF8);
    value = new ByteSet<BYTE>("123", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("abcd", UTF8);
    value = new ByteSet<BYTE>("abcd", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("abc", UTF8);
    value = new ByteSet<BYTE>("abc", UTF8);
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x7a320748f780ad9ad5b0837302075ce0eeba6c26e3d8562c67ccc0f1b273298a"));
}

TEST(Test_TrieNode, trietest_secureTrie)
{
    //https://github.com/ethereum/tests/blob/develop/TrieTests/trietest.json

    TrieRoot btt(true);
    ByteSet<NIBBLE> key;
    ByteSet<BYTE>* value;
    
    key = ByteSet<NIBBLE>("do", UTF8);
    value = new ByteSet<BYTE>("verb", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("ether", UTF8);
    value = new ByteSet<BYTE>("wookiedoo", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("horse", UTF8);
    value = new ByteSet<BYTE>("stallion", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("shaman", UTF8);
    value = new ByteSet<BYTE>("horse", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("doge", UTF8);
    value = new ByteSet<BYTE>("coin", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("ether", UTF8);
    value = nullptr;
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("dog", UTF8);
    value = new ByteSet<BYTE>("puppy", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("shaman", UTF8);
    value = nullptr;
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x29b235a58c3c25ab83010c327d5932bcf05324b7d6b1185e650798034783ca9d"));

    btt.clear();

    key = ByteSet<NIBBLE>("0x04110d816c380812a427968ece99b1c963dfbce6");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x095e7baea6a6c7c4c2dfeb977efac326af552d87");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0a517d755cebbf66312b30fff713666a9cb917e0");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x24dd378f51adc67a50e339e8031fe9bd4aafab36");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x293f982d000532a7861ab122bdc4bbfd26bf9030");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x2cf5732f017b0cf1b1f13a1478e10239716bf6b5");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x31c640b92c21a1f1465c91070b4b3b4d6854195f");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x37f998764813b136ddf5a754f34063fd03065e36");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x37fa399a749c121f8a15ce77e3d9f9bec8020d7a");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x4f36659fa632310b6ec438dea4085b522a2dd077");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x62c01474f089b07dae603491675dc5b5748f7049");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x729af7294be595a0efd7d891c9e51f89c07950c7");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x83e3e5a16d3b696a0314b30b2534804dd5e11197");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x8703df2417e0d7c59d063caa9583cb10a4d20532");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x8dffcd74e5b5923512916c6a64b502689cfa65e1");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x95a4d7cccb5204733874fa87285a176fe1e9e240");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x99b2fcba8120bedd048fe79f5262a6690ed38c39");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xa4202b8b8afd5354e3e40a219bdc17f6001bf2cf");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xa9647f4a0a14042d91dc33c0328030a7157c93ae");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xaa6cffe5185732689c18f37a7f86170cb7304c2a");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xaae4a2e3c51c04606dcb3723456e58f3ed214f45");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xc37a43e940dfb5baf581a0b82b351d48305fc885");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xd2571607e241ecf590ed94b12d87c94babe36db6");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xf735071cbee190d76b704ce68384fc21e389fbe7");
    value = new ByteSet<BYTE>("something", UTF8);
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x04110d816c380812a427968ece99b1c963dfbce6");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x095e7baea6a6c7c4c2dfeb977efac326af552d87");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0a517d755cebbf66312b30fff713666a9cb917e0");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x24dd378f51adc67a50e339e8031fe9bd4aafab36");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x293f982d000532a7861ab122bdc4bbfd26bf9030");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x2cf5732f017b0cf1b1f13a1478e10239716bf6b5");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x31c640b92c21a1f1465c91070b4b3b4d6854195f");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x37f998764813b136ddf5a754f34063fd03065e36");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x37fa399a749c121f8a15ce77e3d9f9bec8020d7a");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x4f36659fa632310b6ec438dea4085b522a2dd077");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x62c01474f089b07dae603491675dc5b5748f7049");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x729af7294be595a0efd7d891c9e51f89c07950c7");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x83e3e5a16d3b696a0314b30b2534804dd5e11197");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x8703df2417e0d7c59d063caa9583cb10a4d20532");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x8dffcd74e5b5923512916c6a64b502689cfa65e1");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x95a4d7cccb5204733874fa87285a176fe1e9e240");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x99b2fcba8120bedd048fe79f5262a6690ed38c39");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xa4202b8b8afd5354e3e40a219bdc17f6001bf2cf");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xa94f5374fce5edbc8e2a8697c15331677e6ebf0b");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xa9647f4a0a14042d91dc33c0328030a7157c93ae");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xaa6cffe5185732689c18f37a7f86170cb7304c2a");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xaae4a2e3c51c04606dcb3723456e58f3ed214f45");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xc37a43e940dfb5baf581a0b82b351d48305fc885");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xd2571607e241ecf590ed94b12d87c94babe36db6");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0xf735071cbee190d76b704ce68384fc21e389fbe7");
    value = nullptr;
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x56e81f171bcc55a6ff8345e692c0f86e5b48e01b996cadc001622fb5e363b421"));

    btt.clear();

    key = ByteSet<NIBBLE>("0x0000000000000000000000000000000000000000000000000000000000000045");
    value = new ByteSet<BYTE>("0x22b224a1420a802ab51d326e29fa98e34c4f24ea");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0000000000000000000000000000000000000000000000000000000000000046");
    value = new ByteSet<BYTE>("0x67706c2076330000000000000000000000000000000000000000000000000000");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0000000000000000000000000000000000000000000000000000001234567890");
    value = new ByteSet<BYTE>("0x697c7b8c961b56f675d570498424ac8de1a918f6");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x000000000000000000000000697c7b8c961b56f675d570498424ac8de1a918f6");
    value = new ByteSet<BYTE>("0x1234567890");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0000000000000000000000007ef9e639e2733cb34e4dfc576d4b23f72db776b2");
    value = new ByteSet<BYTE>("0x4655474156000000000000000000000000000000000000000000000000000000");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x000000000000000000000000ec4f34c97e43fbb2816cfd95e388353c7181dab1");
    value = new ByteSet<BYTE>("0x4e616d6552656700000000000000000000000000000000000000000000000000");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x4655474156000000000000000000000000000000000000000000000000000000");
    value = new ByteSet<BYTE>("0x7ef9e639e2733cb34e4dfc576d4b23f72db776b2");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x4e616d6552656700000000000000000000000000000000000000000000000000");
    value = new ByteSet<BYTE>("0xec4f34c97e43fbb2816cfd95e388353c7181dab1");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x0000000000000000000000000000000000000000000000000000001234567890");
    value = nullptr;
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x000000000000000000000000697c7b8c961b56f675d570498424ac8de1a918f6");
    value = new ByteSet<BYTE>("0x6f6f6f6820736f2067726561742c207265616c6c6c793f000000000000000000");
    btt.store(key, value);

    key = ByteSet<NIBBLE>("0x6f6f6f6820736f2067726561742c207265616c6c6c793f000000000000000000");
    value = new ByteSet<BYTE>("0x697c7b8c961b56f675d570498424ac8de1a918f6");
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0x72adb52e9d9428f808e3e8045be18d3baa77881d0cfab89a17a2bcbacee2f320"));
}

TEST(Test_TrieNode, BranchWithValue_merging)
{
    TrieRoot btt;
    ByteSet<BYTE>* value;
    ByteSet<NIBBLE> key;

    //Merge with the above EXTENSION => LEAF
  
    key = ByteSet<NIBBLE>("test", UTF8);
    value = new ByteSet<BYTE>("test", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("te", UTF8);
    value = new ByteSet<BYTE>("testy", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("test", UTF8);
    value = nullptr;
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0xa0ad4c6568ad0369c5724f1d19a07d0e450d9a07350cd3b0e3cfb3fc04b9dd41"));

    //Merge as ROOT => LEAF

    btt.clear();
  
    key = ByteSet<NIBBLE>("test", UTF8);
    value = new ByteSet<BYTE>("test", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>();
    value = new ByteSet<BYTE>("testy", UTF8);
    btt.store(key, value);
    
    key = ByteSet<NIBBLE>("st", UTF8);
    value = nullptr;
    btt.store(key, value);

    ASSERT_EQ(btt.hash(), ByteSet("0xebb7900d6230a3fbb8ad855dc12aec8cf566c93ec302472386c96b2a5bb5a0a5"));
}