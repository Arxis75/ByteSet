#include <data/ByteSetComposite.h>

IByteSetContainer* ByteSetComposite::newChild(bool is_composite) {
    IByteSetContainer* child;
    if(is_composite)
        child = new ByteSetComposite();
    else
        child = new ByteSetField();
    child->setParent(this);
    return child;
}

/*std::unique_ptr<const IByteSetContainer> ByteSetComposite::clone() const { 
    auto copy = std::unique_ptr<ByteSetComposite>(new ByteSetComposite());
    for(const auto& child : m_children)
        copy->m_children.push_back(child->clone());
    return copy;
}*/

void ByteSetComposite::deleteChildren() {
    while(m_children.size()) {
        DumpChildren();
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

void ByteSetComposite::RLPparse(ByteSet<8> &b)
{
    while(b.byteSize()) {
        ByteSet payload = b.RLPparse();
        IByteSetContainer* child = newChild(payload.getRLPType() == RLPType::LIST); //polymorphic
        child->RLPparse(payload);
        push_back(child);   //implicit call of unique_ptr ctor
    }
}

const ByteSet<8> ByteSetComposite::RLPserialize() const
{
    ByteSet<8> rlp;
    for(uint64_t i=0; i<m_children.size(); i++) {
        if(m_children[i])
            rlp.push_back(m_children[i]->RLPserialize());
    }

    if (typeid(*this) == typeid(ByteSetComposite))  //Avoid serializing the concrete caller
        rlp =  rlp.RLPserialize(true);
    return rlp;
}

template<typename T>
const T* ByteSetComposite::getChildrenAs(uint children_index) {
    T* children = nullptr;
    if(auto nest = dynamic_cast<const ByteSetComposite*>(getItem(0)); nest && nest->getComposite())
        children = nest->getItem(children_index);
    return children;
}
template<typename T>
T* ByteSetComposite::newChildrenAs(uint children_index) {
    T* list = nullptr;
    if(auto nest = dynamic_cast<const ByteSetComposite*>(getItem(0)); nest && nest->getComposite())
        if(auto children = const_cast<ByteSetComposite*>(nest)->takeItem(children_index); children) {
            list = new T();
            list->push_back(children.release());
        }
    return list;
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