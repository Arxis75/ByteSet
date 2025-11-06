#include <ByteSet/EthComposite.h>
#include <System.h>

template<typename T>
void EthContainer::buildItem(uint64_t index, uint64_t type) {
    if(auto nest = const_cast<ByteSetComposite*>(dynamic_cast<const ByteSetComposite*>(getChildAt(0))); nest) {
        if(!m_items)
            m_items = unique_arr<unique_ptr<IByteSetContainer>>(nest->getChildrenCount());
        m_items[index] = make_unique<T>();
        nest->moveChildAt_To(index, m_items[index]);
        const_cast<T*>(dynamic_cast<const T*>(m_items[index].get()))->buildStructure(type);
    }
}

template<typename T>
void EthContainer::buildAllItems(bool typed) {
    //DumpChildren();
    if(auto nest = const_cast<ByteSetComposite*>(dynamic_cast<const ByteSetComposite*>(getChildAt(0))); nest) {
        //nest->DumpChildren();
        if(!m_items)
            m_items = unique_arr<unique_ptr<IByteSetContainer>>(nest->getChildrenCount());
        for(uint i = 0; i<m_items.size(); i++) {
            uint64_t type =  typed ? nest->RLPparseTypedChildAt(i) : 0;
            buildItem<T>(i, type);
        }
        deleteChildren();
    }
}

const ByteSet<8> EthContainer::RLPserialize() const {
    ByteSet<8> rlp;
    for(uint64_t i=0; i<m_items.size(); i++) {
        if(m_items[i]) {
            rlp.push_back(m_items[i]->RLPserialize());
        }
    }
    
    rlp = rlp.RLPserialize(true);               
    if(getType()) {
        rlp.push_front(ByteSet<8>(getType()));
        rlp = rlp.RLPserialize(false);
    }
    return rlp;
}

void EthContainer::DumpFields() const
{
    if(m_items.size())
        for(int i=0;i<m_items.size();i++) {
            cout << hex << m_items[i].get() << " (";
            if(!m_items[i])
                cout << "nullptr";
            else {
                if(!m_items[i]->getComposite())
                    cout << "F";
                else {
                    cout << "C:" << dec << dynamic_cast<const ByteSetComposite*>(m_items[i].get())->getChildrenCount();
                }
                cout << " P:" << hex << m_items[i]->getParent();
            }
            cout << ") ";
        }
    else
        cout << "None ()";
    cout << endl;
}

const Block* BlockChain::newBlockFromRawRLP(ByteSet<8> &b) {
    auto block = make_unique<Block>();
    block->RLPparse(b);
    block->buildStructure();
    auto result= block.get();
    m_blocks.insert({block->getHeight(), std::move(block)});
    return result;
} 

void Block::buildStructure(uint64_t) {
    buildItem<BlockHeader>(0);
    buildItem<BlockTransactions>(1);
    buildItem<BlockUncles>(2);
    buildItem<BlockWithdrawals>(3);
    
    deleteChildren();

    setHeight(getHeader()->get<const BlockField>(8)->getIntValue());
}

void BlockTransaction::buildStructure(uint64_t type)
{
    setType(type);

    switch(getType()) {
        case 1: //EIP-2930 (11 fields)
            //chain_id, nonce, gas_price, gas_limit, to, value, data, access_list, signature_y_parity, signature_r, signature_s
            for(uint i = 0; i < 7; i++) buildItem<BlockField>(i);
            buildItem<BlockAccessLists>(7);
            for(uint i = 8; i < m_items.size(); i++) buildItem<BlockField>(i);
            deleteChildren();
            break;
        case 2: //EIP-1559 (12 fields)
            //chain_id, nonce, max_priority_fee_per_gas, max_fee_per_gas, gas_limit, destination, amount, data, access_list, signature_y_parity, signature_r, signature_s
            for(uint i = 0; i < 8; i++) buildItem<BlockField>(i);
            buildItem<BlockAccessLists>(8);
            for(uint i = 9; i < m_items.size(); i++) buildItem<BlockField>(i);
            deleteChildren();
            break;
        case 3: //EIP-4844 (14 fields)
            //chain_id, nonce, max_priority_fee_per_gas, max_fee_per_gas, gas_limit, to, value, data, access_list, max_fee_per_blob_gas, blob_versioned_hashes, y_parity, r, s
            for(uint i = 0; i < 8; i++) buildItem<BlockField>(i);
            buildItem<BlockAccessLists>(8);
            buildItem<BlockField>(9);
            buildItem<BlockBlobVersionHashes>(10);
            for(uint i = 11; i < m_items.size(); i++) buildItem<BlockField>(i);
            deleteChildren();
            break;
        case 4: //EIP-7702 (13 fields)
            //chain_id, nonce, max_priority_fee_per_gas, max_fee_per_gas, gas_limit, to, value, data, access_list, authorization_list, signature_y_parity, signature_r, signature_s
            for(uint i = 0; i < 8; i++) buildItem<BlockField>(i);
            buildItem<BlockAccessLists>(8);
            buildItem<BlockAuthorizationList>(9);
            for(uint i = 10; i < m_items.size(); i++) buildItem<BlockField>(i);
            deleteChildren();
            break;
        default: //Legacy/EIP-155 (9 fields)
            //nonce, gas_price, gas_limit, to, value, data, v, signature_r, signature_s
            buildAllItems<BlockField>();
            break;
    }
}