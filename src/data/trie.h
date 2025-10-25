#pragma once
#include <data/ByteSet.h>

class TrieNode {
    public:
        TrieNode();

        virtual void update(const ByteSet<4> &key, const ByteSet<> &value);
        virtual const TrieNode* lookup(const ByteSet<4> &key) const;
        virtual void del(const ByteSet<4> &key);
        virtual void iterate() const {}     //TODO

        inline const ByteSet<>& getValue() const { return m_value; }
        inline bool hasChildren() const {
            return m_children[0] || m_children[1] || m_children[2] || m_children[3] ||
                   m_children[4] || m_children[5] || m_children[6] || m_children[7] ||
                   m_children[8] || m_children[9] || m_children[10] || m_children[11] ||
                   m_children[12] || m_children[13] || m_children[14] || m_children[15];
        }

    private:
        TrieNode* m_children[16];
        ByteSet<> m_value;
};

class CompressedTrieNode
{
    public:
        CompressedTrieNode() = default;

        virtual void update(const ByteSet<4> &key, const ByteSet<> &value);
        virtual const CompressedTrieNode* lookup(const ByteSet<4> &key) const;
        virtual void del(const ByteSet<4> &key);
        virtual void iterate() const {}     //TODO

    private:
        CompressedTrieNode* m_children[16];
        ByteSet<> m_value;
};