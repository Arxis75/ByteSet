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
        virtual ~ByteSetTrieNode() { cout << "-- node " << this << " deleted. --" << endl; }

        virtual const ByteSet<BYTE> hash() const;
        
        virtual uint64_t getChildrenCount() const override;
        virtual void dumpChildren() const override;

        inline TYPE getType() const { return m_children.size() == 16 ? BRAN : (m_children.size() == 0 ? (m_value.getNbElements() ? LEAF : EMPTY) : EXTN);}
        
        /*inline const ByteSet<NIBBLE>& getKey() const { return m_key; }
        inline void setKey(const ByteSet<NIBBLE>& key) { m_key = key; }

        inline const ByteSet<BYTE>& getValue() const { return m_value; }
        inline void setValue(const ByteSet<BYTE>& value) { m_value = value; }*/

        inline void clear() { m_children.release(); m_key.clear(); m_value.clear(); }

    protected:
        ByteSetTrieNode* createLeaf(const ByteSet<NIBBLE>& key, const ByteSet<BYTE>& value, bool do_mutate = false);
        ByteSetTrieNode* createExtension(const ByteSet<NIBBLE>& key, bool do_mutate = false);
        ByteSetTrieNode* createBranch(const ByteSet<BYTE>& value, bool do_mutate = false);

        void storeKV(ByteSet<NIBBLE> &key, const ByteSet<BYTE>& value);
        void wipeK(uint index = 0);

        void connectChild(ByteSetTrieNode* child, uint child_index);
        ByteSetTrieNode* disconnectChild(uint child_index);
        ByteSetTrieNode* insert(ByteSetTrieNode* parent, uint index_in_parent, ByteSetTrieNode* child, uint child_index, TYPE type, const ByteSet<NIBBLE>& key = EMPTY_KEY, const ByteSet<BYTE>& value = EMPTY_VALUE);
        int getChildIndex(const ByteSetTrieNode* child) const;
        int getFirstChildIndex() const;

        ByteSet<NIBBLE> extractCommonNibbles(ByteSet<NIBBLE> &key1, ByteSet<NIBBLE> &key2) const;

    protected:
        unique_arr<std::unique_ptr<ByteSetTrieNode>> m_children;
        ByteSet<NIBBLE> m_key;
        ByteSet<BYTE> m_value;
};

class BlockTransactionsTrie : public ByteSetTrieNode
{
    public:
        BlockTransactionsTrie(bool is_secure = false) : ByteSetTrieNode(), m_is_secure(is_secure) {}
        virtual ~BlockTransactionsTrie() = default;

        inline void store(ByteSet<NIBBLE> &key, const ByteSet<BYTE>& value) {
            ByteSet<NIBBLE> tmp = m_is_secure ? key.keccak256() : key;
            storeKV(tmp, value); 
        }

    private:
        const bool m_is_secure;
};