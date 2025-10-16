#include <data/tryout.h>
#include <data/CompositeParsingStrategy.h>

Composite::Composite(ByteSet<> &b) 
    : m_parser(CompositeRLPParsing::GetInstance())
{
    while(b.byteSize()) {
        shared_ptr<FieldList> fl = nullptr;
        m_parser->parse(b, fl);
        if(auto f = dynamic_pointer_cast<Field>(fl) ; f)
            push_back(f);
        else if(auto l = dynamic_pointer_cast<List>(fl) ; l)
            push_back(l);
    }
}