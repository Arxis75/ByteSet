#pragma once
#include <ByteSet/TrieNode.h>

template <typename T>
void TrieNode<T>::addChild(uint child_index, IComponent *child) {
    auto value = dynamic_cast<T*>(child);
    assert(value);
    child->setParent(this);
    ByteSet<NIBBLE> key(child_index ? ByteSet<BYTE>(child_index).serialize().as<NIBBLE>() : ByteSet<BYTE>::EMPTY.serialize().as<NIBBLE>());
    storeKV(key, value);
}

template <typename T>
IComponent* TrieNode<T>::removeChild(uint child_index) {
    ByteSet<NIBBLE> key(child_index ? ByteSet<BYTE>(child_index).serialize().as<NIBBLE>() : ByteSet<BYTE>::EMPTY.serialize().as<NIBBLE>());
    //IComponent * child = get<T>(key);   //FIXME! the child will be deleted by the next line which does a m_value.reset()
    storeKV(key, nullptr);
    return nullptr; //return child;
}

/// @brief creates a new Leaf or tries to mutate "this" into a Leaf: mutation keeps the parent consistant.
/// WARNING:  If a new Object is created, the caller is in charge of assigning the right parent to the object.
///           A caller of this function should not assume that mutating "this" returns "this".
///           The function might merge the Leaf with the parent when possible, and then return
///           the merged PARENT.
///           Best practive is to always do "auto mutated_this = createLeaf(..., true);"
///           and use mutated_this as the final Leaf (not "this").
/// @param key: the key of the Leaf
/// @param value: the value of the Leaf
/// @param do_mutate: if true, tries to mutate "this" into a leaf: might cause merge with the parent
/// @return the Leaf pointer, which might differ from "this" even in a mutation scenario.
template <typename T>
TrieNode<T>* TrieNode<T>::createLeaf(const ByteSet<NIBBLE>& key, const T* value, bool do_mutate) {
    TrieNode* leaf = nullptr;
    if(do_mutate) {
        auto parent = const_cast<TrieNode*>(dynamic_cast<const TrieNode*>(getParent()));
        if(parent && parent->getType() == TYPE::EXTN)
            leaf = parent;
        else {
            leaf = this;
            leaf->m_key.clear();
        }
    }
    else
        leaf = new TrieNode();
    
    leaf->m_key.push_back(key.withTerminator());
    leaf->m_value.reset(value);
    leaf->m_children.reset();
    return leaf;
}

/// @brief creates a new Extension or tries to mutate "this" into a Extension: mutation keeps the parent consistant.
/// WARNING:  If a new Object is created, the caller is in charge of assigning the right parent to the object.
///           A caller of this function should not assume that mutating "this" returns "this".
///           The function might merge the Extension with the parent when possible, and then return
///           the merged PARENT.
///           Best practive is to always do "auto mutated_this = createExtension(..., true);"
///           and use mutated_this as the final Extension (not "this").
/// @param key: the key of the Extension
/// @param do_mutate: if true, tries to mutate "this" into a Extension: might cause merge with the parent
/// @return the Extension pointer, which might differ from "this" even in a mutation scenario.
template <typename T>
TrieNode<T>* TrieNode<T>::createExtension(const ByteSet<NIBBLE>& key, bool do_mutate) {
    TrieNode* extension = nullptr;
    if(do_mutate) {
        auto parent = const_cast<TrieNode*>(dynamic_cast<const TrieNode*>(getParent()));
        if(parent && parent->getType() == TYPE::EXTN)
            extension = parent;
        else {
            extension = this;
            extension->m_key.clear();
        }
    }
    else
        extension = new TrieNode();
    extension->m_key.push_back(key.withoutTerminator());
    extension->m_value.reset();
    extension->m_children.reset();
    extension->m_children = unique_arr<unique_ptr<TrieNode>>(1);
    return extension;
}

/// @brief creates a new Branch or tries to mutate "this" into a Branch: mutation keeps the parent consistant.
/// WARNING:  If a new Object is created, the caller is in charge of assigning the right parent to the object.
/// @param value: the value of the Branch
/// @param do_mutate: if true, mutates "this" into a Branch
/// @return the Branch pointer, which is always "this" in a mutation scenario (no merge with the parent).
template <typename T>
TrieNode<T>* TrieNode<T>::createBranch(const T* value, bool do_mutate) {
    TrieNode* branch = do_mutate ? this : new TrieNode();
    branch->m_key.clear();
    branch->m_value.reset(value);
    branch->m_children.reset();
    branch->m_children = unique_arr<unique_ptr<TrieNode>>(16);
    return branch;
}

template <typename T>
const ByteSet<BYTE> TrieNode<T>::hash() const {
    RLPByteSet<BYTE> rlp;

    if(getType() == TYPE::EMPTY) {
        rlp = RLPByteSet<BYTE>::EMPTY;
        //cout << "Empty node " << dec << " rlp = " << rlp.asString() << endl;
    }
    else if(getType() == TYPE::LEAF) {
        rlp.push_back_rlp(m_key.HexToCompact());
        rlp.push_back_rlp(m_value->getValue());
        cout << "Leaf " << dec << " rlp = " << rlp.asString() << endl;
    }
    else if(getType() == TYPE::EXTN) {
        rlp.push_back_rlp(m_key.HexToCompact());
        ByteSet<BYTE> h(m_children[0]->hash());
        RLPByteSet<BYTE> rlp_encoded_h(h);
        if(h.byteSize() < 32)
            //If the child node hash length < 32 Bytes, the hash is in fact already a RLP => raw copy
            rlp_encoded_h = h;
        rlp.push_back_rlp(rlp_encoded_h);    
        cout << "Extension " << dec << " rlp = " << rlp.asString() << endl;
    }
    else if(getType() == TYPE::BRAN) {
        for(uint i=0;i<m_children.size();i++) {
            if(m_children[i]) {
                ByteSet<BYTE> h(m_children[i]->hash());
                RLPByteSet<BYTE> rlp_encoded_h(h);
                if(h.byteSize() < 32)
                    //If the child node hash length < 32 Bytes, the hash is in fact already a RLP => raw copy
                    rlp_encoded_h = h;
                rlp.push_back_rlp(rlp_encoded_h);   
            }
            else
                rlp.push_back_rlp(ByteSet<BYTE>::EMPTY);
        }
        rlp.push_back_rlp(m_value ? m_value->getValue() : ByteSet<BYTE>::EMPTY);
        cout << "Branch " << dec << " rlp = " << rlp.asString() << endl;
    }
    if(rlp.byteSize() >= 32 || isRoot()){    // < 32 Bytes => Value node, else Hash Node
        cout << "Node " << dec << " hash = " << rlp.keccak256().asString() << endl;
        return rlp.keccak256();
    }
    else {
        cout << "Node " << dec << " hash = " << rlp.asString() << endl;
        return rlp.as<BYTE>();
    }
} 

template <typename T>
void TrieNode<T>::connectChild(TrieNode* child, uint child_index) {
    if(child) {
        assert(child_index < m_children.size() && !m_children[child_index]);
        child->setParent(this);
        m_children[child_index].reset(child);
    }
}

template <typename T>
TrieNode<T>* TrieNode<T>::disconnectChild(uint child_index) {
    TrieNode* child = nullptr;
    assert(child_index < m_children.size());
    if(m_children[child_index]) {
        m_children[child_index]->setParent(nullptr);
        child = m_children[child_index].release();
    }
    return child;
}

template <typename T>
TrieNode<T>* TrieNode<T>::insert(TrieNode* parent, uint index_in_parent, TrieNode* child, uint child_index, TYPE type, ByteSet<NIBBLE>* key, const T* value) {
    assert(parent && child);
    assert(!parent->m_children[index_in_parent]);
    TrieNode* node;
    if(type == TYPE::LEAF) {
        assert(key && value);
        node = createLeaf(*key, value);
    }
    else if(type == TYPE::EXTN) {
        assert(key);
        node = createExtension(*key);
        node->connectChild(child, child_index);
    }
    else if(type == TYPE::BRAN) {
        node = createBranch(value);
        node->connectChild(child, child_index);
    }
    parent->connectChild(node, index_in_parent);
    return node;
}

template <typename T>
void TrieNode<T>::storeKV(ByteSet<NIBBLE> &key, const T* value) {
    ByteSet<NIBBLE> shared_nibbles, unshared_nibbles;
    switch(getType()) {
        case EMPTY:
            if(value) {
                //First KV storage: EMPTY ROOT => LEAF
                m_key = key.withTerminator();
                m_value.reset(value);
            }
            break;
        case LEAF: {
            if(!value)
                // (key,value) erasure
                wipeK();
            else if(key == m_key.withoutTerminator())
                //LEAF value update
                m_value.reset(value);
            else {
                //prune key and m_key of their common nibbles.
                shared_nibbles = extractCommonNibbles(key, m_key);
                unshared_nibbles = key;
                
                //Save a copy of the pruned Leaf
                TrieNode* pruned_previous_leaf = nullptr;
                unique_ptr<const T> previous_value(m_value.release());
                uint pruned_previous_leaf_index = -1;
                if(!m_key.isTerminator()) {
                    pruned_previous_leaf_index = m_key.pop_front_elem();
                    pruned_previous_leaf = createLeaf(m_key, previous_value.release());
                }

                if(shared_nibbles.getNbElements()) {
                    //Some common nibbles: mutate from LEAF => EXTENSION
                    auto ext = createExtension(shared_nibbles, true);

                    TrieNode* branch = nullptr;
                    if(pruned_previous_leaf)
                        //insert a BRANCH as child of ext EXTENSION and reconnect the pruned LEAF
                        branch = insert(ext, 0, pruned_previous_leaf, pruned_previous_leaf_index, BRAN);
                    else {
                        //create a sub-BRANCH with the former LEAF m_value
                        branch = createBranch(previous_value.release());
                        //and connect it to THIS EXTENSION
                        ext->connectChild(branch, 0);
                    }

                    //Continue the key parsing
                    branch->storeKV(key, value);
                }
                else {
                    //No common nibbles: mutate from LEAF => BRANCH
                    createBranch(nullptr, true);
                    
                    if(pruned_previous_leaf)
                        //Reconnect the pruned LEAF
                        this->connectChild(pruned_previous_leaf, pruned_previous_leaf_index);
                    else
                        //Stores the previous LEAF value
                        m_value.reset(previous_value.release());

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
            else if(!shared_nibbles.getNbElements()) {
                //Disconnect the previous child branch before mutation
                auto previous_branch = disconnectChild(0);

                //No common nibbles: mutate from EXTN => BRANCH
                createBranch(nullptr, true);
                
                uint64_t unshared_index = unshared_nibbles.pop_front_elem();
                if(unshared_nibbles.getNbElements())
                    //Insert a new child EXTENSION between the mutated branch (parent) and the previous branch (child)
                    auto unshared_ext = insert(this, unshared_index, previous_branch, 0, EXTN, &unshared_nibbles);
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
                    previous_branch = new_branch->disconnectChild(new_branch_child_index);

                    //Insert a new child EXTENSION between the newly created branch (parent) and the previous branch (child)
                    auto unshared_ext = insert(new_branch, new_branch_child_index, previous_branch, 0, EXTN, &unshared_nibbles);
                }
                //Continue the key parsing under the new branch
                new_branch->storeKV(key, value);
            }
            break;
        }
        case BRAN: {
            if(!key.getNbElements())
                m_value.reset(value);
            else {
                uint64_t index = key.pop_front_elem();

                if(!m_children[index]) {
                    //The key's first nibble placeholder is empty => new LEAF
                    auto new_leaf = createLeaf(key, value);
                    connectChild(new_leaf, index);
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

template <typename T>
const T* TrieNode<T>::getV(ByteSet<NIBBLE> &key, bool &is_absent) const {
    const T* result = nullptr;
    key.addTerminator();
    while(!is_absent && key.getNbElements()) {
        if( getType() == TYPE::BRAN ) {
            if(!key.isTerminator()) {
                if(auto next_child = m_children[key.pop_front_elem()].get(); next_child)
                    result = next_child->getV(key, is_absent);
                else
                    is_absent = true;
            }
            else
                result = m_value.get();
        }
        else if( getType() == TYPE::LEAF &&
                 m_key == key.pop_back(key.getNbElements()) )
            result = m_value.get();
        else if( getType() == TYPE::EXTN &&
                 key.getNbElements() > m_key.getNbElements() &&
                 m_key == key.pop_front(m_key.getNbElements()) )
            result = m_children[0]->getV(key, is_absent);
        else
            is_absent = true;
    }
    return result;
}

template <typename T>
const IComponent* TrieNode<T>::getChild(uint child_index) const {
    ByteSet<NIBBLE> key(child_index ? ByteSet<BYTE>(child_index).serialize().as<NIBBLE>() : ByteSet<BYTE>::EMPTY.serialize().as<NIBBLE>());
    bool is_absent = false;
    return getV(key, is_absent);
}

template <typename T>
int TrieNode<T>::getChildIndex(const TrieNode* child) const {
    int index = -1;
    for(uint i=0;i<m_children.size();i++) {
        if(m_children[i].get() == child) {
            index = i;
            break;
        }
    }
    return index;
}

template <typename T>
int TrieNode<T>::getFirstChildIndex() const
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

template <typename T>
uint TrieNode<T>::getChildrenNodeCount() const {
    int counter = 0;
    for(uint i=0;i<m_children.size();i++) {
        if(m_children[i])
            counter++;
    }
    return counter;
}

template <typename T>
void TrieNode<T>::wipeK(uint index) {
    auto parent = const_cast<TrieNode*>(dynamic_cast<const TrieNode*>(getParent()));
    switch(getType()) {
        case EMPTY:
            break;
        case LEAF: {
                if(parent)
                    parent->wipeK(parent->getChildIndex(this));
                else {
                    //Root erasure
                    m_key.clear();
                    m_value.reset();
                }
            break;
        }
        case EXTN: {
            //An EXTENSION will never require direct erasure. The mandatory BRANCH under it
            // will first mutate into an extension to merge with it, then when its child banch mutates into 
            // a single leaf, the leaf will absorbe the extension into a new leaf that may be erased.
            break;
        }
        case BRAN: {
                if(m_children[index])
                    m_children[index].reset();
                uint nb_children = getChildrenNodeCount();
                if(!nb_children && m_value)
                    //This BRANCH mutates to LEAF to integrate the m_value of the former BRANCH
                    auto leaf = createLeaf(EMPTY_KEY, m_value.release(), true);
                else if(nb_children == 1 && !m_value) {
                    int only_child_index = getFirstChildIndex();
                    TrieNode* only_child = m_children[only_child_index].release();
                    
                    ByteSet<NIBBLE> new_key(only_child_index);
                    if(only_child->getType() != TYPE::BRAN)
                        new_key.push_back(only_child->m_key.withoutTerminator());

                    if(only_child->getType() == TYPE::LEAF) {
                        //This BRANCH mutates to LEAF to integrate the m_key/m_value of only_child
                        auto leaf = createLeaf(new_key, only_child->m_value.release(), true);              
                    }
                    else if(only_child->getType() == TYPE::EXTN) {
                        auto child_extension_child = only_child->m_children[0].release();   //Disconnect only_child's child
                        //This BRANCH mutates to EXTENSION to integrate the m_key of only_child
                        auto ext = createExtension(new_key, true);
                        ext->connectChild(child_extension_child, 0);
                    }
                    else {
                        //This BRANCH mutates to EXTENSION with only_child_index as key
                        auto ext = createExtension(new_key, true);
                        ext->connectChild(only_child, 0);
                    }
                } 
            break;
        }
        default:
            break;
    }
}

template <typename T>
ByteSet<NIBBLE> TrieNode<T>::extractCommonNibbles(ByteSet<NIBBLE> &key1, ByteSet<NIBBLE> &key2) const {
    ByteSet<NIBBLE> result;
    while(key1.getNbElements() && key2.getNbElements() && key1.getElem(0) == key2.getElem(0)) {
        result.push_back_elem(key1.pop_front_elem());
        key2.pop_front_elem();
    }
    return result;
}

template <typename T>
void TrieNode<T>::print() const {
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
        cout << "Dumping " << toString(getType()) << " Node " << this << "(k:" << m_key.asString() << (m_key.hasTerminator() ? "10" : "") << " v:" << (m_value ? "yes" : "no" ) <<" is " << toString(getType()) << ") :" << endl;
        for(int i=0;i<m_children.size();i++) {
            if(m_children[i])
                cout << m_children[i].get() << "(k:" << m_children[i]->m_key.withoutTerminator().asString() << (m_children[i]->m_key.hasTerminator() ? "10" : "") << " v:" << (m_children[i]->m_value ? "yes" : "no" ) <<" is " << toString(m_children[i]->getType()) << ") ";
            else
                cout << m_children[i].get() << "() ";
        }
        cout << endl << endl;
        for(int i=0;i<m_children.size();i++) {
            if(m_children[i])
                m_children[i]->print();
        }
    }
}