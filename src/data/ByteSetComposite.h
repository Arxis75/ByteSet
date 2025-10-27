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
        IByteSetContainer() = default;    
};

class ByteSetComposite : public IByteSetContainer
{
    public:
        ByteSetComposite(const ByteSetComposite&) = default;
        virtual ~ByteSetComposite() = default;

        virtual const ByteSetComposite* getComposite() const override { return this; }
        virtual void push_back(shared_ptr<const IByteSetContainer> f) override { m_children.push_back(f); }
        inline virtual void RLPparse(ByteSet<8> &b) override = 0;
        inline virtual ByteSet<8> RLPserialize() const override = 0;

    protected:
        ByteSetComposite() = default;
        //ByteSetComposite(ByteSet<8> &b) : IByteSetContainer() {}      

    protected:
        vector<shared_ptr<const IByteSetContainer>> m_children;
};

//----------------------------------------------- LIST / LEAF ---------------------------------------------------

class ByteSetList : public ByteSetComposite {
    public:
        ByteSetList() = default;
        //ByteSetList(ByteSet<8> &b) : ByteSetComposite(b) {}
        virtual ~ByteSetList() = default;

        virtual void RLPparse(ByteSet<8> &b) override;
        virtual ByteSet<8> RLPserialize() const override;
};

class ByteSetField : public IByteSetContainer {
    public:
        ByteSetField() = default;
        ByteSetField(ByteSet<8> b) : IByteSetContainer(), m_value(b) {}
        virtual ~ByteSetField() = default;

        inline virtual void push_back(shared_ptr<const IByteSetContainer> f) override { /*raise exception*/ }
        inline virtual void RLPparse(ByteSet<8> &b) override { m_value = b; }
        inline virtual ByteSet<8> RLPserialize() const override {
            return ByteSet<8>(m_value).RLPserialize(false);
        }

        //------------------------------------ Accessors ------------------------------------------

        inline const RLPType getRLPType() const { return m_value.getRLPType(); }

        inline const ByteSet<8>& getStrValue() const { return m_value; }
        inline const Integer getIntValue() const { return m_value.getNbElements() ? m_value.asInteger() : Integer::zero; }
        inline virtual void setStrValue(const ByteSet<8> &b) { m_value = b; }
        inline virtual void setIntValue(const Integer &i) { m_value = (i == Integer::zero ? ByteSet<8>() : ByteSet<8>(i)); }

        private:
        ByteSet<8> m_value;
};