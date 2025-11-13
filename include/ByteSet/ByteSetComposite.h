#pragma once
#include <ByteSet/ByteSet.h>

class IByteSetComposite;

class IByteSetContainer
{
    public:
        virtual ~IByteSetContainer() = default;

        template<typename T = const IByteSetComposite*>
            inline T getParent() const { return dynamic_cast<T>(m_parent); }
        inline void setParent(const IByteSetComposite* p) { m_parent = p; }

    protected:
        IByteSetContainer() : m_parent(nullptr) {}

    private:
        const IByteSetComposite* m_parent;
};

class IByteSetComponent: public virtual IByteSetContainer
{
    public:
        virtual ~IByteSetComponent() = default;

        inline virtual void RLPparse(ByteSet<BYTE> &b) = 0;
        inline virtual const ByteSet<BYTE> RLPSerialize() const = 0;
        inline virtual const IByteSetComposite* getComposite() const { return nullptr; }
};


class IByteSetComposite: public virtual IByteSetContainer
{
    public:
        virtual ~IByteSetComposite() = default;

        inline virtual void dumpChildren() const = 0;
        inline virtual uint64_t getChildrenCount() const = 0;
};

class ByteSetComposite : public IByteSetComponent, public IByteSetComposite
{
    public:
        ByteSetComposite(const ByteSetComposite&) = delete;
        ByteSetComposite& operator=(const ByteSetComposite&) = delete;
        ByteSetComposite(ByteSetComposite&&) noexcept = default;
        ByteSetComposite& operator=(ByteSetComposite&&) noexcept = default;
        inline virtual ~ByteSetComposite() { reset(); }

        virtual const ByteSet<BYTE> RLPSerialize() const override;
        inline virtual const IByteSetComposite* getComposite() const override { return this; }

        template<typename T>
            void create(ByteSet<BYTE> &b);
        template<typename T>
            inline void createAll(ByteSet<BYTE> &b) { while(b.byteSize()) create<T>(b); }
        template<typename T>
           inline const T* get(uint64_t index) const { return (index < m_children.size() ? dynamic_cast<const T*>(m_children[index].get()) : nullptr); }
        
        virtual void dumpChildren() const override;
        
        inline virtual uint64_t getChildrenCount() const override { return m_children.size(); }

    protected:
        ByteSetComposite() = default;
        
        inline virtual void push_back(IByteSetComponent *f) { f->setParent(this); m_children.emplace_back(f); }

        inline virtual uint64_t getType() const { return 0; }
        inline virtual void setType(uint64_t type) { }
        
        virtual void reset();

    private:
        std::vector<std::unique_ptr<const IByteSetComponent>> m_children;
};

class TypedByteSetComposite : public ByteSetComposite {
    public:
        virtual ~TypedByteSetComposite() = default;
        TypedByteSetComposite(TypedByteSetComposite&&) noexcept = default;
        TypedByteSetComposite& operator=(TypedByteSetComposite&&) noexcept = default;

        virtual const ByteSet<BYTE> RLPSerialize() const override;

        inline virtual uint64_t getType() const override { return m_type; }
        inline virtual void setType(uint64_t type) override { m_type = type; }
    
    protected:
        TypedByteSetComposite() : m_type(0) {}  // 0 = Legacy (without type)

    private:
        int64_t m_type;
};

//----------------------------------------------- LEAF ---------------------------------------------------

class ByteSetField : public virtual IByteSetComponent {
    public:
        ByteSetField() : m_value(nullptr) {}
        ByteSetField(const ByteSetField&) = delete;
        ByteSetField& operator=(const ByteSetField&) = delete;
        virtual ~ByteSetField() = default;
        
        inline virtual void RLPparse(ByteSet<BYTE> &b) override { m_value = std::make_unique<ByteSet<BYTE>>(b); } //might call deleter
        inline virtual const ByteSet<BYTE> RLPSerialize() const override { return m_value->RLPSerialize(false); } //by copy

        inline const ByteSet<BYTE>& getValue() const { return *m_value.get(); }
        inline const Integer getIntValue() const { return m_value->getNbElements() ? m_value->asInteger() : Integer::zero; }

    private:
        std::unique_ptr<ByteSet<BYTE>> m_value;
};

#include <ByteSet/ByteSetComposite.tpp>