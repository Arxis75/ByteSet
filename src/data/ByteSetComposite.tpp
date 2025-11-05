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

void ByteSetComposite::moveChildAt_To(uint64_t child_index, unique_ptr<IByteSetContainer>& target) {
    assert(target);
    if(auto uptr = takeChildAt(child_index); uptr) {
        assert(uptr->isComposite() == target->isComposite());
        target->push_back(uptr.release());
    }
}

uint64_t ByteSetComposite::RLPparseTypedChildAt(uint64_t child_index) {
    uint64_t type = 0;
    if(auto f = dynamic_cast<const ByteSetField*>(getChildAt(child_index)); f) {
        ByteSetComposite* typed_composite = new ByteSetComposite;;
        typed_composite->setParent(f->getParent());
        ByteSet<8> typed_child = f->getValue();
        type = typed_child.pop_front_elem();  //assume here type = 8 bits
        typed_composite->RLPparse(typed_child);
        if(auto ptr = dynamic_cast<const ByteSetComposite*>(typed_composite->getChildAt(0)); ptr)
            m_children[child_index].reset(ptr);          //skip the list wrapper (nest)
        else
            delete typed_composite;
    }
    return type;
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

void ByteSetField::push_back(const IByteSetContainer *b)
{
    if(auto f = dynamic_cast<const ByteSetField*>(b); f)
        if(auto v = const_cast<ByteSetField*>(f)->takeValue(); v)
            m_value.reset(v.release());
}