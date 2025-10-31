#include <data/EthComposite.h>

void EthContainer::buildStructure() {
    auto nest = getItem(0);
    if(nest && nest->getComposite()) {
        auto raw_nest = const_cast<ByteSetComposite*>(dynamic_cast<const ByteSetComposite*>(nest));
        uint64_t nb_fields = raw_nest->getChildrenCount();
        m_field = unique_arr<unique_ptr<const IByteSetContainer>>(nb_fields);
        for(int i =0;i<nb_fields;i++) {
            if(auto item = raw_nest->takeItem(i); item)
                m_field[i] = std::move(item);
        }
        deleteChildren();
    }
}

const ByteSet<8> EthContainer::RLPserialize() const {
    ByteSet<8> rlp;
    for(uint64_t i=0; i<m_field.size(); i++) {
        if(m_field[i])
            rlp.push_back(m_field[i]->RLPserialize());
    }               
    return rlp =  rlp.RLPserialize(true);
}

void EthContainer::DumpFields() const
{
    if(m_field.size())
        for(int i=0;i<m_field.size();i++) {
            cout << hex << m_field[i].get() << " (";
            if(!m_field[i])
                cout << "nullptr";
            else {
                if(!m_field[i]->getComposite())
                    cout << "F";
                else {
                    cout << "C:" << dec << dynamic_cast<const ByteSetComposite*>(m_field[i].get())->getChildrenCount();
                }
                cout << " P:" << hex << m_field[i]->getParent();
            }
            cout << ") ";
        }
    else
        cout << "None ()";
    cout << endl;
}