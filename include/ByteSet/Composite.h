#pragma once
#include <ByteSet/IComponent.h>
#include <ByteSet/ByteSet.h>

class ByteSetComposite : public IComposite
{
    public:
        ByteSetComposite(const ByteSetComposite&) = delete;
        ByteSetComposite& operator=(const ByteSetComposite&) = delete;
        ByteSetComposite(ByteSetComposite&&) noexcept = default;
        ByteSetComposite& operator=(ByteSetComposite&&) noexcept = default;
        inline virtual ~ByteSetComposite() { clear(); }

        virtual const ByteSet<BYTE> serialize() const override;
        inline virtual bool isEmpty() const override{ return getChildrenCount() == 0; }
        inline virtual const IComposite* getComposite() const override { return this; }

        template<typename T>
            void create(ByteSet<BYTE> &b);
        template<typename T>
            inline void createAll(ByteSet<BYTE> &b) { while(b.byteSize()) create<T>(b); }
        template<typename T>
           inline const T* get(uint64_t index) const { return (index < m_children.size() ? dynamic_cast<const T*>(m_children[index].get()) : nullptr); }
        
        virtual void printChildren() const override;
        
        inline virtual uint64_t getChildrenCount() const override { return m_children.size(); }

        virtual void clear() override;

    protected:
        ByteSetComposite() = default;
        
        inline virtual void push_back(IComponent *f) { f->setParent(this); m_children.emplace_back(f); }

        inline virtual uint64_t getType() const { return 0; }
        inline virtual void setType(uint64_t type) { }

    private:
        std::vector<std::unique_ptr<const IComponent>> m_children;
};

class TypedByteSetComposite : public ByteSetComposite {
    public:
        virtual ~TypedByteSetComposite() = default;
        TypedByteSetComposite(TypedByteSetComposite&&) noexcept = default;
        TypedByteSetComposite& operator=(TypedByteSetComposite&&) noexcept = default;

        virtual const ByteSet<BYTE> serialize() const override;

        inline virtual uint64_t getType() const override { return m_type; }
        inline virtual void setType(uint64_t type) override { m_type = type; }
    
    protected:
        TypedByteSetComposite() : m_type(0) {}  // 0 = Legacy (without type)

    private:
        int64_t m_type;
};

#include <ByteSet/Composite.tpp>