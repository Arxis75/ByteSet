#pragma once
#include <ByteSet/ByteSetComposite.h>
#include <ByteSet/Tools.h>

class ByteSetMPTNode : public virtual IByteSetComposite
{
    static const ByteSet<NIBBLE> EMPTY_KEY;
    static const ByteSet<BYTE> EMPTY_VALUE;
    public:
        enum TYPE {EMPTY, EXTN, BRAN, LEAF};
        ByteSetMPTNode() = default;
        ByteSetMPTNode(ByteSet<NIBBLE> &key, ByteSet<BYTE> value, TYPE target_type) {
            assert(target_type != TYPE::EMPTY);

            if(key.getNbElements() && key.getElem(key.getNbElements()-1) == 0x10)
                key.pop_back_elem();                            //general case: remove the terminator

            if(target_type == TYPE::LEAF) {
                m_key = key;
                m_key.push_back_elem(0x10);                     //add a terminator
                m_value = value;                                //set the value
            }
            else if(target_type == TYPE::EXTN) {
                m_children = unique_arr<unique_ptr<ByteSetMPTNode>>(1);
                m_key = key;                                    
            }
            else if(target_type == TYPE::BRAN) {
                m_children = unique_arr<unique_ptr<ByteSetMPTNode>>(16);
                if(!key.getNbElements())
                    m_value = value;                            //if key ends here, store value
            }
        }
        ByteSetMPTNode(const ByteSetMPTNode&) = delete;
        ByteSetMPTNode& operator=(const ByteSetMPTNode&) = delete;
        virtual ~ByteSetMPTNode() { deleteChildren(); }
 
        inline virtual void RLPparse(ByteSet<BYTE> &b) override { /*TODO*/ }
        inline virtual const ByteSet<BYTE> RLPserialize() const override { /*TODO*/ } 

        inline virtual uint64_t getChildrenCount() const override { return m_children.size(); }        
        virtual void DumpChildren() const override {
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
                cout << "Dumping " << toString(getType()) << " Node " << this << ":" << endl;
                for(int i=0;i<m_children.size();i++) {
                    if(m_children[i])
                        cout << m_children[i].get() << "(" << toString(m_children[i]->getType()) << ") ";
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

        ByteSet<NIBBLE> extractCommonNibbles(ByteSet<NIBBLE> &key1, ByteSet<NIBBLE> &key2) const {
            ByteSet<NIBBLE> result;
            while(key1.getNbElements() && key2.getNbElements() && key1.getElem(0) == key2.getElem(0)) {
                result.push_back_elem(key1.pop_front_elem());
                key2.pop_front_elem();
            }
            return result;
        }

        ByteSetMPTNode* disconnectChild(uint child_index) {
            ByteSetMPTNode* child = nullptr;
            if(getType() == TYPE::EXTN) child_index = 0;
            if(m_children[child_index]) {
                m_children[child_index]->setParent(nullptr);
                child = m_children[child_index].release();
            }
            return child;
        }
        void connectChild(ByteSetMPTNode* child, uint child_index) {
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
        ByteSetMPTNode* disconnectFromParent(uint index_in_parent) {
            if(ByteSetMPTNode* parent = const_cast<ByteSetMPTNode*>(dynamic_cast<const ByteSetMPTNode*>(getParent())); parent) {
                if(parent->getType() == TYPE::EXTN) index_in_parent = 0;
                parent->m_children[index_in_parent].release();
                setParent(nullptr);
            }
            return this;
        }
        void connectToParent(ByteSetMPTNode* parent, uint index_in_parent) {
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

        ByteSetMPTNode* insert(ByteSetMPTNode* parent, uint index_in_parent, ByteSetMPTNode* child, uint child_index, TYPE type, const ByteSet<NIBBLE>& key = EMPTY_KEY, const ByteSet<BYTE>& value = EMPTY_VALUE) {
            assert(type != TYPE::EMPTY);
            ByteSetMPTNode* node = new ByteSetMPTNode();
            if(type == TYPE::LEAF) {
                node->m_key = key.withTerminator();
                node->m_value = value;
            }
            else if(type == TYPE::EXTN) {
                m_children = unique_arr<unique_ptr<ByteSetMPTNode>>(1);
                node->m_key = key.withoutTerminator();
                connectChild(child, child_index);
            }
            else if(type == TYPE::BRAN) {
                m_children = unique_arr<unique_ptr<ByteSetMPTNode>>(16);
                connectChild(child, child_index);
            }
            connectToParent(parent, index_in_parent);
            return node;
        }

        void store(ByteSet<NIBBLE> key, ByteSet<BYTE> value) {
            ByteSet<NIBBLE> shared_nibbles, unshared_nibbles;
            switch(getType()) {
                case EMPTY:
                    //First KV storage: EMPTY => LEAF
                    m_key = key.withTerminator();
                    m_value = value;
                    break;
                case LEAF: {
                    assert(key != m_key);   //no double insertion

                    //prune key and m_key of their common nibbles.
                    shared_nibbles = extractCommonNibbles(key, m_key);
                    unshared_nibbles = key;
                    
                    //Save the pruned Leaf
                    uint pruned_previous_leaf_index = unshared_nibbles.pop_front_elem();   //should be at least the terminator left to pop
                    ByteSetMPTNode* pruned_previous_leaf = new ByteSetMPTNode(m_key, m_value, LEAF);

                    if(shared_nibbles.getNbElements()) {
                        //Some common nibbles: mutate from LEAF => EXTENSION
                        m_key = shared_nibbles;
                        m_value.clear();
                        m_children = unique_arr<unique_ptr<ByteSetMPTNode>>(1);

                        //insert a BRANCH as child of this EXTENSION and reconnect the previous LEAF
                        auto branch = insert(this, 0, pruned_previous_leaf, pruned_previous_leaf_index, BRAN);

                        //Continue the key parsing
                        if(unshared_nibbles.getNbElements())
                            branch->store(unshared_nibbles, value);
                    }
                    else {
                        //No common nibbles: mutate from LEAF => BRANCH
                        m_key.clear();
                        m_value.clear();
                        m_children = unique_arr<unique_ptr<ByteSetMPTNode>>(16);
                        
                        //Reconnect the previous LEAF:
                        pruned_previous_leaf->connectToParent(this, pruned_previous_leaf_index);

                        //Continue the key parsing
                        if(unshared_nibbles.getNbElements())
                            store(unshared_nibbles, value);
                    }
                    break;
                }
                case EXTN: {
                    //prune key and m_key of their common nibbles.
                    shared_nibbles = extractCommonNibbles(key, m_key);
                    unshared_nibbles = key;

                    if(shared_nibbles.getNbElements()) {
                        //Disconnect the current child branch
                        ByteSetMPTNode* previous_branch = disconnectChild(0);

                        //Add a new BRANCH child
                        uint64_t new_branch_child_index = unshared_nibbles.pop_front_elem();   //consume 1 NIBBLE taken by the new branch
                        auto new_branch = insert(this, 0, previous_branch, new_branch_child_index, BRAN);

                        if(m_key.getNbElements()) {
                            //Disconnect the newly created branch from the previous branch (its child)
                            new_branch->disconnectChild(new_branch_child_index);
                            //Insert a new child EXTENSION between the newly created branch (parent) and the previous branch (child)
                            auto unshared_ext = insert(new_branch, new_branch_child_index, previous_branch, new_branch_child_index, BRAN);
                            unshared_ext->m_key = unshared_nibbles.pop_back(m_key.getNbElements());
                        }
                    }
                    else {
                        //Disconnect the previous child before mutation
                        auto child = disconnectChild(0);
                        uint64_t child_index = unshared_nibbles.pop_back_elem(); //consume 1 NIBBLE when becoming a branch
                        
                        //No common nibbles: mutate from EXTN => BRANCH
                        m_key.clear();
                        m_value.clear();
                        m_children = unique_arr<unique_ptr<ByteSetMPTNode>>(16);

                        //Reconnect the previous child after mutation
                        connectChild(child, child_index);
                        

                        //Continue the key parsing
                        if(unshared_nibbles.getNbElements())
                            store(unshared_nibbles, value);
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
                            auto new_empty = new ByteSetMPTNode(key, value, LEAF);
                            new_empty->setParent(this);
                            m_children[index].reset(new_empty);
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

        inline TYPE getType() const { return m_children.size() == 16 ? BRAN : (m_children.size() == 0 ? (m_value.getNbElements() ? LEAF : EMPTY) : EXTN);}
        
        inline ByteSet<NIBBLE> getKey() const { return m_key; }
        inline void setKey(ByteSet<NIBBLE> key) { m_key = key; }

        inline ByteSet<BYTE> getValue() const { return m_value; }
        inline void setValue(ByteSet<BYTE> value) { m_value = value; }

    protected:
        virtual void deleteChildren() override { /*TODO*/}

    public:
        unique_arr<std::unique_ptr<ByteSetMPTNode>> m_children;
        ByteSet<NIBBLE> m_key;
        ByteSet<BYTE> m_value;
};

class BlockTransactionsTrie : public ByteSetMPTNode
{
    public:
        BlockTransactionsTrie() = default;
        virtual ~BlockTransactionsTrie() = default;
};