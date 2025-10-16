#pragma once

#include <data/ByteSet.h>

class FieldList;
/*class Composite;
class List;
class Field;*/

//----------------------------------------------- PARSING STRATEGY INTERFACE ---------------------------------------

class CompositeParsingStrategy {
    public:
        virtual void parse(ByteSet<8> &b, shared_ptr<const FieldList> &f) const = 0;
        //virtual ByteSet<8> serialize() const = 0;
    protected:
        CompositeParsingStrategy() {}
};

//---------------------------------------------------- RLP STRATEGY ------------------------------------------------

class CompositeRLPParsing : public CompositeParsingStrategy {
    public:
        static CompositeRLPParsing* GetInstance() {
            if (m_sInstancePtr == nullptr)
                m_sInstancePtr = new CompositeRLPParsing();
            return m_sInstancePtr;
        }

        CompositeRLPParsing(const CompositeRLPParsing& obj) = delete;

        virtual void parse(ByteSet<8> &b, shared_ptr<const FieldList> &f) const override;
        //virtual ByteSet<8> serialize() const { return ByteSet<8>(); /*FIXME*/ }
    private:
        CompositeRLPParsing() {}
    private:
        static CompositeRLPParsing *m_sInstancePtr;
};