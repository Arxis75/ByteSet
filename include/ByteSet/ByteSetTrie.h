#pragma once
#include <ByteSet/ByteSetComposite.h>
#include <ByteSet/Tools.h>

template <typename T = ByteSet<BYTE>>
class ByteSetTrieNode : public IByteSetComposite
{
    static_assert(std::is_base_of_v<ITrieable, T>, "Trie value type must inherit from ITrieable");

        inline static const ByteSet<NIBBLE> EMPTY_KEY = ByteSet<NIBBLE>();

    public:
        enum TYPE {EMPTY, EXTN, BRAN, LEAF};
        ByteSetTrieNode() = default;
        ByteSetTrieNode(const ByteSetTrieNode&) = delete;
        ByteSetTrieNode& operator=(const ByteSetTrieNode&) = delete;
        virtual ~ByteSetTrieNode() = default; //{ cout << "-- node " << this << " deleted. --" << endl; }

        virtual const ByteSet<BYTE> hash() const;
        
        virtual uint64_t getChildrenCount() const override;
        virtual void dumpChildren() const override;

        inline TYPE getType() const { return m_children.size() == 16 ? BRAN : (m_children.size() == 0 ? (!m_value.isEmpty() ? LEAF : EMPTY) : EXTN);}

        inline void clear() { m_children.reset(); m_key.clear(); m_value.clear(); }

    protected:
        ByteSetTrieNode* createLeaf(const ByteSet<NIBBLE>& key, T&& value, bool do_mutate = false);
        ByteSetTrieNode* createExtension(const ByteSet<NIBBLE>& key, bool do_mutate = false);
        ByteSetTrieNode* createBranch(T&& value, bool do_mutate = false);

        void storeKV(ByteSet<NIBBLE> &key, T&& value);
        void wipeK(uint index = 0);

        void connectChild(ByteSetTrieNode* child, uint child_index);
        ByteSetTrieNode* disconnectChild(uint child_index);
        ByteSetTrieNode* insert(ByteSetTrieNode* parent, uint index_in_parent, ByteSetTrieNode* child, uint child_index, TYPE type, const ByteSet<NIBBLE>& key = EMPTY_KEY, T&& value = T());
        int getChildIndex(const ByteSetTrieNode* child) const;
        int getFirstChildIndex() const;

        ByteSet<NIBBLE> extractCommonNibbles(ByteSet<NIBBLE> &key1, ByteSet<NIBBLE> &key2) const;

    protected:
        unique_arr<std::unique_ptr<ByteSetTrieNode>> m_children;
        ByteSet<NIBBLE> m_key;
        T m_value;
};

template <typename T = ByteSet<BYTE>>
class BlockTransactionsTrie : public ByteSetTrieNode<T>
{
    public:
        BlockTransactionsTrie(bool is_secure = false) : ByteSetTrieNode<T>(), m_is_secure(is_secure) {}
        virtual ~BlockTransactionsTrie() = default;

        inline void store(ByteSet<NIBBLE> &key, T& value) {
            ByteSet<NIBBLE> tmp = m_is_secure ? key.keccak256() : key;
            ByteSetTrieNode<T>::storeKV(tmp, std::move(value)); 
        }

    private:
        const bool m_is_secure;
};

#include <ByteSet/ByteSetTrie.tpp>