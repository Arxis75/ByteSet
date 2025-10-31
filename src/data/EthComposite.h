#pragma once
#include <data/ByteSetComposite.h>

class EthContainer : public ByteSetComposite {
    public:
        EthContainer() = default;
        virtual ~EthContainer() = default;

        virtual const ByteSet<8> RLPserialize() const override;
        
        void buildStructure();
        inline const ByteSetField* getField(uint64_t index = 0) const { return dynamic_cast<const ByteSetField*>(m_field[index].get()); }
    
        void DumpFields() const;

    protected:
        unique_arr<unique_ptr<const IByteSetContainer>> m_field;
};

struct BlockHeader : public EthContainer {};
struct BlockTransaction : public EthContainer {};
struct BlockTransactions : public EthContainer {
    inline const BlockTransaction* getTransaction(uint64_t index = 0) const { return dynamic_cast<const BlockTransaction*>(m_field[index].get()); }
};
struct BlockUncles : public EthContainer {
    inline const BlockHeader* getHeader(uint64_t index = 0) const { return dynamic_cast<const BlockHeader*>(m_field[index].get()); }
};
struct BlockWithdrawal : public EthContainer {};
struct BlockWithdrawals : public EthContainer {
    inline const BlockWithdrawal* getWithdrawal(uint64_t index = 0) const { return dynamic_cast<const BlockWithdrawal*>(m_field[index].get()); }
};

class Block : public ByteSetComposite {
    public:
        Block() : ByteSetComposite(), m_header(nullptr) {}
        virtual ~Block() = default;
      
        inline uint64_t getHeight() const { return m_block_height; }
        inline void setHeight(uint64_t height) { m_block_height = height; }
        
        void buildFromRLP(ByteSet<8> &b, uint64_t block_height) {
            setHeight(block_height);
            RLPparse(b);

            m_header.reset(newChildrenAs<BlockHeader>(0));
            m_transactions.reset(newChildrenAs<BlockTransactions>(1));
            m_uncles.reset(newChildrenAs<BlockUncles>(2));
            m_withdrawals.reset(newChildrenAs<BlockWithdrawals>(3));

            deleteChildren();

            m_header->buildStructure();
            m_transactions->buildStructure();
            m_uncles->buildStructure();
            m_withdrawals->buildStructure();
        }

        virtual const ByteSet<8> RLPserialize() const override {
            ByteSet<8> rlp;
            rlp.push_back(m_header->RLPserialize());
            rlp.push_back(m_transactions->RLPserialize());
            rlp.push_back(m_uncles->RLPserialize());
            rlp.push_back(m_withdrawals->RLPserialize());
            return rlp.RLPserialize(true);
        }

        inline const BlockHeader* getHeader() const { return m_header.get(); }
        inline const BlockTransactions* getTransactions() const { return m_transactions.get(); }
        inline const BlockUncles* getUncles() const { return m_uncles.get(); }
        inline const BlockWithdrawals* getWithdrawals() const { return m_withdrawals.get(); }

    private:
        uint64_t m_block_height;
        unique_ptr<BlockHeader> m_header;
        unique_ptr<BlockTransactions> m_transactions;
        unique_ptr<BlockUncles> m_uncles;
        unique_ptr<BlockWithdrawals> m_withdrawals;
};

class BlockChain {
    public:
        BlockChain() = default;

        const Block* addBlockFromRawRLP(ByteSet<8> &b, uint64_t block_height) {
            auto block = make_unique<Block>();
            block->buildFromRLP(b, block_height);
            m_blocks.insert({block_height, std::move(block)});
            return getBlock(block_height);
        } 
    
        inline const Block* getBlock(uint64_t block_height) const { return m_blocks.find(block_height)->second.get(); };

    private:
        std::map<uint64_t, unique_ptr<const Block>> m_blocks;
};

#include <data/EthComposite.tpp>