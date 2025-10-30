#pragma once
#include <data/ByteSet.h>

class ByteSetComposite;

class IByteSetContainer
{
    public:
        virtual ~IByteSetContainer() = default;
        virtual void RLPparse(ByteSet<8> &b) = 0;
        virtual const ByteSet<8> RLPserialize() const = 0;

        virtual const ByteSetComposite* getComposite() const { return 0; }
        
        const ByteSetComposite* getParent() const { return m_parent; }
        void setParent(const ByteSetComposite* p) { m_parent = p; }

        virtual void push_back(const IByteSetContainer *f) = 0;
        virtual unique_ptr<const IByteSetContainer> clone() const = 0;
    protected:
        IByteSetContainer() = default;

    private:
        const ByteSetComposite* m_parent;
};

class ByteSetComposite : public virtual IByteSetContainer
{
    public:
        ByteSetComposite(const ByteSetComposite &other);
        virtual ~ByteSetComposite() { DumpChildren(); };

        virtual const ByteSetComposite* getComposite() const override { return this; }
        const IByteSetContainer* getItem(uint64_t index = 0) const { return (index < m_children.size() ? m_children[index].get() : nullptr); }

        virtual void RLPparse(ByteSet<8> &b) override;
        virtual const ByteSet<8> RLPserialize() const override;

    protected:
        ByteSetComposite() = default;

        IByteSetContainer* makeChild(bool is_composite);
        uint64_t getChildrenCount() const { return m_children.size(); }
        void DumpChildren() const;
        //void deleteChildren();

        virtual void push_back(const IByteSetContainer *f) override { m_children.push_back(f->clone()); }
    
        //virtual unique_ptr<const IByteSetContainer> clone() const override { return nullptr; }
        virtual unique_ptr<const IByteSetContainer> clone() const override { return make_unique<const ByteSetComposite>(*this); }

    private:
        vector<unique_ptr<const IByteSetContainer>> m_children;
};

//----------------------------------------------- LEAF ---------------------------------------------------

class List : public ByteSetComposite {
    public:
        List() = default;
        virtual ~List() = default;
};

class ByteSetField : public virtual IByteSetContainer {
    public:
        ByteSetField() = default;
        ByteSetField(const IByteSetContainer *p) : IByteSetContainer(*p) {}  //deep copy constructor from ptr
        virtual ~ByteSetField() = default;
        
        inline virtual void RLPparse(ByteSet<8> &b) override { m_value = b; }
        inline virtual const ByteSet<8> RLPserialize() const override { return m_value.RLPserialize(false); } //by copy

        inline const ByteSet<8>& getValue() const { return m_value; }     
        inline const Integer getIntValue() const { return m_value.getNbElements() ? m_value.asInteger() : Integer::zero; }

    protected:
        inline virtual void push_back(const IByteSetContainer *f) override { /*raise exception*/ }
        virtual unique_ptr<const IByteSetContainer> clone() const override  { return make_unique<ByteSetField>(*this); /*deep copy*/ }

    private:
        ByteSet<8> m_value;
};