struct node
{
    protected:
        node();
};

struct Trie
{
    node* m_root;

    void add(ByteSet<> key, ByteSet<> value);
};

struct BrancheNode : public node
{
    ByteSet<> m_hash;
    ByteSet<> m_value;
    node* m_parent;
    std::map<char, node*> m_children;
};

struct ExtensionNode : public node
{
    ByteSet<> m_hash;
    ByteSet<> m_value;
    node* m_parent;
    std::map<char, node*> m_children;
};

struct LeafNode : public node
{
    ByteSet<> m_value;
    node* m_parent;
};