#pragma once
#include <cstdint>

enum BitsPerElem {ONE = 1, FOUR = 4, EIGHT = 8};

constexpr BitsPerElem BIT = BitsPerElem::ONE;
constexpr BitsPerElem NIBBLE = BitsPerElem::FOUR;
constexpr BitsPerElem BYTE = BitsPerElem::EIGHT;

template <BitsPerElem> class ByteSet;

class IComposite;
class IComponent
{
    public:
        virtual ~IComponent() = default;

        template<typename T = const IComposite*>
            inline T getParent() const { return dynamic_cast<T>(m_parent); }
        inline void setParent(const IComposite* p) { m_parent = p; }

        inline virtual void  parse(ByteSet<BYTE> &b) = 0;
        inline virtual const ByteSet<BYTE> serialize() const = 0;
        
        inline virtual bool isEmpty() const = 0;
        inline virtual void clear() = 0;

        inline virtual const IComposite* getComposite() const { return nullptr; }

    protected:
        IComponent() : m_parent(nullptr) {}

    private:
        const IComposite* m_parent;
};


class IComposite: virtual public IComponent
{
    public:
        virtual ~IComposite() = default;

        inline virtual void printChildren() const = 0;
        inline virtual uint64_t getChildrenCount() const = 0;
};