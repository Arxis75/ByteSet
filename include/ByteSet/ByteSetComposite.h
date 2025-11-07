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
        virtual void push_back(const IByteSetContainer* f) = 0;

        inline virtual const ByteSetComposite* getComposite() const { return nullptr; }
        inline virtual const bool isComposite() const { return getComposite(); }

        const ByteSetComposite* getParent() const { return m_parent; }
        void setParent(const ByteSetComposite* p) { m_parent = p; }

        inline virtual uint64_t getType() const { return 0; };
        inline virtual void setType(uint64_t type) {}

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

        virtual void RLPparse(ByteSet<BYTE> &b) override = 0;
        virtual const ByteSet<BYTE> RLPserialize() const override;
        inline virtual void push_back(const IByteSetContainer *f) override { m_children.emplace_back(f); }

        template<typename T>
            void create(ByteSet<BYTE> &b);
        template<typename T>
            inline void createAll(ByteSet<BYTE> &b) { while(b.byteSize()) create<T>(b); }
        template<typename T>
           inline const T* get(uint64_t index) const { return (index < m_children.size() ? dynamic_cast<const T*>(m_children[index].get()) : nullptr); }
        
        inline uint64_t getChildrenCount() const { return m_children.size(); }
        void DumpChildren() const;
        void deleteChildren();
        
        inline virtual const ByteSetComposite* getComposite() const override { return this; }

    protected:
        ByteSetComposite() = default;

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
        virtual void push_back(const IByteSetContainer *b) override { assert(false); }

        inline const ByteSet<BYTE>& getValue() const { return *m_value.get(); }
        inline const Integer getIntValue() const { return m_value->getNbElements() ? m_value->asInteger() : Integer::zero; }

        inline void setValue(std::unique_ptr<ByteSet<BYTE>> &b) { return m_value.reset(b.release()); }
        inline std::unique_ptr<ByteSet<BYTE>> takeValue() { return std::move(m_value); }

    private:
        std::unique_ptr<ByteSet<BYTE>> m_value;
};

#include <ByteSet/ByteSetComposite.tpp>