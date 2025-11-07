#include <ByteSet/ByteSetComposite.h>
#include <System.h>

template<typename T>
void ByteSetComposite::create(ByteSet<BYTE> &b) {
        bool has_list_header = b.hasRLPListHeader();
        ByteSet payload = b.RLPparse();
        T* item = new T();
        if(auto typed_item = dynamic_cast<TypedByteSetComposite*>(item); typed_item && !has_list_header) { 
            typed_item->setType(payload.pop_front_elem());
            payload = payload.RLPparse();
        }
        item->RLPparse(payload);
        item->setParent(this);
        push_back(item);  //implicit call of unique_ptr ctor
}

void ByteSetComposite::deleteChildren() {
    while(m_children.size()) {
        //DumpChildren();
        unique_ptr<const IByteSetContainer> uchild = std::move(m_children[m_children.size()-1]);
        if(uchild) {
            auto cchild = dynamic_cast<const ByteSetComposite*>(uchild.get());
            if(cchild && cchild->getChildrenCount()) 
                const_cast<ByteSetComposite*>(cchild)->deleteChildren();
            else {
                //delete cchild;                                    //delete handled solely by unique_ptr
                m_children.pop_back();
            }
        }
        else {
            if(m_children[m_children.size()-1]) {                   //test if nullptr in the vector
                //delete m_children[m_children.size()-1].get();     //delete handled solely by unique_ptr
            }
            m_children.pop_back();
        }
    }
}

const ByteSet<BYTE> ByteSetComposite::RLPserialize() const
{
    ByteSet<BYTE> rlp;
    for(uint64_t i=0; i<m_children.size(); i++) {
        if(m_children[i])
            rlp.push_back(m_children[i]->RLPserialize());
    }
    rlp =  rlp.RLPserialize(true);
    return rlp;
}

const ByteSet<BYTE> TypedByteSetComposite::RLPserialize() const {
    ByteSet result = ByteSetComposite::RLPserialize();
    if(uint8_t type = getType(); type) {
        result.push_front_elem(type);
        result = result.RLPserialize(false);
    }
    return result;
}

void ByteSetComposite::DumpChildren() const
{
    if(m_children.size())
        for(int i=0;i<m_children.size();i++) {
            cout << hex << m_children[i].get() << " (";
            if(!m_children[i])
                cout << "nullptr";
            else {
                if(!m_children[i]->getComposite())
                    cout << "F";
                else {
                    cout << "C:" << dec << dynamic_cast<const ByteSetComposite*>(m_children[i].get())->getChildrenCount();
                }
                cout << " P:" << hex << m_children[i]->getParent();
            }
            cout << ") ";
        }
    else
        cout << "None ()";
    cout << endl;
}