#pragma once
#include <data/ByteSet.h>

class ByteSetComposite;
class ByteSetField;

class IByteSetContainer
{
    public:
        virtual ~IByteSetContainer() = default;

        virtual void RLPparse(ByteSet<8> &b) = 0;
        virtual const ByteSet<8> RLPserialize() const = 0;
        virtual void push_back(const IByteSetContainer* f) = 0;

        inline virtual const ByteSetComposite* getComposite() const { return nullptr; }
        inline virtual const bool isComposite() const { return getComposite(); }

        const ByteSetComposite* getParent() const { return m_parent; }
        void setParent(const ByteSetComposite* p) { m_parent = p; }

    protected:
        IByteSetContainer() = default;

    private:
        const ByteSetComposite* m_parent;
};

class ByteSetComposite : public virtual IByteSetContainer
{
    public:
        ByteSetComposite(const ByteSetComposite&) = delete;
        ByteSetComposite& operator=(const ByteSetComposite&) = delete;
        virtual ~ByteSetComposite() = default;

        virtual void RLPparse(ByteSet<8> &b) override;
        uint64_t RLPparseTypedChildAt(uint64_t child_index);
        virtual const ByteSet<8> RLPserialize() const override;
        inline virtual void push_back(const IByteSetContainer *f) override { m_children.emplace_back(f); }

        IByteSetContainer* newChild(bool is_composite);
        inline const IByteSetContainer* getChildAt(uint64_t index) const { return (index < m_children.size() ? m_children[index].get() : nullptr); }
        inline unique_ptr<const IByteSetContainer> takeChildAt(uint64_t index) { return (index < m_children.size() ? std::move(m_children[index]) : nullptr); }
        void moveChildAt_To(uint64_t child_index, unique_ptr<IByteSetContainer>& target);           //the call does not move ownership of target
        inline uint64_t getChildrenCount() const { return m_children.size(); }
        void DumpChildren() const;
        void deleteChildren();
        
        inline virtual const ByteSetComposite* getComposite() const override { return this; }

    protected:
        ByteSetComposite() = default;

    private:
        vector<unique_ptr<const IByteSetContainer>> m_children;
};

//----------------------------------------------- LEAF ---------------------------------------------------

class ByteSetField : public virtual IByteSetContainer {
    public:
        ByteSetField() = default;
        virtual ~ByteSetField() = default;
        
        inline virtual void RLPparse(ByteSet<8> &b) override { m_value = make_unique<ByteSet<8>>(b); }
        inline virtual const ByteSet<8> RLPserialize() const override { return m_value->RLPserialize(false); } //by copy
        virtual void push_back(const IByteSetContainer *b) override;

        inline const ByteSet<8>& getValue() const { return *m_value.get(); }
        inline const Integer getIntValue() const { return m_value->getNbElements() ? m_value->asInteger() : Integer::zero; }

        inline void setValue(unique_ptr<ByteSet<8>> &b) { return m_value.reset(b.release()); }
        inline unique_ptr<ByteSet<8>> takeValue() { return std::move(m_value); }

    private:
        unique_ptr<ByteSet<8>> m_value;
};

#include <data/ByteSetComposite.tpp>