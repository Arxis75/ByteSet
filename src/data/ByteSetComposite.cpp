#include <data/ByteSetComposite.h>

IByteSetContainer* ByteSetComposite::makeChild(bool is_composite) {
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

void ByteSetComposite::RLPparse(ByteSet<8> &b)
{
    while(b.byteSize()) {
        ByteSet payload = b.RLPparse();
        auto child = makeChild(payload.getRLPType() == RLPType::LIST);
        child->RLPparse(payload);
        push_back(child);
    }
}

const ByteSet<8> ByteSetComposite::RLPserialize() const
{
    ByteSet<8> rlp;
    for(uint64_t i=0; i<m_children.size(); i++) {
        rlp.push_back(m_children[i]->RLPserialize());
    }

    if (typeid(*this) == typeid(ByteSetComposite))  //Avoid serializing the concrete caller
        rlp =  rlp.RLPserialize(true);
    return rlp;
}

void ByteSetComposite::DumpChildren() const
{
    for(int i=0;i<m_children.size();i++) {
        cout << hex << m_children[i].get() << " (";
        if(!m_children[i]->getComposite())
            cout << "F)";
        else {
            cout << "C:" << dec << dynamic_cast<const ByteSetComposite*>(m_children[i].get())->getChildrenCount() << ")";
        }
        cout << " ";
    }
    cout << endl;
}