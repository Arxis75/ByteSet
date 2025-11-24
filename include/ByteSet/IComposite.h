#pragma once
#include <cassert>

using uint = unsigned int;

enum BitsPerElem {ONE = 1, FOUR = 4, EIGHT = 8};

constexpr BitsPerElem BIT = BitsPerElem::ONE;
constexpr BitsPerElem NIBBLE = BitsPerElem::FOUR;
constexpr BitsPerElem BYTE = BitsPerElem::EIGHT;

class IComposite;
template <BitsPerElem> class ByteSet;
template <BitsPerElem> class RLPByteSet;

class IComponent
{
    public:
        virtual ~IComponent() = default;

        /******************************** ICOMPONENT INTERFACE **************************/
        inline virtual const IComposite* getComposite() const { return nullptr; }
        inline virtual void parse(RLPByteSet<BYTE> &b) = 0;
        virtual const ByteSet<BYTE> getValue() const = 0;
        inline virtual const RLPByteSet<BYTE> serialize() const = 0;
        inline virtual void print() const = 0;
        inline virtual void clear() = 0;
        /********************************************************************************/

        inline const IComposite* getParent() const { return m_parent; }
        inline void setParent(const IComposite* p) { m_parent = p; }

    protected:
        IComponent() : m_parent(nullptr) {}

    private:
        const IComposite* m_parent;
};

class IcompositeIterator;

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
        virtual void parse(RLPByteSet<BYTE> &b) override;
        virtual const ByteSet<BYTE> getValue() const override;
        virtual const RLPByteSet<BYTE> serialize() const override;
        virtual void print() const override = 0;
        inline virtual void clear() override = 0;
        //********************************************************************************

        //******************************* ICOMPOSITE INTERFACE ****************************
        inline virtual IComponent* newChild(uint creation_index = 0) = 0;
        inline virtual void addChild(uint child_index, IComponent *child) = 0;
        inline virtual IComponent* removeChild(uint child_index) = 0;
        inline virtual const IComponent* getChild(uint child_index) const = 0;
        inline virtual uint getChildrenCount() const = 0;
        //********************************************************************************

        template<typename T>
           inline const T* get(uint child_index) const { return dynamic_cast<const T*>(getChild(child_index)); }

        inline IComposite* removeBrackets() {
            assert(getChildrenCount() == 1 && getChild(0)->getComposite());
            return dynamic_cast<IComposite*>(removeChild(0)); 
        }

        inline virtual uint getTyped() const { return m_typed; }
        inline virtual void setTyped(uint typed) { m_typed = typed; }

    protected:
        IComposite()  : m_typed(0) {}  // 0 = Legacy (without type)

    private:
        uint m_typed;
};