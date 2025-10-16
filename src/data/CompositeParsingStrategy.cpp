#include <data/CompositeParsingStrategy.h>
#include <data/tryout.h>
#include <data/ByteSet.h>

CompositeRLPParsing* CompositeRLPParsing::m_sInstancePtr = nullptr;

void CompositeRLPParsing::parse(ByteSet<8> &b, FieldList* &f) const
{
    if(b.byteSize()) {
        ByteSet<8> val;
        uint64_t size = 0, size_size = 0;
        uint8_t header = b[0];

        //Single element
        if(header < 0x80) {
            val = header;
            f = new Field(val);
        }
        else if(header < 0xb7) {
            size = header - 0x80;
            val = b.at(1, size);
            f = new Field(val);
        }
        else if(header < 0xc0) {
            size_size = header - 0xb7;
            size = b.at(1, size_size).asInteger();
            val = b.at(1 + size_size, size);
            f = new Field(val);
        }
        //List
        else if(header < 0xf7) {
            size = header - 0xc0;
            val = b.at(1, size);
            f = new List(val);
        }
        else {
            size_size = header - 0xf7;
            size = b.at(1, size_size).asInteger();
            val = b.at(1 + size_size, size);
            f = new List(val);
        }
        
        b.pop_front(1 + size_size + size);
    }
}