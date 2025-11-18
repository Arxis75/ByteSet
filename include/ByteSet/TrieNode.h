#pragma once
#include <ByteSet/IComponent.h>
#include <ByteSet/ByteSet.h>
#include <ByteSet/Tools.h>

template <typename T = ByteSet<BYTE>>
class TrieNode : public IComposite
{
    static_assert(std::is_base_of_v<IComponent, T>, "Trie value type must inherit from IComponent");

        inline static const ByteSet<NIBBLE> EMPTY_KEY = ByteSet<NIBBLE>();

    public:
        enum TYPE {EMPTY, EXTN, BRAN, LEAF};
        virtual ~TrieNode() = default; //{ cout << "-- node " << this << " deleted. --" << endl; }

        //*********************************** ICOMPONENT INTERFACE ************************************************/
        inline virtual const ByteSet<BYTE> serialize() const override { return TrieNode<T>::hash(); }
        virtual void print() const override;
        inline virtual bool isEmpty() const override { return TrieNode<T>::getType() ==  TrieNode<T>::TYPE::EMPTY; }
        inline void clear() override { m_children.reset(); m_key.clear(); m_value.reset(); }
        //**********************************************************************************************************

        virtual const ByteSet<BYTE> hash() const;
        inline TYPE getType() const { return m_children.size() == 16 ? BRAN : (m_children.size() == 0 ? (m_value ? LEAF : EMPTY) : EXTN);}
        inline virtual const bool isRoot() const { return false; }

    protected:
        TrieNode() = default;

         //*********************************** ICOMPOSITE INTERFACE ************************************************
        virtual IComponent* newChild(uint creation_index = 0) override { return new T(); }
        virtual void addChild(IComponent *child, const ByteSet<NIBBLE>& key) override;
        virtual const IComponent* getChild(uint child_index) const override { return (child_index < m_children.size() ? m_children[child_index].get() : nullptr); }
        virtual uint getChildrenCount() const override;
        inline virtual uint getChildrenContainerSize() const override { return m_children.size(); }
        //**********************************************************************************************************

        TrieNode* createLeaf(const ByteSet<NIBBLE>& key, const T* value, bool do_mutate = false);
        TrieNode* createExtension(const ByteSet<NIBBLE>& key, bool do_mutate = false);
        TrieNode* createBranch(const T* value, bool do_mutate = false);

        void storeKV(ByteSet<NIBBLE> &key, const T* value);
        void wipeK(uint index = 0);

        void connectChild(TrieNode* child, uint child_index);
        TrieNode* disconnectChild(uint child_index);
        TrieNode* insert(TrieNode* parent, uint index_in_parent, TrieNode* child, uint child_index, TYPE type, ByteSet<NIBBLE>* key = nullptr, const T* value = nullptr);
        int getChildIndex(const TrieNode* child) const;
        int getFirstChildIndex() const;

        ByteSet<NIBBLE> extractCommonNibbles(ByteSet<NIBBLE> &key1, ByteSet<NIBBLE> &key2) const;

    protected:
        unique_arr<unique_ptr<TrieNode>> m_children;
        ByteSet<NIBBLE> m_key;
        unique_ptr<const T> m_value;
};

template <typename T = ByteSet<BYTE>>
class TrieRoot : public TrieNode<T>
{
    public:
        TrieRoot(bool is_secure = false) : TrieNode<T>(), m_is_secure(is_secure) {}
        virtual ~TrieRoot() = default;

        inline void store(ByteSet<NIBBLE> &key, const T* value) {
            ByteSet<NIBBLE> secure_key = m_is_secure ? key.keccak256() : key;
            TrieNode<T>::storeKV(secure_key, value); 
        }

        inline virtual const bool isRoot() const override { return true; }

    private:
        const bool m_is_secure;
};

#include <ByteSet/TrieNode.tpp>