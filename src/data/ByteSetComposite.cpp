#include <data/ByteSetComposite.h>

void ByteSetList::RLPparse(ByteSet<8> &b)
{
    bool is_list = false;
    while(b.byteSize()) {
        //cout << "is_list = " << int(is_list) << endl;
        //cout << "f = " << hex << parsed_b.asString() << endl;
        //cout << "b = " << hex << b.asString() << endl;
        //cout << "----------------------------------" << endl;
        shared_ptr<IByteSetContainer> child;
        bool is_list = b.hasRLPListHeader();
        ByteSet inside = b.RLPparse();
        if(is_list)
            child = make_shared<ByteSetList>();
        else
            child = make_shared<ByteSetField>();
        child->RLPparse(inside);
        push_back(child);
    }
}

ByteSet<8> ByteSetList::RLPserialize() const
{
    ByteSet<8> rlp;
    for(uint64_t i=0; i<m_children.size(); i++) {
        ByteSet<8> y = m_children[i]->RLPserialize();
        if(m_children[i]->getComposite())
            cout << "composite child = " << hex << y.asString() << endl;
        else
            cout << "leaf child = " << hex << y.asString() << endl;
        rlp.push_back(y);
    }
    ByteSet<8> x =  rlp.RLPserialize(true);
    cout << "composite = " << hex << x.asString() << endl;
    return x;
}
