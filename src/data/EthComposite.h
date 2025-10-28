#pragma once
#include <data/ByteSetComposite.h>

class Block : public ByteSetComposite {
    public:
        Block() : ByteSetComposite(nullptr) {}
        virtual ~Block() = default;
};