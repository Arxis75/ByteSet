#include <ByteSet/IComponent.h>
#include <ByteSet/ByteSet.h>

void IComposite::parse(ByteSet<BYTE> &b) {
    uint child_index = 0;
    while(b.byteSize()) {
        bool has_list_header = b.hasRLPListHeader();
        ByteSet payload = b.parse();
        if(IComponent* child = newChild(child_index); child) {
            if(auto composite_child = dynamic_cast<IComposite*>(child); composite_child && !has_list_header) { 
                //If composite without RLP List header => typed composite
                composite_child->setTyped(payload.pop_front_elem());
                payload = payload.parse();
            }
            child->parse(payload);
            addChild(child_index, child);
            child_index++;
        }
        else
            break;
    }
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
    result = result.RLPSerialize(true);
    if(uint typed = getTyped(); typed) {
        result.push_front_elem(typed);
        result = result.RLPSerialize(false);
    }
    return result;
}

void IComposite::print() const {
    uint child_index = 0;
    auto child = getChild(child_index);
    while(child) {
        child->print();
        child_index++;
        child = getChild(child_index);
    }
}