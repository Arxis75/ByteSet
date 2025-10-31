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

        virtual const ByteSetComposite* getComposite() const { return 0; }
        
        const ByteSetComposite* getParent() const { return m_parent; }
        void setParent(const ByteSetComposite* p) { m_parent = p; }

        //virtual unique_ptr<const IByteSetContainer> clone() const = 0;
    protected:
        IByteSetContainer() = default;

        virtual void push_back(const IByteSetContainer *f) = 0;

    private:
        const ByteSetComposite* m_parent;
};

class ByteSetComposite : public virtual IByteSetContainer
{
    public:
        ByteSetComposite(const ByteSetComposite&) = delete;
        ByteSetComposite& operator=(const ByteSetComposite&) = delete;
        virtual ~ByteSetComposite() = default;

        inline virtual const ByteSetComposite* getComposite() const override { return this; }

        inline virtual const IByteSetContainer* getItem(uint64_t index = 0) const { return (index < m_children.size() ? m_children[index].get() : nullptr); }
        inline unique_ptr<const IByteSetContainer> takeItem(uint64_t index = 0)
            { return (index < m_children.size() ? std::move(m_children[index]) : nullptr); }

        virtual void RLPparse(ByteSet<8> &b) override;
        virtual const ByteSet<8> RLPserialize() const override;

        inline IByteSetContainer* makeChild(bool is_composite);
        inline virtual void push_back(const IByteSetContainer *f) override { m_children.emplace_back(f); }
        inline void move_back(unique_ptr<const IByteSetContainer> p) { m_children.emplace_back(std::move(p)); }
        inline uint64_t getChildrenCount() const { return m_children.size(); }
        void DumpChildren() const;
        void deleteChildren();

    protected:
        ByteSetComposite() = default;
        ByteSetComposite(ByteSetComposite&&) noexcept = default;
        ByteSetComposite& operator=(ByteSetComposite&&) noexcept = default;

        template<typename T> std::unique_ptr<T> takeChildrenAs(uint list_index);
        std::unique_ptr<ByteSetField> takeField(uint field_index);

        //virtual unique_ptr<const IByteSetContainer> clone() const override;

    private:
        vector<unique_ptr<const IByteSetContainer>> m_children;
};

//----------------------------------------------- LEAF ---------------------------------------------------

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
        //virtual unique_ptr<const IByteSetContainer> clone() const override  { return make_unique<ByteSetField>(*this); /*deep copy*/ }

    private:
        ByteSet<8> m_value;
};

#include <data/ByteSetComposite.tpp>