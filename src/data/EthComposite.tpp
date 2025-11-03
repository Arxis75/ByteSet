#include <data/EthComposite.h>

template<typename T>
void EthContainer::buildItem(uint64_t index) {
    if(auto nest = const_cast<ByteSetComposite*>(dynamic_cast<const ByteSetComposite*>(getChildAt(0))); nest) {
        if(!m_items)
            m_items = unique_arr<unique_ptr<IByteSetContainer>>(nest->getChildrenCount());
        m_items[index] = make_unique<T>();
        nest->moveChildAt_To(index, m_items[index]);
        const_cast<T*>(dynamic_cast<const T*>(m_items[index].get()))->buildStructure();
    }
}

template<typename T>
void EthContainer::buildAllItems() {
    if(auto nest = const_cast<ByteSetComposite*>(dynamic_cast<const ByteSetComposite*>(getChildAt(0))); nest) {
        if(!m_items)
            m_items = unique_arr<unique_ptr<IByteSetContainer>>(nest->getChildrenCount());
        for(uint i = 0; i<m_items.size(); i++)
            buildItem<T>(i);
        deleteChildren();
    }
}

const ByteSet<8> EthContainer::RLPserialize() const {
    ByteSet<8> rlp;
    for(uint64_t i=0; i<m_items.size(); i++) {
        if(m_items[i])
            rlp.push_back(m_items[i]->RLPserialize());
    }               
    return rlp =  rlp.RLPserialize(true);
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