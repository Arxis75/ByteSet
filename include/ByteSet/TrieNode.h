#pragma once
#include <ByteSet/Composite.h>
#include <ByteSet/Tools.h>

template <typename T = ByteSet<BYTE>>
class TrieNode : public IComposite
{
    static_assert(std::is_base_of_v<IComponent, T>, "Trie value type must inherit from IComponent");

        inline static const ByteSet<NIBBLE> EMPTY_KEY = ByteSet<NIBBLE>();

    public:
        enum TYPE {EMPTY, EXTN, BRAN, LEAF};
        TrieNode() = default;
        TrieNode(const TrieNode&) = delete;
        TrieNode& operator=(const TrieNode&) = delete;
        virtual ~TrieNode() = default; //{ cout << "-- node " << this << " deleted. --" << endl; }

        virtual const ByteSet<BYTE> hash() const;
        
        virtual uint64_t getChildrenCount() const override;
        virtual void printChildren() const override;

        inline TYPE getType() const { return m_children.size() == 16 ? BRAN : (m_children.size() == 0 ? (!m_value.isEmpty() ? LEAF : EMPTY) : EXTN);}

        //*********************************** ICOMPONENT INTERFACE ************************************************/
        inline virtual void  parse(ByteSet<BYTE> &b) override { /*TODO*/};
        inline virtual const ByteSet<BYTE> serialize() const override { return TrieNode<T>::hash(); }
        inline virtual bool isEmpty() const override { return TrieNode<T>::getType() ==  TrieNode<T>::TYPE::EMPTY; }
        inline void clear() override { m_children.reset(); m_key.clear(); m_value.clear(); }

    protected:
        TrieNode* createLeaf(const ByteSet<NIBBLE>& key, T&& value, bool do_mutate = false);
        TrieNode* createExtension(const ByteSet<NIBBLE>& key, bool do_mutate = false);
        TrieNode* createBranch(T* value = nullptr, bool do_mutate = false);

        void storeKV(ByteSet<NIBBLE> &key, T&& value);
        void wipeK(uint index = 0);

        void connectChild(TrieNode* child, uint child_index);
        TrieNode* disconnectChild(uint child_index);
        TrieNode* insert(TrieNode* parent, uint index_in_parent, TrieNode* child, uint child_index, TYPE type, ByteSet<NIBBLE>* key = nullptr, T* value = nullptr);
        int getChildIndex(const TrieNode* child) const;
        int getFirstChildIndex() const;

        ByteSet<NIBBLE> extractCommonNibbles(ByteSet<NIBBLE> &key1, ByteSet<NIBBLE> &key2) const;

    protected:
        unique_arr<std::unique_ptr<TrieNode>> m_children;
        ByteSet<NIBBLE> m_key;
        T m_value;
};

template <typename T = ByteSet<BYTE>>
class SecureTrieNode : public TrieNode<T>
{
    public:
        SecureTrieNode(bool is_secure = false) : TrieNode<T>(), m_is_secure(is_secure) {}
        virtual ~SecureTrieNode() = default;

        inline void store(ByteSet<NIBBLE> &key, T& value) {
            ByteSet<NIBBLE> tmp = m_is_secure ? key.keccak256() : key;
            TrieNode<T>::storeKV(tmp, std::move(value)); 
        }

    private:
        const bool m_is_secure;
};

#include <ByteSet/TrieNode.tpp>