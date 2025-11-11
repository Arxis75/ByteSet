#include <ByteSet/ByteSetTrie.h>

ByteSetTrieNode* ByteSetTrieNode::newLeaf(const ByteSet<NIBBLE>& key, const ByteSet<BYTE>& value) const {
    ByteSetTrieNode* new_leaf = new ByteSetTrieNode();
    new_leaf->m_key = key.withTerminator();
    new_leaf->m_value = value;
    return new_leaf;
}

ByteSetTrieNode* ByteSetTrieNode::createExtension(const ByteSet<NIBBLE>& key, bool do_mutate) {
    ByteSetTrieNode* extension = do_mutate ? this : new ByteSetTrieNode();
    extension->m_key = key.withoutTerminator();
    extension->m_value.clear();
    extension->m_children = unique_arr<unique_ptr<ByteSetTrieNode>>(1);
    return extension;
}

ByteSetTrieNode* ByteSetTrieNode::createBranch(bool do_mutate) {
    ByteSetTrieNode* branch = do_mutate ? this : new ByteSetTrieNode();
    branch->m_key.clear();
    branch->m_value.clear();
    branch->m_children = unique_arr<unique_ptr<ByteSetTrieNode>>(16);
    return branch;
}

const ByteSet<BYTE> ByteSetTrieNode::hash() const {
    ByteSet<BYTE> result;

    if(getType() == TYPE::LEAF) {
        result.push_back(m_key.HexToCompact().RLPSerialize(false));
        result.push_back(m_value.RLPSerialize(false));
        result = result.RLPSerialize(true);
        cout << "Leaf " << dec << " rlp = " << result.asString() << endl;
        result = result.keccak256();
        cout << "Leaf " << dec << " hash = " << result.asString() << endl;
    }
    else if(getType() == TYPE::EXTN) {
        result.push_back(m_key.HexToCompact());
        result.push_back(m_children[0]->hash().RLPSerialize(false));
        result = result.RLPSerialize(true);
        cout << "Extension " << dec << " rlp = " << result.asString() << endl;
        result = result.keccak256();
        cout << "Extension " << dec << " hash = " << result.asString() << endl;
    }
    else if(getType() == TYPE::BRAN) {
        for(uint i=0;i<m_children.size();i++)
            result.push_back(m_children[i] ? m_children[i]->hash().RLPSerialize(false) : ByteSet<BYTE>().RLPSerialize(false));
        result.push_back(m_value.RLPSerialize(false));
        result = result.RLPSerialize(true);
        cout << "Branch " << dec << " rlp = " << result.asString() << endl;
        result = result.keccak256();
        cout << "Branch " << dec << " hash = " << result.asString() << endl;
    }
    return result;
} 

ByteSet<NIBBLE> ByteSetTrieNode::extractCommonNibbles(ByteSet<NIBBLE> &key1, ByteSet<NIBBLE> &key2) const {
    ByteSet<NIBBLE> result;
    while(key1.getNbElements() && key2.getNbElements() && key1.getElem(0) == key2.getElem(0)) {
        result.push_back_elem(key1.pop_front_elem());
        key2.pop_front_elem();
    }
    return result;
}

ByteSetTrieNode* ByteSetTrieNode::disconnectChild(uint child_index) {
    ByteSetTrieNode* child = nullptr;
    if(getType() == TYPE::EXTN) child_index = 0;
    if(m_children[child_index]) {
        m_children[child_index]->setParent(nullptr);
        child = m_children[child_index].release();
    }
    return child;
}

void ByteSetTrieNode::connectChild(ByteSetTrieNode* child, uint child_index) {
    if(child) {
        if(getType() == TYPE::BRAN && child_index == 0x10) {
            //Child gets eaten by THIS branch node
            m_value = child->m_value;
            //delete child;
        }
        else {
            if(getType() == TYPE::EXTN) child_index = 0;
            assert(getType() != TYPE::LEAF && !m_children[child_index]);
            child->setParent(this);
            m_children[child_index].reset(child);
        }
    }
}

ByteSetTrieNode* ByteSetTrieNode::disconnectFromParent(uint index_in_parent) {
    if(ByteSetTrieNode* parent = const_cast<ByteSetTrieNode*>(dynamic_cast<const ByteSetTrieNode*>(getParent())); parent) {
        if(parent->getType() == TYPE::EXTN) index_in_parent = 0;
        parent->m_children[index_in_parent].release();
        setParent(nullptr);
    }
    return this;
}

void ByteSetTrieNode::connectToParent(ByteSetTrieNode* parent, uint index_in_parent) {
    if(parent) {
        if(parent->getType() == TYPE::BRAN && index_in_parent == 0x10) {
            //THIS gets eaten by the parent branch node
            parent->m_value = m_value;
            //delete this;
        }
        else {
            if(parent->getType() == TYPE::EXTN) index_in_parent = 0;
            assert(parent->getType() != TYPE::LEAF && !parent->m_children[index_in_parent]);
            setParent(parent);
            parent->m_children[index_in_parent].reset(this);
        }
    }
}

ByteSetTrieNode* ByteSetTrieNode::insert(ByteSetTrieNode* parent, uint index_in_parent, ByteSetTrieNode* child, uint child_index, TYPE type, const ByteSet<NIBBLE>& key, const ByteSet<BYTE>& value) {
    assert(type != TYPE::EMPTY);
    ByteSetTrieNode* node;
    if(type == TYPE::LEAF) {
        node = newLeaf(key, value);
    }
    else if(type == TYPE::EXTN) {
        node = createExtension(key);
        node->connectChild(child, child_index);
    }
    else if(type == TYPE::BRAN) {
        node = createBranch();
        node->connectChild(child, child_index);
    }
    node->connectToParent(parent, index_in_parent);
    return node;
}

void ByteSetTrieNode::store(ByteSet<NIBBLE> &key, const ByteSet<BYTE>& value) {
    ByteSet<NIBBLE> shared_nibbles, unshared_nibbles;
    switch(getType()) {
        case EMPTY:
            //First KV storage: EMPTY ROOT => LEAF
            m_key = key.withTerminator();
            m_value = value;
            break;
        case LEAF: {
            assert(key != m_key);   //no double insertion

            //prune key and m_key of their common nibbles.
            shared_nibbles = extractCommonNibbles(key, m_key);
            unshared_nibbles = key;
            
            //Save a copy of the pruned Leaf
            uint pruned_previous_leaf_index = m_key.pop_front_elem();   //a popped Terminator will be handled by connectToParent()
            auto pruned_previous_leaf = newLeaf(m_key, m_value);

            if(shared_nibbles.getNbElements()) {
                //Some common nibbles: mutate from LEAF => EXTENSION
                createExtension(shared_nibbles, true);

                //insert a BRANCH as child of this EXTENSION and reconnect the previous LEAF
                auto branch = insert(this, 0, pruned_previous_leaf, pruned_previous_leaf_index, BRAN);

                //Continue the key parsing
                branch->store(key, value);
            }
            else {
                //No common nibbles: mutate from LEAF => BRANCH
                createBranch(true);
                
                //Reconnect the previous LEAF:
                pruned_previous_leaf->connectToParent(this, pruned_previous_leaf_index);

                //Continue the key parsing
                store(key, value);
            }
            break;
        }
        case EXTN: {
            //Prune key and m_key of their common nibbles.
            shared_nibbles = extractCommonNibbles(key, m_key);
            //unshared are what's left of m_key
            unshared_nibbles = m_key;  
            //Shrink the existing EXTN node's m_key to only shared_nibbles 
            m_key = shared_nibbles;    

            if(!unshared_nibbles.getNbElements()) {
                //Continue the key parsing under the existing child branch
                m_children[0]->store(key, value);
            }
            else if(!shared_nibbles.getNbElements())
            {
                //Disconnect the previous child branch before mutation
                auto previous_branch = disconnectChild(0);
                
                //No common nibbles: mutate from EXTN => BRANCH
                createBranch(true);

                //Insert a new child EXTENSION between the mutated branch (parent) and the previous branch (child)
                uint64_t unshared_ext_index = unshared_nibbles.pop_front_elem();
                auto unshared_ext = insert(this, unshared_ext_index, previous_branch, 0, EXTN, unshared_nibbles);

                //Continue the key parsing
                store(key, value);
            }
            else {
                //Disconnect the previous child branch
                auto previous_branch = disconnectChild(0);

                //Insert a new BRANCH child between THIS Extension and the previous BRANCH
                uint64_t new_branch_child_index = unshared_nibbles.pop_front_elem();   //consume 1 NIBBLE taken by the new branch
                auto new_branch = insert(this, 0, previous_branch, new_branch_child_index, BRAN);

                if(unshared_nibbles.getNbElements()) {
                    //Disconnect the newly created branch from previous_branch (its child)
                    new_branch->disconnectChild(new_branch_child_index);

                    //Insert a new child EXTENSION between the newly created branch (parent) and the previous branch (child)
                    auto unshared_ext = insert(new_branch, new_branch_child_index, previous_branch, 0, EXTN, unshared_nibbles);
                }
                //Continue the key parsing under the new branch
                new_branch->store(key, value);
            }
            break;
        }
        case BRAN: {
            if(!key.getNbElements())
                m_value = value;
            else {
                uint64_t index = key.pop_front_elem();

                if(!m_children[index]) {
                    //The key's first nibble placeholder is empty => new LEAF
                    auto new_leaf = newLeaf(key, value);
                    new_leaf->setParent(this);
                    m_children[index].reset(new_leaf);
                }
                else
                    //Continue the key parsing in the proper placeholder
                    m_children[index]->store(key, value);
            }
            break;
        }
        default:
            break;
    }
}

void ByteSetTrieNode::DumpChildren() const {
    string type;
    auto toString = [](TYPE t) -> std::string {
        switch (t) {
        case TYPE::EMPTY:    return "EMPTY";
        case TYPE::LEAF:    return "LEAF";
        case TYPE::EXTN:    return "EXTN";
        case TYPE::BRAN:    return "BRAN";
        default:            return "Unknown";
        }
    };

    if(getType() == TYPE::EXTN || getType() == TYPE::BRAN || !getParent()) {
        cout << "Dumping " << toString(getType()) << " Node " << this << "(k:" << m_key.withoutTerminator().asString() << (m_key.hasTerminator() ? "10" : "") << " v:" << (m_value.getNbElements() ? "yes" : "no" ) <<" is " << toString(getType()) << ") :" << endl;
        for(int i=0;i<m_children.size();i++) {
            if(m_children[i])
                cout << m_children[i].get() << "(k:" << m_children[i]->m_key.withoutTerminator().asString() << (m_children[i]->m_key.hasTerminator() ? "10" : "") << " v:" << (m_children[i]->m_value.getNbElements() ? "yes" : "no" ) <<" is " << toString(m_children[i]->getType()) << ") ";
            else
                cout << m_children[i].get() << "() ";
        }
        cout << endl << endl;
        for(int i=0;i<m_children.size();i++) {
            if(m_children[i])
                m_children[i]->DumpChildren();
        }
    }
}