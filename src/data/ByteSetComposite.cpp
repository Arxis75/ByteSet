#include <data/ByteSetComposite.h>

shared_ptr<IByteSetContainer> ByteSetComposite::Factory(ByteSetComposite *parent, bool is_composite) {
    shared_ptr<IByteSetContainer> child;
    if(is_composite)
        child = std::shared_ptr<ByteSetComposite>(new ByteSetComposite(parent));
    else
        child = make_shared<ByteSetField>(parent);
    parent->push_back(child);
    return child;
}

void ByteSetComposite::RLPparse(ByteSet<8> &b)
{
    while(b.byteSize()) {
        ByteSet payload = b.RLPparse();
        auto child = Factory(this, payload.getRLPType() == RLPType::LIST);
        child->RLPparse(payload);
    }
}

ByteSet<8> ByteSetComposite::RLPserialize() const
{
    ByteSet<8> rlp;
    for(uint64_t i=0; i<m_children.size(); i++)
        rlp.push_back(m_children[i]->RLPserialize());

    if (typeid(*this) == typeid(ByteSetComposite))  //Avoid serializing the concrete caller
        rlp =  rlp.RLPserialize(true);
    return rlp;
}