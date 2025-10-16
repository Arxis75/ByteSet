#pragma once 

#include <data/ByteSet.h>

class Composite;
class CompositeParsingStrategy;
class CompositeRLPParsing;

class FieldList {
    public:
        virtual ~FieldList() = default;

        virtual const Composite* getComposite() { return 0; }

        virtual void push_back(FieldList *f) = 0;
        //virtual void pop_back() = 0;

    protected:
        FieldList() {}
};

class Composite : public FieldList {
    public:
        virtual ~Composite() {}

        virtual const Composite* getComposite() override { return this; }

        virtual void push_back(FieldList *f) override { m_fieldlist.push_back(f); }
        //virtual void pop_back() override { m_fieldlist.pop_back(); }

        FieldList* getItem(uint64_t index = 0) { return (index < m_fieldlist.size() ? m_fieldlist[index] : nullptr); }

    protected:
        Composite(ByteSet<> &b);   //consumes b

        vector<FieldList*> m_fieldlist;
        const CompositeParsingStrategy *m_parser;
};

//----------------------------------------------- LIST / LEAF ---------------------------------------------------

class List : public Composite {
    public:
        List(ByteSet<> &b) : Composite(b) {}
        virtual ~List() {};

        virtual void push_back(FieldList *f) override { /*raise exception*/ }
        //virtual void pop_back() override { /*raise exception*/ }
};

class Field : public FieldList {
    public:
        Field(const ByteSet<> &b) : FieldList(), value(b) {}
        virtual ~Field() {};

        virtual void push_back(FieldList *f) override { /*raise exception*/ }
        //virtual void pop_back() override { /*raise exception*/ }

        const ByteSet<>& getValue() const { return value; }

    private:
        const ByteSet<> value;
};

//------------------------------------------ ETHEREUM COMPOSITES ---------------------------------------------

class BlockHeader : public Composite {
    public:
        BlockHeader(Composite *c) : Composite(*c) {}
        //BlockHeader(ByteSet<> &b) : Composite(b) {}
        virtual ~BlockHeader() {}

        const ByteSet<> *getParentHash() {
            auto f = dynamic_cast<Field*>(getItem(0));   
            return (f ? &(f->getValue()) : nullptr);
        }
};

class Withdrawal : public Composite {
    public:
        Withdrawal(const Composite *c) : Composite(*c) {}
        virtual ~Withdrawal() {};

        const ByteSet<>* getIndex() {
            auto f = dynamic_cast<const Field*>(getItem(0));   
            return (f ? &(f->getValue()) : nullptr);
        }
        const ByteSet<>* getValidatorIndex() {
            auto f = dynamic_cast<const Field*>(getItem(1));   
            return (f ? &(f->getValue()) : nullptr);
        }
        const ByteSet<>* getAddress() {
            auto f = dynamic_cast<const Field*>(getItem(2));   
            return (f ? &(f->getValue()) : nullptr);
        }
        const ByteSet<>* getAmount() {
            auto f = dynamic_cast<const Field*>(getItem(3));   
            return (f ? &(f->getValue()) : nullptr);
        }
};

class Withdrawals : public Composite {
    public:
        Withdrawals(const Composite *c) : Composite(*c) {}
        virtual ~Withdrawals() {}

        Withdrawal* getWithdrawal(uint64_t index) {
            List* w = dynamic_cast<List*>(getItem(index));   
            return new Withdrawal(w);
        }
};

class Block : public Composite {
    public:
        Block(ByteSet<> b) : Composite(b) {}
        virtual ~Block() {};

         BlockHeader* getHeader() {
            List* block = dynamic_cast<List*>(getItem(0));
            List* header = dynamic_cast<List*>(block->getItem(0));
            return new BlockHeader(header);
        }

         Withdrawals* getWithdrawals() {
            List* block = dynamic_cast<List*>(getItem(0));
            List* w = dynamic_cast<List*>(block->getItem(3));
            return new Withdrawals(w);
        }
};
