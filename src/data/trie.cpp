#include <data/trie.h>

TrieNode::TrieNode()
    : m_children{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
    , m_value()
{ }

void TrieNode::update(const ByteSet<4> &key, const ByteSet<> &value) {
    if(key.getNbElements()) {
        if(!m_children[key[0]])
            m_children[key[0]] = new TrieNode();
        m_children[key[0]]->update(key.at(1, key.getNbElements() - 1), value);
    }
    else
        m_value = value;
}

const TrieNode* TrieNode::lookup(const ByteSet<4> &key) const {
    if(key.getNbElements() && m_children[key[0]])
        return m_children[key[0]]->lookup(key.at(1, key.getNbElements() - 1));
    else
        return this;
}

void TrieNode::del(const ByteSet<4> &key) {
    vector<std::pair<TrieNode*, uint8_t>> parents;
    TrieNode* target = this;
    if(key.getNbElements())
        //lookup the node thats ends the path represented by key
        do {
            TrieNode* children[16];
            memcpy(children, target->m_children, 16*sizeof(TrieNode*));
            //save the former vector size before push_back
            uint64_t index = parents.size();
            //push_back the pair <parent, path to the child leading to the target node>
            parents.push_back(make_pair(target, key.getElem(index)));
            target = children[key.getElem(index)];          //goes 1-level down
        } while(target && parents.size() < key.getNbElements());

    if(target) {
        //delete the found value
        target->m_value.clear();
        //checks the path to the deleted value in the parents
        while(parents.size()) {
            auto ppair = parents[parents.size()-1];
            TrieNode *pnode = ppair.first;
            //If a parent's child in the path has no child and no value,
            //it needs to be deleted as well as its reference in the parent
            //(first iteration parent's child = target)
            if( !pnode->m_children[ppair.second]->hasChildren() &&
                 pnode->m_children[ppair.second]->m_value.isEmpty() ) {
                delete pnode->m_children[ppair.second];
                pnode->m_children[ppair.second] = nullptr;
                parents.pop_back();                         //goes 1-level up 
            }
            else
                parents.clear();                            //no more cleanup allowed
        }
    }
}

//----------------------------------------------------------------------------------------------------------------

const CompressedTrieNode* CompressedTrieNode::lookup(const ByteSet<4> &key) const {
    return this;
}

void CompressedTrieNode::update(const ByteSet<4> &key, const ByteSet<> &value) {
}

void CompressedTrieNode::del(const ByteSet<4> &key) {

}