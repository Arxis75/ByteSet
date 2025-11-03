#pragma once
#include <data/ByteSetComposite.h>

class BlockWithdrawal;
using Withdrawal = const BlockWithdrawal;

class EthContainer : public ByteSetComposite {
    public:
        EthContainer() = default;
        virtual ~EthContainer() = default;

        virtual const ByteSet<8> RLPserialize() const override;

        virtual void buildStructure() = 0;
        template<typename T>
            void buildItem(uint64_t index);
        template<typename T>
            void buildAllItems();
        template<typename T>
            inline const T* get(uint64_t index) const { return (index < m_items.size() ? dynamic_cast<const T*>(m_items[index].get()) : nullptr); }

        void DumpFields() const;

    protected:
        unique_arr<unique_ptr<IByteSetContainer>> m_items;
};
struct BlockField : public ByteSetField {
    inline virtual void buildStructure() {}
};

struct BlockHeader : public EthContainer {
    virtual void buildStructure() override { buildAllItems<BlockField>(); }
};
struct BlockTransaction : public EthContainer {
    virtual void buildStructure() override { buildAllItems<BlockField>(); /*FIXME*/ }
};
struct BlockTransactions : public EthContainer { 
     virtual void buildStructure() override { buildAllItems<BlockField>(); /*FIXME*/ }
};
struct BlockUncles : public EthContainer {
    virtual void buildStructure() override { buildAllItems<BlockHeader>(); }
};
struct BlockWithdrawal : public EthContainer {
    virtual void buildStructure() override { buildAllItems<BlockField>(); }
};
struct BlockWithdrawals : public EthContainer {
    virtual void buildStructure() override { buildAllItems<BlockWithdrawal>(); }
};

class Block : public EthContainer {
    public:
        Block() : EthContainer(), m_block_height(-1) {}
        virtual ~Block() = default;
      
        virtual void buildStructure() override {
            buildItem<BlockHeader>(0);
            buildItem<BlockTransactions>(1);
            buildItem<BlockUncles>(2);
            buildItem<BlockWithdrawals>(3);
            
            deleteChildren();

            setHeight(getHeader()->get<const BlockField>(8)->getIntValue());
        }

        const BlockHeader* getHeader() const { return get<const BlockHeader>(0); }
        const BlockTransactions* getTransactions() const { return get<const BlockTransactions>(1); }
        const BlockUncles* getUncles() const { return get<const BlockUncles>(2); }
        const BlockWithdrawals* getWithdrawals() const { return get<const BlockWithdrawals>(3); }

        inline void setHeight(uint64_t height) { m_block_height = height; }
        inline uint64_t getHeight() const { return m_block_height; }

    private:
        int64_t m_block_height;
};

class BlockChain {
    public:
        BlockChain() = default;

        const Block* newBlockFromRawRLP(ByteSet<8> &b) {
            auto block = make_unique<Block>();
            block->RLPparse(b);
            block->buildStructure();
            auto result= block.get();
            m_blocks.insert({block->getHeight(), std::move(block)});
            return result;
        } 
    
        inline const Block* getBlock(uint64_t block_height) const { return m_blocks.find(block_height)->second.get(); };

    private:
        std::map<uint64_t, unique_ptr<const Block>> m_blocks;
};

using Header = const BlockHeader;
using Transactions = const BlockTransactions;
using Transactions = const BlockTransactions;
using Uncles = const BlockUncles;
using Withdrawals = const BlockWithdrawals;
using Withdrawal = const BlockWithdrawal;
using Field = const BlockField;

#include <data/EthComposite.tpp>
