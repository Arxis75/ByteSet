#pragma once
#include <ByteSet/ByteSet.h>

class IByteSetComposite;

class IByteSetComponent
{
    public:
        virtual ~IByteSetComponent() = default;

        virtual void RLPparse(ByteSet<BYTE> &b) = 0;
        virtual const ByteSet<BYTE> RLPserialize() const = 0;

        inline virtual const IByteSetComposite* getComposite() const { return nullptr; }

        const IByteSetComposite* getParent() const { return m_parent; }
        void setParent(const IByteSetComposite* p) { m_parent = p; }

    protected:
        IByteSetComponent() : m_parent(nullptr) {}

    private:
        const IByteSetComposite* m_parent;
};

class IByteSetComposite : public virtual IByteSetComponent
{
    public:
        virtual ~IByteSetComposite() = default;
        inline virtual const IByteSetComposite* getComposite() const override { return this; }
        inline virtual uint64_t getChildrenCount() const = 0;
        virtual void DumpChildren() const = 0;
    protected:
        IByteSetComposite() = default;
        virtual void deleteChildren() = 0;
};

class ByteSetComposite : public virtual IByteSetComposite
{
    public:
        ByteSetComposite(const ByteSetComposite&) = delete;
        ByteSetComposite& operator=(const ByteSetComposite&) = delete;
        virtual ~ByteSetComposite() { deleteChildren(); }

        virtual void RLPparse(ByteSet<BYTE> &b) override = 0;
        virtual const ByteSet<BYTE> RLPserialize() const override;

        template<typename T>
            void create(ByteSet<BYTE> &b);
        template<typename T>
            inline void createAll(ByteSet<BYTE> &b) { while(b.byteSize()) create<T>(b); }
        template<typename T>
           inline const T* get(uint64_t index) const { return (index < m_children.size() ? dynamic_cast<const T*>(m_children[index].get()) : nullptr); }
        
        void DumpChildren() const override;
        
        inline uint64_t getChildrenCount() const override { return m_children.size(); }

    protected:
        ByteSetComposite() = default;
        
        inline virtual void push_back(IByteSetComponent *f) { f->setParent(this); m_children.emplace_back(f); }

        inline virtual uint64_t getType() const { return 0; }
        inline virtual void setType(uint64_t type) { }
        
        virtual void deleteChildren() override;

    private:
        std::vector<std::unique_ptr<const IByteSetComponent>> m_children;
};

class TypedByteSetComposite : public ByteSetComposite {
    public:
        virtual ~TypedByteSetComposite() = default;

        virtual const ByteSet<BYTE> RLPserialize() const override;

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
        
        inline virtual void RLPparse(ByteSet<BYTE> &b) override { m_value = std::make_unique<ByteSet<BYTE>>(b); }
        inline virtual const ByteSet<BYTE> RLPserialize() const override { return m_value->RLPserialize(false); } //by copy

        inline const ByteSet<BYTE>& getValue() const { return *m_value.get(); }
        inline const Integer getIntValue() const { return m_value->getNbElements() ? m_value->asInteger() : Integer::zero; }

        //inline void setValue(std::unique_ptr<ByteSet<BYTE>> &b) { return m_value.reset(b.release()); }
        //inline std::unique_ptr<ByteSet<BYTE>> takeValue() { return std::move(m_value); }

    private:
        std::unique_ptr<ByteSet<BYTE>> m_value;
};

#include <ByteSet/ByteSetComposite.tpp>