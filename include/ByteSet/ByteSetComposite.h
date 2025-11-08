#pragma once
#include <ByteSet/ByteSet.h>

class ByteSetComposite;
class ByteSetField;

class IByteSetContainer
{
    public:
        virtual ~IByteSetContainer() = default;

        virtual void RLPparse(ByteSet<BYTE> &b) = 0;
        virtual const ByteSet<BYTE> RLPserialize() const = 0;

        inline virtual const ByteSetComposite* getComposite() const { return nullptr; }

        const ByteSetComposite* getParent() const { return m_parent; }
        void setParent(const ByteSetComposite* p) { m_parent = p; }

    protected:
        IByteSetContainer() = default;

        virtual void push_back(IByteSetContainer* f) = 0;
        inline virtual uint64_t getType() const { return 0; };
        inline virtual void setType(uint64_t type) {}

    private:
        const ByteSetComposite* m_parent;
};

class ByteSetComposite : public virtual IByteSetContainer
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
        
        void DumpChildren() const;
        
        inline virtual const ByteSetComposite* getComposite() const override { return this; }
        inline uint64_t getChildrenCount() const { return m_children.size(); }

    protected:
        ByteSetComposite() = default;
        
        inline virtual void push_back(IByteSetContainer *f) override { f->setParent(this); m_children.emplace_back(f); }
        
        void deleteChildren();

    private:
        std::vector<std::unique_ptr<const IByteSetContainer>> m_children;
};

class TypedByteSetComposite : public ByteSetComposite {
    public:
        virtual ~TypedByteSetComposite() = default;

        virtual const ByteSet<BYTE> RLPserialize() const override;

        inline virtual uint64_t getType() const override { return m_type; }
        inline virtual void setType(uint64_t type) override { m_type = type; }
    
    protected:
        TypedByteSetComposite() = default;

    private:
        int64_t m_type;
};

//----------------------------------------------- LEAF ---------------------------------------------------

class ByteSetField : public virtual IByteSetContainer {
    public:
        ByteSetField() = default;
        ByteSetField(const ByteSetField&) = delete;
        ByteSetField& operator=(const ByteSetField&) = delete;
        virtual ~ByteSetField() = default;
        
        inline virtual void RLPparse(ByteSet<BYTE> &b) override { m_value = std::make_unique<ByteSet<BYTE>>(b); }
        inline virtual const ByteSet<BYTE> RLPserialize() const override { return m_value->RLPserialize(false); } //by copy

        inline const ByteSet<BYTE>& getValue() const { return *m_value.get(); }
        inline const Integer getIntValue() const { return m_value->getNbElements() ? m_value->asInteger() : Integer::zero; }

        inline void setValue(std::unique_ptr<ByteSet<BYTE>> &b) { return m_value.reset(b.release()); }
        inline std::unique_ptr<ByteSet<BYTE>> takeValue() { return std::move(m_value); }
    
    protected:
        virtual void push_back(IByteSetContainer *b) override { assert(false); }

    private:
        std::unique_ptr<ByteSet<BYTE>> m_value;
};

#include <ByteSet/ByteSetComposite.tpp>