#include <ByteSet/IComposite.h>
#include <ByteSet/ByteSet.h>

void IComposite::parse(ByteSet<BYTE> &b) {
    ByteSet payload;
    uint child_index = 0;
    b.pop_rlp(true);
    while(b.byteSize()) {
        cout << "Left payload to parse: " << b.asString() << endl << endl;
        IComponent* child = newChild(child_index);
        payload = b.pop_rlp();
        cout << "Parsing payload: " << payload.asString() << endl << endl;
        child->parse(payload);
        addChild(child_index, child);
        child_index++;
    }
}

const ByteSet<BYTE> IComposite::getValue() const {
    return IComposite::serialize();
}

const ByteSet<BYTE> IComposite::serialize() const {
    ByteSet<BYTE> result;
    uint child_index = 0;
    auto child = getChild(child_index);
    while(child) {
        result.push_back(child->serialize());
        child_index++;
        child = getChild(child_index);
    }
    return result.RLPSerialize(true);
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