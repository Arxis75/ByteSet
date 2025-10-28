#pragma once
#include <data/ByteSet.h>

class ByteSetComposite;

class IByteSetContainer
{
    public:
        virtual ~IByteSetContainer() = default;

        virtual const ByteSetComposite* getComposite() const { return 0; }
        virtual void push_back(shared_ptr<const IByteSetContainer> f) = 0;
        virtual void RLPparse(ByteSet<8> &b) = 0;
        virtual ByteSet<8> RLPserialize() const = 0;

    protected:
        IByteSetContainer(ByteSetComposite* p) : m_parent(p) {}

    private:
        ByteSetComposite* m_parent;
};

class ByteSetComposite : public IByteSetContainer
{
    public:
        virtual ~ByteSetComposite() = default;

        static shared_ptr<IByteSetContainer> Factory(ByteSetComposite *parent, bool is_composite);

        virtual const ByteSetComposite* getComposite() const override { return this; }
        virtual void push_back(shared_ptr<const IByteSetContainer> f) override { m_children.push_back(f); }
        virtual void RLPparse(ByteSet<8> &b) override;
        virtual ByteSet<8> RLPserialize() const override;

    protected:
        ByteSetComposite(ByteSetComposite* p) : IByteSetContainer(p) {}

    private:
        vector<shared_ptr<const IByteSetContainer>> m_children;
};

//----------------------------------------------- LEAF ---------------------------------------------------

class ByteSetField : public IByteSetContainer {
    public:
        ByteSetField(ByteSetComposite* p) : IByteSetContainer(p) {}
        virtual ~ByteSetField() = default;

        inline virtual void push_back(shared_ptr<const IByteSetContainer> f) override { /*raise exception*/ }
        inline virtual void RLPparse(ByteSet<8> &b) override { m_value = b; }
        inline virtual ByteSet<8> RLPserialize() const override { return ByteSet<8>(m_value).RLPserialize(false); } //by copy

        private:
        ByteSet<8> m_value;
};

inline shared_ptr<IByteSetContainer>ByteSetComposite::Factory(ByteSetComposite *parent, bool is_composite) {
    shared_ptr<IByteSetContainer> child;
    if(is_composite)
        child = std::shared_ptr<ByteSetComposite>(new ByteSetComposite(parent));
    else
        child = make_shared<ByteSetField>(parent);
    parent->push_back(child);
    return child;
}