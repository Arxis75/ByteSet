#pragma once

/*struct node
{
    public:
        void add(const ByteSet<> &key, const ByteSet<> &value)
        {
            if(m_children.size() == 0) {
                node* n = new node(key, value);
            }     
        }

        virtual ByteSet<> getHash() const 
        {
            ByteSet<> to_hash;
            for(int i=0;i<m_children.size(); i++)
                to_hash.push_back(m_children[i]->getHash());
            return to_hash.keccak256();
        }

    protected:
        node(const ByteSet<> &key, const ByteSet<> &value);
    
    private:
        node* m_parent;
        std::map<ByteSet<> key, node* child> m_children;
        ByteSet<> m_value;
};

struct Trie : public node
{
};

struct BrancheNode : public node
{
};

struct ExtensionNode : public node
{
};

struct LeafNode : public node
{
};*/