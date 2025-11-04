#pragma once
#include <data/ByteSetComposite.h>

class EthContainer : public ByteSetComposite {
    public:
        virtual ~EthContainer() = default;

        virtual const ByteSet<8> RLPserialize() const override;

        virtual void buildStructure(uint64_t type = 0) = 0;
        template<typename T>
            void buildItem(uint64_t index, uint64_t type = 0);
        template<typename T>
            void buildAllItems(bool typed = false);
        template<typename T>
            inline const T* get(uint64_t index) const { return (index < m_items.size() ? dynamic_cast<const T*>(m_items[index].get()) : nullptr); }

        void DumpFields() const;

        inline uint64_t getType() const { return m_type; }
    
    protected:
        EthContainer() = default;
        inline void setType(uint64_t type) { m_type = type; }

    protected:
        unique_arr<unique_ptr<IByteSetContainer>> m_items;
        int64_t m_type;
};
struct BlockField : public ByteSetField {
    inline virtual void buildStructure(uint64_t) {}
};

struct BlockHeader : public EthContainer {
    virtual void buildStructure(uint64_t) override { buildAllItems<BlockField>(); }
};
struct BlockAuthorization : public EthContainer {
    virtual void buildStructure(uint64_t) override { buildAllItems<BlockField>(); }
};
struct BlockAuthorizationList : public EthContainer {
    virtual void buildStructure(uint64_t) override { buildAllItems<BlockAuthorization>(); }
};
struct BlockBlobVersionHashes : public EthContainer {
    virtual void buildStructure(uint64_t) override { buildAllItems<BlockField>(); }
};
struct BlockStorageKeys : public EthContainer {
    virtual void buildStructure(uint64_t) override { buildAllItems<BlockField>(); }
};
struct BlockAccessList : public EthContainer {
    virtual void buildStructure(uint64_t) override {
        buildItem<BlockField>(0);
        buildItem<BlockStorageKeys>(1);
        deleteChildren();
    }
};
struct BlockAccessLists : public EthContainer {
    virtual void buildStructure(uint64_t) override { buildAllItems<BlockAccessList>(); }
};
struct BlockTransaction : public EthContainer {
    virtual void buildStructure(uint64_t) override;
};
struct BlockTransactions : public EthContainer { 
     virtual void buildStructure(uint64_t) override { buildAllItems<BlockTransaction>(true); }
};
struct BlockUncles : public EthContainer {
    virtual void buildStructure(uint64_t) override { buildAllItems<BlockHeader>(); }
};
struct BlockWithdrawal : public EthContainer {
    virtual void buildStructure(uint64_t) override { buildAllItems<BlockField>(); }
};
struct BlockWithdrawals : public EthContainer {
    virtual void buildStructure(uint64_t) override { buildAllItems<BlockWithdrawal>(); }
};

class Block : public EthContainer {
    public:
        Block() : EthContainer(), m_block_height(-1) {}
        virtual ~Block() = default;
      
        virtual void buildStructure(uint64_t type = 0) override;

        const BlockHeader* getHeader() const { return get<const BlockHeader>(0); }
        const BlockTransactions* getTransactions() const { return get<const BlockTransactions>(1); }
        const BlockUncles* getUncles() const { return get<const BlockUncles>(2); }
        const BlockWithdrawals* getWithdrawals() const { return get<const BlockWithdrawals>(3); }

        inline uint64_t getHeight() const { return m_block_height; }

    protected:
        inline void setHeight(uint64_t height) { m_block_height = height; }

    private:
        int64_t m_block_height;
};

class BlockChain {
    public:
        BlockChain() = default;

        const Block* newBlockFromRawRLP(ByteSet<8> &b);
        inline const Block* getBlock(uint64_t block_height) const { return m_blocks.find(block_height)->second.get(); };

    private:
        std::map<uint64_t, unique_ptr<const Block>> m_blocks;
};

using Header = const BlockHeader;
using Transactions = const BlockTransactions;
using Transaction = const BlockTransaction;
using Uncles = const BlockUncles;
using Withdrawals = const BlockWithdrawals;
using Withdrawal = const BlockWithdrawal;
using Field = const BlockField;

#include <data/EthComposite.tpp>
