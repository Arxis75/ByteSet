#include <ByteSet/IComponent.h>
#include <ByteSet/ByteSet.h>

void IComposite::parse(ByteSet<BYTE> &b) {
    uint child_index = 0;
    while(b.byteSize()) {
        bool has_list_header = b.hasRLPListHeader();
        ByteSet payload = b.parse();
        if(IComponent* child = newChild(child_index); child) {
            if(child->getComposite() && !has_list_header) { 
                //If composite without RLP List header => typed composite
                setTyped(payload.pop_front_elem());
                payload = payload.parse();
            }
            child->parse(payload);
            ByteSet<NIBBLE> key(child_index);
            key.setRLPType(RLPType::INT);
            addChild(child, key.RLPSerialize(false));
            child_index++;
        }
        else
            break;
    }
}

const ByteSet<BYTE> IComposite::serialize() const {
    ByteSet<BYTE> result;
    for(uint child_index = 0; child_index < getChildrenContainerSize(); child_index++) {
        if(auto child = getChild(child_index); child)
            result.push_back(child->serialize());
        else
            result.push_back_elem(0x80);
    }
    result = result.RLPSerialize(true);
    if(uint typed = getTyped(); typed) {
        result.push_front_elem(typed);
        result = result.RLPSerialize(false);
    }
    return result;
}

void IComposite::print() const {
    for(uint child_index = 0; child_index < getChildrenContainerSize(); child_index++) {
        if(auto child = getChild(child_index); child)
            child->print();
        else
            IComponent::print();
    }
}