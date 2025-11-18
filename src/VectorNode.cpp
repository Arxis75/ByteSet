#include <ByteSet/VectorNode.h>
#include <ByteSet/Tools.h>

void VectorNode::clear() {
    while(m_children.size()) {
        unique_ptr<const IComponent> uchild = std::move(m_children[m_children.size()-1]);
        if(uchild) {
            auto cchild = dynamic_cast<const VectorNode*>(uchild.get());
            if(cchild && cchild->getChildrenCount()) 
                const_cast<VectorNode*>(cchild)->clear();
            else {
                //delete cchild;                                    //delete handled solely by unique_ptr
                m_children.pop_back();
                //dumpChildren();
            }
        }
        else {
            if(m_children[m_children.size()-1]) {                   //test if nullptr in the vector
                //delete m_children[m_children.size()-1].get();     //delete handled solely by unique_ptr
            }
            m_children.pop_back();
            //dumpChildren();
        }
    }
}

inline void VectorNode::print() const
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
                    cout << "C:" << dec << dynamic_cast<const VectorNode*>(m_children[i].get())->getChildrenCount();
                }
                cout << " P:" << hex << m_children[i]->getParent();
            }
            cout << ") ";
        }
    else
        cout << "None ()";
    cout << endl;
}