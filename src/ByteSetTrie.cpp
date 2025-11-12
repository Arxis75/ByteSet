#include <ByteSet/ByteSetTrie.h>

ByteSetTrieNode* ByteSetTrieNode::createLeaf(const ByteSet<NIBBLE>& key, const ByteSet<BYTE>& value, bool do_mutate) {
    ByteSetTrieNode* leaf = do_mutate ? this : new ByteSetTrieNode();
    leaf->m_key = key.withTerminator();
    leaf->m_value = value;
    leaf->m_children.release();
    return leaf;
}

ByteSetTrieNode* ByteSetTrieNode::createExtension(const ByteSet<NIBBLE>& key, bool do_mutate) {
    ByteSetTrieNode* extension = do_mutate ? this : new ByteSetTrieNode();
    extension->m_key = key.withoutTerminator();
    extension->m_value.clear();
    extension->m_children.release();
    extension->m_children = unique_arr<unique_ptr<ByteSetTrieNode>>(1);
    return extension;
}

ByteSetTrieNode* ByteSetTrieNode::createBranch(const ByteSet<BYTE>& value, bool do_mutate) {
    ByteSetTrieNode* branch = do_mutate ? this : new ByteSetTrieNode();
    branch->m_key.clear();
    branch->m_value = value;
    branch->m_children.release();
    branch->m_children = unique_arr<unique_ptr<ByteSetTrieNode>>(16);
    return branch;
}

const ByteSet<BYTE> ByteSetTrieNode::hash() const {
    ByteSet<BYTE> result;

    if(getType() == TYPE::EMPTY) {
        result = result.RLPSerialize(false);
        cout << "Empty node " << dec << " rlp = " << result.asString() << endl;
        if(result.byteSize() >= 32 || !getParent())
            result = result.keccak256();
        cout << "Empty node " << dec << " hash = " << result.asString() << endl;
    }
    else if(getType() == TYPE::LEAF) {
        result.push_back(m_key.HexToCompact().RLPSerialize(false));
        result.push_back(m_value.RLPSerialize(false));
        result = result.RLPSerialize(true);
        cout << "Leaf " << dec << " rlp = " << result.asString() << endl;
        if(result.byteSize() >= 32 || !getParent())
            result = result.keccak256();
        cout << "Leaf " << dec << " hash = " << result.asString() << endl;
    }
    else if(getType() == TYPE::EXTN) {
        result.push_back(m_key.HexToCompact().RLPSerialize(false));
        ByteSet<BYTE> h(m_children[0]->hash());
        result.push_back(h.byteSize() < 32 ? h : h.RLPSerialize(false));    // < 32 Bytes => Value node, else Hash Node
        result = result.RLPSerialize(true);
        cout << "Extension " << dec << " rlp = " << result.asString() << endl;
        if(result.byteSize() >= 32 || !getParent())
            result = result.keccak256();
        cout << "Extension " << dec << " hash = " << result.asString() << endl;
    }
    else if(getType() == TYPE::BRAN) {
        for(uint i=0;i<m_children.size();i++) {
            if(m_children[i]) {
                ByteSet<BYTE> h(m_children[i]->hash());
                result.push_back(h.byteSize() < 32 ? h : h.RLPSerialize(false));
            }
            else
                result.push_back_elem(0x80);
        }
        result.push_back(m_value.RLPSerialize(false));
        result = result.RLPSerialize(true);
        cout << "Branch " << dec << " rlp = " << result.asString() << endl;
        if(result.byteSize() >= 32 || !getParent())
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

ByteSetTrieNode* ByteSetTrieNode::disconnectChild(uint child_index) {
    ByteSetTrieNode* child = nullptr;
    if(getType() == TYPE::EXTN) child_index = 0;
    if(m_children[child_index]) {
        m_children[child_index]->setParent(nullptr);
        child = m_children[child_index].release();
    }
    return child;
}

ByteSetTrieNode* ByteSetTrieNode::insert(ByteSetTrieNode* parent, uint index_in_parent, ByteSetTrieNode* child, uint child_index, TYPE type, const ByteSet<NIBBLE>& key, const ByteSet<BYTE>& value) {
    assert(parent && child);
    ByteSetTrieNode* node;
    if(type == TYPE::LEAF) {
        node = createLeaf(key, value);
    }
    else if(type == TYPE::EXTN) {
        node = createExtension(key);
        node->connectChild(child, child_index);
    }
    else if(type == TYPE::BRAN) {
        node = createBranch(value);
        node->connectChild(child, child_index);
    }
    parent->connectChild(node, index_in_parent);
    return node;
}

void ByteSetTrieNode::storeKV(ByteSet<NIBBLE> &key, const ByteSet<BYTE>& value) {
    ByteSet<NIBBLE> shared_nibbles, unshared_nibbles;
    switch(getType()) {
        case EMPTY:
            //First KV storage: EMPTY ROOT => LEAF
            m_key = key.withTerminator();
            m_value = value;
            break;
        case LEAF: {
            if(!value.getNbElements())
                // (key,value) erasure
                wipeK();
            else if(key == m_key.withoutTerminator())
                //LEAF value update
                m_value = value;
            else {
                //prune key and m_key of their common nibbles.
                shared_nibbles = extractCommonNibbles(key, m_key);
                unshared_nibbles = key;
                
                //Save a copy of the pruned Leaf
                ByteSetTrieNode* pruned_previous_leaf = nullptr;
                ByteSet<BYTE> previous_value = m_value;
                uint pruned_previous_leaf_index = -1;
                if(!m_key.isTerminator()) {
                    pruned_previous_leaf_index = m_key.pop_front_elem();
                    pruned_previous_leaf = createLeaf(m_key, m_value);
                }

                if(shared_nibbles.getNbElements()) {
                    //Some common nibbles: mutate from LEAF => EXTENSION
                    createExtension(shared_nibbles, true);

                    ByteSetTrieNode* branch = nullptr;
                    if(pruned_previous_leaf)
                        //insert a BRANCH as child of this EXTENSION and reconnect the pruned LEAF
                        branch = insert(this, 0, pruned_previous_leaf, pruned_previous_leaf_index, BRAN);
                    else {
                        //create a sub-BRANCH with the former LEAF m_value
                        branch = createBranch(previous_value);
                        //and connect it to THIS EXTENSION
                        this->connectChild(branch, 0);
                    }

                    //Continue the key parsing
                    branch->storeKV(key, value);
                }
                else {
                    //No common nibbles: mutate from LEAF => BRANCH
                    createBranch(EMPTY_VALUE, true);
                    
                    if(pruned_previous_leaf)
                        //Reconnect the pruned LEAF
                        this->connectChild(pruned_previous_leaf, pruned_previous_leaf_index);
                    else
                        //Stores the previous LEAF value
                        m_value = previous_value;

                    //Continue the key parsing
                    storeKV(key, value);
                }
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
                m_children[0]->storeKV(key, value);
            }
            else if(!shared_nibbles.getNbElements())
            {
                //Disconnect the previous child branch before mutation
                auto previous_branch = disconnectChild(0);

                //No common nibbles: mutate from EXTN => BRANCH
                createBranch(EMPTY_VALUE, true);
                
                uint64_t unshared_index = unshared_nibbles.pop_front_elem();
                if(unshared_nibbles.getNbElements())
                    //Insert a new child EXTENSION between the mutated branch (parent) and the previous branch (child)
                    auto unshared_ext = insert(this, unshared_index, previous_branch, 0, EXTN, unshared_nibbles);
                else
                    //Reconnect the previous child branch
                    connectChild(previous_branch, unshared_index);

                //Continue the key parsing
                storeKV(key, value);
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
                new_branch->storeKV(key, value);
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
                    auto new_leaf = createLeaf(key, value);
                    new_leaf->setParent(this);
                    m_children[index].reset(new_leaf);
                }
                else
                    //Continue the key parsing in the proper placeholder
                    m_children[index]->storeKV(key, value);
            }
            break;
        }
        default:
            break;
    }
}

int ByteSetTrieNode::getChildIndex(const ByteSetTrieNode* child) const {
    int index = -1;
    for(uint i=0;i<m_children.size();i++) {
        if(m_children[i].get() == child) {
            index = i;
            break;
        }
    }
    return index;
}

int ByteSetTrieNode::getFirstChildIndex() const
{
    int first_child_found_index = -1;
    for(uint i=0;i<m_children.size();i++) {
        if(m_children[i]) {
            first_child_found_index = i;
            break;
        }
    }
    return first_child_found_index;
}

uint64_t ByteSetTrieNode::getChildrenCount() const {
    int counter = 0;
    for(uint i=0;i<m_children.size();i++) {
        if(m_children[i])
            counter++;
    }
    return counter;
}

void ByteSetTrieNode::wipeK(uint index) {
    auto parent = const_cast<ByteSetTrieNode*>(dynamic_cast<const ByteSetTrieNode*>(getParent()));
    switch(getType()) {
        case EMPTY:
            break;
        case LEAF: {
                if(parent)
                    parent->wipeK(parent->getChildIndex(this));
                else {
                    //Root erasure
                    m_key.clear();
                    m_value.clear();
                }
            break;
        }
        case EXTN: {
                if(m_children[index])
                    m_children[index].release();
                if(parent)
                    //Propagates emptiness to the parent
                    parent->wipeK(parent->getChildIndex(this));
                else
                    //THIS becomes an EMPTY Root
                    createLeaf(EMPTY_KEY, EMPTY_VALUE, true);
            break;
        }
        case BRAN: {
                if(m_children[index])
                    m_children[index].release();
                uint nb_children = getChildrenCount();
                if(!nb_children) {
                    if(m_value.getNbElements()) {
                        if(parent && parent->getType() == TYPE::EXTN)
                            //Parent EXTENSION mutate to LEAF to integrate the m_value of the former BRANCH
                            parent->createLeaf(parent->m_key, m_value, true);   //Wipes THIS
                        else
                            //This BRANCH mutates to LEAF to integrate the m_value of the former BRANCH
                            createLeaf(EMPTY_KEY, m_value, true);
                    }
                    else if(parent)
                        //Propagates emptiness to the parent
                        parent->wipeK(parent->getChildIndex(this));
                    else
                        //THIS becomes an EMPTY Root
                        createLeaf(EMPTY_KEY, EMPTY_VALUE, true);
                }
                else if(nb_children == 1 && !m_value.getNbElements()) {
                    int only_child_index = getFirstChildIndex();
                    ByteSetTrieNode* only_child = m_children[only_child_index].release();
                    
                    ByteSet<NIBBLE> new_key(only_child_index);
                    if(only_child->getType() != TYPE::BRAN)
                        new_key.push_back(only_child->m_key);

                    if(only_child->getType() == TYPE::LEAF) {
                        //This BRANCH mutates to LEAF to integrate the m_key/m_value of only_child
                        createLeaf(new_key, only_child->m_value, true);              
                    }
                    else if(only_child->getType() == TYPE::EXTN) {
                        auto child_extension_child = only_child->m_children[0].release();   //Disconnect only_child's child
                        //This BRANCH mutates to EXTENSION to integrate the m_key of only_child
                        createExtension(new_key, true);
                        connectChild(child_extension_child, 0);
                    }
                    else {
                        //This BRANCH mutates to EXTENSION with only_child_index as key
                        createExtension(new_key, true);
                        connectChild(only_child, 0);
                    }
                } 
            break;
        }
        default:
            break;
    }
}

void ByteSetTrieNode::dumpChildren() const {
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
                m_children[i]->dumpChildren();
        }
    }
}