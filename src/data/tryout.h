#pragma once 

#include <data/ByteSet.h>

class Composite;
class CompositeParsingStrategy;
class CompositeRLPParsing;

class FieldList {
    public:
        virtual ~FieldList() = default;

        virtual const Composite* getComposite() { return 0; }

        virtual void push_back(shared_ptr<const FieldList> f) = 0;
        //virtual void pop_back() = 0;

    protected:
        FieldList() {}
};

class Composite : public FieldList {
    public:
        virtual ~Composite() {}

        virtual const Composite* getComposite() override { return this; }

        virtual void push_back(shared_ptr<const FieldList> f) override { m_fieldlist.push_back(f); }
        //virtual void pop_back() override { m_fieldlist.pop_back(); }

        shared_ptr<const FieldList> getItem(uint64_t index = 0) const { return (index < m_fieldlist.size() ? m_fieldlist[index] : nullptr); }

    protected:
        Composite(ByteSet<> &b);        //consumes b

        vector<shared_ptr<const FieldList>> m_fieldlist;
        CompositeParsingStrategy* const m_parser;
};

//----------------------------------------------- LIST / LEAF ---------------------------------------------------

class List : public Composite {
    public:
        List(ByteSet<> &b) : Composite(b) {}    //consumes b
        virtual ~List() {};

        virtual void push_back(shared_ptr<const FieldList> f) override { /*raise exception*/ }
        //virtual void pop_back() override { /*raise exception*/ }
};

class Field : public FieldList {
    public:
        Field(const ByteSet<> &b) : FieldList(), value(b) {}
        virtual ~Field() {};

        virtual void push_back(shared_ptr<const FieldList> f) override { /*raise exception*/ }
        //virtual void pop_back() override { /*raise exception*/ }

        const ByteSet<>& getValue() const { return value; }

    private:
        const ByteSet<> value;
};

//------------------------------------------ ETHEREUM COMPOSITES ---------------------------------------------

class BlockHeader : public Composite {
    public:
        BlockHeader(shared_ptr<const Composite> c) : Composite(*c) {}
        //BlockHeader(ByteSet<> &b) : Composite(b) {}
        virtual ~BlockHeader() {}

        const ByteSet<> *getParentHash() const {
            auto f = dynamic_pointer_cast<const Field>(getItem(0));   
            return (f ? &(f->getValue()) : nullptr);
        }
};

class Withdrawal : public Composite {
    public:
        Withdrawal(shared_ptr<const Composite> c) : Composite(*c) {}
        virtual ~Withdrawal() {};

        const ByteSet<>* getIndex() const {
            auto f = dynamic_pointer_cast<const Field>(getItem(0));   
            return (f ? &(f->getValue()) : nullptr);
        }
        const ByteSet<>* getValidatorIndex() const {
            auto f = dynamic_pointer_cast<const Field>(getItem(1));   
            return (f ? &(f->getValue()) : nullptr);
        }
        const ByteSet<>* getAddress() const {
            auto f = dynamic_pointer_cast<const Field>(getItem(2));   
            return (f ? &(f->getValue()) : nullptr);
        }
        const ByteSet<>* getAmount() const {
            auto f = dynamic_pointer_cast<const Field>(getItem(3));   
            return (f ? &(f->getValue()) : nullptr);
        }
};

class Withdrawals : public Composite {
    public:
        Withdrawals(shared_ptr<const Composite> c) : Composite(*c) {}
        virtual ~Withdrawals() {}

        Withdrawal* getWithdrawal(uint64_t index) const {
            auto w = dynamic_pointer_cast<const List>(getItem(index));   
            return new Withdrawal(w);
        }
};

class Block : public Composite {
    public:
        Block(ByteSet<> &b) : Composite(b) {}    // ByteSet<> is copied here to preserve the original
        virtual ~Block() {};

         BlockHeader* getHeader() {
            auto block = dynamic_pointer_cast<const List>(getItem(0));
            auto header = dynamic_pointer_cast<const List>(block->getItem(0));
            return new BlockHeader(header);
        }

         Withdrawals* getWithdrawals() {
            auto block = dynamic_pointer_cast<const List>(getItem(0));
            auto w = dynamic_pointer_cast<const List>(block->getItem(3));
            return new Withdrawals(w);
        }
};
