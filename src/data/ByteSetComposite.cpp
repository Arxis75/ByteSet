#include <data/ByteSetComposite.h>

ByteSetComposite::ByteSetComposite(const ByteSetComposite &other) {
    // optional optimization
    m_children.reserve(other.m_children.size());
    for (const auto& item : other.m_children) {
        if(item) {
            // std::unique_ptr<IByteSetContainer>
            auto tmp = item->clone();                                                   
            // wrap in unique_ptr<const IByteSetContainer>
            m_children.push_back(unique_ptr<const IByteSetContainer>(std::move(tmp)));
        }
        else
            m_children.emplace_back(nullptr); // preserve null entries
    }
}

IByteSetContainer* ByteSetComposite::makeChild(bool is_composite) {
    IByteSetContainer* child;
    if(is_composite)
        child = new ByteSetComposite();
    else
        child = new ByteSetField();
    child->setParent(this);
    return child;
}

/*void ByteSetComposite::deleteChildren() {
    while(m_children.size()) {
        //DumpChildren();
        auto cchild = dynamic_cast<const ByteSetComposite*>(m_children[m_children.size()-1].get());
        if(cchild) {
            if(cchild->getChildrenCount()) 
                const_cast<ByteSetComposite*>(cchild)->deleteChildren();
            else {
                delete cchild;
                m_children.pop_back();
            }
        }
        else {
            if(m_children[m_children.size()-1])         //test if nullptr in the vector
                delete m_children[m_children.size()-1].get();
            m_children.pop_back();
        }
    }
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