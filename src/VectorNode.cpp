#include <ByteSet/VectorNode.h>
#include <ByteSet/Tools.h>

IComponent* VectorNode::pop_front() {
    if(getChildrenCount()) {
        const IComponent* child = m_children[0].release();
        m_children.erase(m_children.begin());
        return const_cast<IComponent*>(child);
    }
    else
        return nullptr;
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