#pragma once
#include <ByteSet/ByteSet.h>

class IComposite;

class IComponent
{
    public:
        virtual ~IComponent() = default;

        template<typename T = const IComposite*>
            inline T getParent() const { return dynamic_cast<T>(m_parent); }
        inline void setParent(const IComposite* p) { m_parent = p; }

    protected:
        IComponent() : m_parent(nullptr) {}

    private:
        const IComposite* m_parent;
};

class IRLPListComponent: public virtual IComponent
{
    public:
        virtual ~IRLPListComponent() = default;

        inline virtual void RLPparse(ByteSet<BYTE> &b) = 0;
        inline virtual const ByteSet<BYTE> RLPSerialize() const = 0;
        inline virtual const IComposite* getComposite() const { return nullptr; }
};


class IComposite: public virtual IComponent
{
    public:
        virtual ~IComposite() = default;

        inline virtual void printChildren() const = 0;
        inline virtual uint64_t getChildrenCount() const = 0;
};

class RLPListComposite : public IRLPListComponent, public IComposite
{
    public:
        RLPListComposite(const RLPListComposite&) = delete;
        RLPListComposite& operator=(const RLPListComposite&) = delete;
        RLPListComposite(RLPListComposite&&) noexcept = default;
        RLPListComposite& operator=(RLPListComposite&&) noexcept = default;
        inline virtual ~RLPListComposite() { reset(); }

        virtual const ByteSet<BYTE> RLPSerialize() const override;
        inline virtual const IComposite* getComposite() const override { return this; }

        template<typename T>
            void create(ByteSet<BYTE> &b);
        template<typename T>
            inline void createAll(ByteSet<BYTE> &b) { while(b.byteSize()) create<T>(b); }
        template<typename T>
           inline const T* get(uint64_t index) const { return (index < m_children.size() ? dynamic_cast<const T*>(m_children[index].get()) : nullptr); }
        
        virtual void printChildren() const override;
        
        inline virtual uint64_t getChildrenCount() const override { return m_children.size(); }

    protected:
        RLPListComposite() = default;
        
        inline virtual void push_back(IRLPListComponent *f) { f->setParent(this); m_children.emplace_back(f); }

        inline virtual uint64_t getType() const { return 0; }
        inline virtual void setType(uint64_t type) { }
        
        virtual void reset();

    private:
        std::vector<std::unique_ptr<const IRLPListComponent>> m_children;
};

class TypedRLPListComposite : public RLPListComposite {
    public:
        virtual ~TypedRLPListComposite() = default;
        TypedRLPListComposite(TypedRLPListComposite&&) noexcept = default;
        TypedRLPListComposite& operator=(TypedRLPListComposite&&) noexcept = default;

        virtual const ByteSet<BYTE> RLPSerialize() const override;

        inline virtual uint64_t getType() const override { return m_type; }
        inline virtual void setType(uint64_t type) override { m_type = type; }
    
    protected:
        TypedRLPListComposite() : m_type(0) {}  // 0 = Legacy (without type)

    private:
        int64_t m_type;
};

//----------------------------------------------- LEAF ---------------------------------------------------

class RLPListField : public virtual IRLPListComponent {
    public:
        RLPListField() : m_value(nullptr) {}
        RLPListField(const RLPListField&) = delete;
        RLPListField& operator=(const RLPListField&) = delete;
        virtual ~RLPListField() = default;
        
        inline virtual void RLPparse(ByteSet<BYTE> &b) override { m_value = std::make_unique<ByteSet<BYTE>>(b); } //might call deleter
        inline virtual const ByteSet<BYTE> RLPSerialize() const override { return m_value->RLPSerialize(false); } //by copy

        inline const ByteSet<BYTE>& getValue() const { return *m_value.get(); }
        inline const Integer getIntValue() const { return m_value->getNbElements() ? m_value->asInteger() : Integer::zero; }

    private:
        std::unique_ptr<ByteSet<BYTE>> m_value;
};

#include <ByteSet/Composite.tpp>