#include <ByteSet/IComposite.h>
#include <ByteSet/ByteSet.h>

void IComposite::parse(RLPByteSet<BYTE> &b) {
    RLPByteSet<BYTE> payload;
    uint child_index = 0;
    b.pop_brackets();
    do {
        //cout << "Left payload to parse: " << b.asString() << endl;
        IComponent* child = newChild(child_index);
        if(b.byteSize()) {
            payload = b.pop_front_rlp();
            cout << "Parsing payload: " << payload.asString() << endl << endl;
            child->parse(payload);
        }
        else
            cout << "Parsing empty payload: " << endl << endl;
        addChild(child_index, child);
        child_index++;
    } while(b.byteSize()); 
}

const ByteSet<BYTE> IComposite::getValue() const {
    return IComposite::serialize().as<BYTE>();
}

const RLPByteSet<BYTE> IComposite::serialize() const {
    RLPByteSet<BYTE> result;
    uint child_index = 0;
    auto child = getChild(child_index);
    while(child) {
        result.push_back_rlp(child->serialize());
        child_index++;
        child = getChild(child_index);
    }
    return result;
}

void IComposite::print() const {
    uint child_index = 0;
    auto child = getChild(child_index);
    cout << "IComposite::print() for " << this << ":" << endl;
    while(child) {
        child->print();
        child_index++;
        child = getChild(child_index);
    }
    cout << endl;
}