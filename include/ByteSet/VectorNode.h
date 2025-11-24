#pragma once
#include <ByteSet/IComposite.h>
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

class VectorNode : public IComposite
{
    public:
        inline virtual ~VectorNode() = default;

        //*********************************** ICOMPONENT INTERFACE ************************************************
        virtual void print() const override;
        virtual void clear() override { m_children.clear(); }
        //**********************************************************************************************************

         //*********************************** ICOMPOSITE INTERFACE ************************************************
        virtual IComponent* newChild(uint creation_index = 0) override = 0;
        inline virtual void addChild(uint child_index, IComponent *child) override { push_back(child); }
        inline virtual IComponent* removeChild(uint child_index) override { return pop_front(); }
        virtual const IComponent* getChild(uint child_index) const override { return (child_index < m_children.size() ? m_children[child_index].get() : nullptr); }
        inline virtual uint getChildrenCount() const override { return m_children.size(); }
        //**********************************************************************************************************

        inline void push_back(IComponent* child) { child->setParent(this); m_children.emplace_back(child); }
        IComponent* pop_front();

    protected:
        VectorNode() = default;

    private:
        vector<unique_ptr<const IComponent>> m_children;
};