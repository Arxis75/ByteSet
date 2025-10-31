#include <data/EthComposite.h>

/*struct Field {
    const char* ID = "";
    const RLPType rlp_type = RLPType::LIST;
    const uint64_t from_block = 0;
    const uint64_t to_block = 0xFFFFFFFFFFFFFFFF;
};

template<typename T> struct VersionedByteSetComposite::Item {
    using base_type = const ByteSetComposite;
    static constexpr const char* ID = "";
    static constexpr const RLPType rlp_type = RLPType::LIST;
    static constexpr const uint64_t rank = 0;
    static constexpr const uint64_t from_block = 0;
    static constexpr const uint64_t to_block = 0xFFFFFFFFFFFFFFFF;
    static constexpr const struct Field fields[] = {};
};

template<> struct VersionedByteSetComposite::Item<const Block> {
    using base_type = const ByteSetComposite;
    static constexpr const char* ID = "Block";
    static constexpr const RLPType rlp_type = RLPType::LIST;
    static constexpr const uint64_t rank = 0;
    static constexpr const uint64_t from_block = 0;
    static constexpr const uint64_t to_block = 0xFFFFFFFFFFFFFFFF;
    static constexpr const struct Field fields[] = {};
};

template<> struct VersionedByteSetComposite::Item<const BlockHeader> {
    using base_type = const ByteSetComposite;
    static constexpr const char* ID = "Header";
    static constexpr const RLPType rlp_type = RLPType::LIST;
    static constexpr const uint64_t rank = 0;
    static constexpr const uint64_t from_block = 0;
    static constexpr const uint64_t to_block = 0xFFFFFFFFFFFFFFFF;
    static constexpr const struct Field fields[] = {};
};

template<> struct VersionedByteSetComposite::Item<const BlockWithdrawals> {
    using base_type = const ByteSetComposite;
    static constexpr const char* ID = "Withdrawals";
    static constexpr const RLPType rlp_type = RLPType::LIST;
    static constexpr const uint64_t rank = 3;
    static constexpr const uint64_t from_block = 0;
    static constexpr const uint64_t to_block = 0xFFFFFFFFFFFFFFFF;
    static constexpr const struct Field fields[] = {};
};

template<> struct VersionedByteSetComposite::Item<const BlockWithdrawal> {
    using base_type = const ByteSetComposite;
    static constexpr const char* ID = "Withdrawal";
    static constexpr const RLPType rlp_type = RLPType::LIST;
    static constexpr const uint64_t rank = 0;
    static constexpr const uint64_t from_block = 0;
    static constexpr const uint64_t to_block = 0xFFFFFFFFFFFFFFFF;
    static constexpr const struct Field fields[] = {
        {"Adress", RLPType::BYTE, 0, 0xFFFFFFFFFFFFFFFF},
        {"Amount", RLPType::BYTE, 0, 0xFFFFFFFFFFFFFFFF},
        {"Index", RLPType::BYTE, 0, 0xFFFFFFFFFFFFFFFF},
        {"ValidatorIndex", RLPType::BYTE, 0, 0xFFFFFFFFFFFFFFFF}
    };
};

template<> struct VersionedByteSetComposite::Item<const ByteSetField> {
    using base_type = const ByteSetField;
    static constexpr const char* ID = "";
    static constexpr const RLPType rlp_type = RLPType::BYTE;
    static constexpr const uint64_t rank = 0;
    static constexpr const uint64_t from_block = 0;
    static constexpr const uint64_t to_block = 0;
    static constexpr const struct Field fields[] = {};
};

template<typename T>
shared_ptr<T> VersionedByteSetComposite::make(uint64_t index) const {
    shared_ptr<const IByteSetContainer> item = nullptr, result = nullptr;
    shared_ptr<const ByteSetComposite> self = nullptr;
    
    uint64_t rank = index ? index : Item<T>::rank;
    if (typeid(*this) == typeid(Block)) {
        if(auto self = dynamic_pointer_cast<const ByteSetComposite>(getItem(0)); self)
            item = dynamic_pointer_cast<typename Item<T>::base_type>(self->getItem(rank));
    }
    else
        item = dynamic_pointer_cast<typename Item<T>::base_type>(getItem(rank));
    if(item) {
        bool is_field = (item->getComposite() == 0);
        uint64_t from_block = is_field ? Item<T>::fields[rank].from_block : Item<T>::from_block;
        uint64_t to_block = is_field ? Item<T>::fields[rank].to_block : Item<T>::to_block;
        if(getHeight() >= from_block && getHeight() < to_block) {
            result = make_shared<T>(dynamic_pointer_cast<typename Item<T>::base_type>(item));
        //}
    }
    return dynamic_pointer_cast<T>(result);
}*/