#pragma once
#include <cstdint>

using uint = unsigned int;

enum BitsPerElem {ONE = 1, FOUR = 4, EIGHT = 8};

constexpr BitsPerElem BIT = BitsPerElem::ONE;
constexpr BitsPerElem NIBBLE = BitsPerElem::FOUR;
constexpr BitsPerElem BYTE = BitsPerElem::EIGHT;

class IComposite;
template <BitsPerElem> class ByteSet;

class IComponent
{
    public:
        virtual ~IComponent() = default;

        /******************************** ICOMPONENT INTERFACE **************************/
        inline virtual const IComposite* getComposite() const { return nullptr; }
        inline virtual void parse(ByteSet<BYTE> &b) = 0;
        inline virtual const ByteSet<BYTE> serialize() const = 0;
        inline virtual void print() const { /*TODO*/ };
        inline virtual bool isEmpty() const = 0;
        inline virtual void clear() = 0;
        /********************************************************************************/

        inline const IComposite* getParent() const { return m_parent; }
        inline void setParent(const IComposite* p) { m_parent = p; }

    protected:
        IComponent() : m_parent(nullptr) {}

    private:
        const IComposite* m_parent;
};


class IComposite: virtual public IComponent
{
    public:
        IComposite(const IComposite&) = delete;
        IComposite& operator=(const IComposite&) = delete;
        IComposite(IComposite&&) noexcept = default;
        IComposite& operator=(IComposite&&) noexcept = default;
        virtual ~IComposite() = default;
        
        //******************************* ICOMPONENT INTERFACE ****************************
        inline virtual const IComposite* getComposite() const override { return this; }
        virtual void parse(ByteSet<BYTE> &b) override;
        virtual const ByteSet<BYTE> serialize() const override;
        virtual void print() const override;
        inline virtual bool isEmpty() const override{ return getChildrenCount() == 0; }
        inline virtual void clear() override = 0;
        //********************************************************************************

        //******************************* ICOMPOSITE INTERFACE ****************************
        inline virtual IComponent* newChild(uint creation_index = 0) = 0;
        inline virtual void addChild(IComponent *child, const ByteSet<NIBBLE>& key)= 0;
        inline virtual const IComponent* getChild(uint child_index) const = 0;
        inline virtual uint getChildrenCount() const = 0;
        inline virtual uint getChildrenContainerSize() const = 0;
        //********************************************************************************

        template<typename T>
           inline const T* get(uint child_index) const { return dynamic_cast<const T*>(getChild(child_index)); }
   
        inline virtual uint getTyped() const { return m_typed; }
        inline virtual void setTyped(uint typed) { m_typed = typed; }

    protected:
        IComposite()  : m_typed(0) {}  // 0 = Legacy (without type)

    private:
        uint m_typed;
};
