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

template<typename T>
std::unique_ptr<T> ByteSetComposite::takeChildrenAs(uint list_index) {
    auto list = make_unique<T>();
    auto list_children = const_cast<ByteSetComposite*>(dynamic_cast<const ByteSetComposite*>(getItem(0)))->takeItem(list_index);
    list->move_back(std::move(list_children));
    return list;
}

std::unique_ptr<ByteSetField> ByteSetComposite::takeField(uint field_index) {
    auto item = takeItem(field_index);
    ByteSetField* field = const_cast<ByteSetField*>(dynamic_cast<const ByteSetField*>(item.release()));
    return unique_ptr<ByteSetField>(field);
}

void ByteSetComposite::DumpChildren() const
{
    for(int i=0;i<m_children.size();i++) {
        cout << hex << m_children[i].get() << " (";
        if(!m_children[i])
            cout << "nullptr)";
        else {
            if(!m_children[i]->getComposite())
                cout << "F)";
            else {
                cout << "C:" << dec << dynamic_cast<const ByteSetComposite*>(m_children[i].get())->getChildrenCount() << ")";
            }
        }
        cout << " ";
    }
    cout << endl;
}