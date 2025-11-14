#include <ByteSet/Composite.h>
#include <ByteSet/Tools.h>

template<typename T>
void RLPListComposite::create(ByteSet<BYTE> &b) {
        bool has_list_header = b.hasRLPListHeader();
        ByteSet payload = b.RLPparse();
        T* item = new T();
        if(auto typed_item = dynamic_cast<TypedRLPListComposite*>(item); typed_item && !has_list_header) { 
            typed_item->setType(payload.pop_front_elem());
            payload = payload.RLPparse();
        }
        item->RLPparse(payload);
        push_back(item);  //implicit call of unique_ptr ctor
}

inline const ByteSet<BYTE> RLPListComposite::RLPSerialize() const
{
    ByteSet<BYTE> rlp;
    for(uint64_t i=0; i<m_children.size(); i++) {
        if(m_children[i])
            rlp.push_back(m_children[i]->RLPSerialize());
    }
    rlp =  rlp.RLPSerialize(true);
    return rlp;
}

inline const ByteSet<BYTE> TypedRLPListComposite::RLPSerialize() const {
    ByteSet result = RLPListComposite::RLPSerialize();
    if(uint8_t type = getType(); type) {
        result.push_front_elem(type);
        result = result.RLPSerialize(false);
    }
    return result;
}

inline void RLPListComposite::reset() {
    while(m_children.size()) {
        unique_ptr<const IRLPListComponent> uchild = std::move(m_children[m_children.size()-1]);
        if(uchild) {
            auto cchild = dynamic_cast<const RLPListComposite*>(uchild.get());
            if(cchild && cchild->getChildrenCount()) 
                const_cast<RLPListComposite*>(cchild)->reset();
            else {
                //delete cchild;                                    //delete handled solely by unique_ptr
                m_children.pop_back();
                //printChildren();
            }
        }
        else {
            if(m_children[m_children.size()-1]) {                   //test if nullptr in the vector
                //delete m_children[m_children.size()-1].get();     //delete handled solely by unique_ptr
            }
            m_children.pop_back();
            //printChildren();
        }
    }
}

inline void RLPListComposite::printChildren() const
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
                    cout << "C:" << dec << dynamic_cast<const RLPListComposite*>(m_children[i].get())->getChildrenCount();
                }
                cout << " P:" << hex << m_children[i]->getParent();
            }
            cout << ") ";
        }
    else
        cout << "None ()";
    cout << endl;
}