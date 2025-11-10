#pragma once
#include <ByteSet/ByteSetComposite.h>
#include <ByteSet/Tools.h>

class ByteSetTrieNode : public IByteSetComposite
{
        inline static const ByteSet<NIBBLE> EMPTY_KEY = ByteSet<NIBBLE>();
        inline static const ByteSet<BYTE> EMPTY_VALUE = ByteSet<BYTE>();

    public:
        enum TYPE {EMPTY, EXTN, BRAN, LEAF};
        ByteSetTrieNode() = default;
        ByteSetTrieNode(const ByteSetTrieNode&) = delete;
        ByteSetTrieNode& operator=(const ByteSetTrieNode&) = delete;
        virtual ~ByteSetTrieNode() = default;

        virtual const ByteSet<BYTE> hash() const;
        inline virtual uint64_t getChildrenCount() const override {return m_children.size(); }
        virtual void DumpChildren() const override;

        ByteSet<NIBBLE> extractCommonNibbles(ByteSet<NIBBLE> &key1, ByteSet<NIBBLE> &key2) const;

        ByteSetTrieNode* disconnectChild(uint child_index);
        void connectChild(ByteSetTrieNode* child, uint child_index);
        ByteSetTrieNode* disconnectFromParent(uint index_in_parent);
        void connectToParent(ByteSetTrieNode* parent, uint index_in_parent);

        ByteSetTrieNode* insert(ByteSetTrieNode* parent, uint index_in_parent, ByteSetTrieNode* child, uint child_index, TYPE type, const ByteSet<NIBBLE>& key = EMPTY_KEY, const ByteSet<BYTE>& value = EMPTY_VALUE);

        void store(ByteSet<NIBBLE> key, ByteSet<BYTE> value);

        inline TYPE getType() const { return m_children.size() == 16 ? BRAN : (m_children.size() == 0 ? (m_value.getNbElements() ? LEAF : EMPTY) : EXTN);}
        inline const ByteSet<NIBBLE>& getKey() const { return m_key; }
        inline void setKey(const ByteSet<NIBBLE>& key) { m_key = key; }

        inline const ByteSet<BYTE>& getValue() const { return m_value; }
        inline void setValue(const ByteSet<BYTE>& value) { m_value = value; }

    protected:

    public:
        unique_arr<std::unique_ptr<ByteSetTrieNode>> m_children;
        ByteSet<NIBBLE> m_key;
        ByteSet<BYTE> m_value;
};

class BlockTransactionsTrie : public ByteSetTrieNode
{
    public:
        BlockTransactionsTrie() = default;
        virtual ~BlockTransactionsTrie() = default;
};