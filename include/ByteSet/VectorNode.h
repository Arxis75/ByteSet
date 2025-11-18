#pragma once
#include <ByteSet/IComponent.h>
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class VectorNode : public IComposite
{
    public:
        inline virtual ~VectorNode() { clear(); }

        //*********************************** ICOMPONENT INTERFACE ************************************************
        virtual void print() const override;
        virtual void clear() override;
        //**********************************************************************************************************

         //*********************************** ICOMPOSITE INTERFACE ************************************************
        virtual IComponent* newChild(uint creation_index = 0) override = 0;
        inline virtual void addChild(IComponent *child, const ByteSet<NIBBLE>& key) override { child->setParent(this); m_children.emplace_back(child); }
        virtual const IComponent* getChild(uint child_index) const override { return (child_index < m_children.size() ? m_children[child_index].get() : nullptr); }
        inline virtual uint getChildrenCount() const override { return m_children.size(); }
        inline virtual uint getChildrenContainerSize() const override { return m_children.size(); }
        //**********************************************************************************************************

    protected:
        VectorNode() = default;

    private:
        vector<unique_ptr<const IComponent>> m_children;
};