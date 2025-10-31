#pragma once
#include <data/ByteSetComposite.h>

/*class VersionedByteSetComposite : public ByteSetComposite {
        template<typename T> struct Item ;
    public:
        VersionedByteSetComposite() : ByteSetComposite(), m_block_height(0) {}
        VersionedByteSetComposite(shared_ptr<const ByteSetComposite> p) : ByteSetComposite(p), m_block_height(0) {}
        virtual ~VersionedByteSetComposite() = default;

        template<typename T> shared_ptr<T> make(uint64_t index = 0) const;


};*/

class BlockHeader : public ByteSetComposite {
    public:
        BlockHeader() = default;
        virtual ~BlockHeader() = default;

        void buildStructure() {}
};

class BlockTransactions : public ByteSetComposite {
    public:
        BlockTransactions() = default;
        virtual ~BlockTransactions() = default;
};

class BlockUncles : public ByteSetComposite {
    public:
        BlockUncles() = default;
        virtual ~BlockUncles() = default;
};

class BlockWithdrawal : public ByteSetComposite {
    public:
        BlockWithdrawal() = default;
        virtual ~BlockWithdrawal() = default;

        const ByteSetField* get(uint index) const { return dynamic_cast<const ByteSetField*>(getItem(index)); }
};

class BlockWithdrawals : public ByteSetComposite {
    public:
        BlockWithdrawals() = default;
        virtual ~BlockWithdrawals() = default;

        void buildStructure() {
            for(int i =0;i<getChildrenCount();i++)
                m_withdrawals.emplace_back(takeChildrenAs<BlockWithdrawal>(i).release());
        }

    private:
        vector<unique_ptr<const BlockWithdrawal>> m_withdrawals;
};

class Block : public ByteSetComposite {
    public:
        Block() : ByteSetComposite(), m_header(nullptr) {}
        virtual ~Block() = default;
      
        inline uint64_t getHeight() const { return m_block_height; }
        inline void setHeight(uint64_t height) { m_block_height = height; }
        
        void buildStructure() {
            m_header.reset(takeChildrenAs<BlockHeader>(0).release());
            const_cast<BlockHeader*>(m_header.get())->buildStructure();
            /*m_transactions.reset(takeChildrenAs<BlockTransactions>(1).release());
            m_transactions->buildStructure();
            m_uncles.reset(takeChildrenAs<BlockUncles>(2).release());
            m_uncles->buildStructure();*/
            m_withdrawals.reset(takeChildrenAs<BlockWithdrawals>(3).release());
            const_cast<BlockWithdrawals*>(m_withdrawals.get())->buildStructure();
        }

    private:
        uint64_t m_block_height;
        unique_ptr<const BlockHeader> m_header;
        unique_ptr<const BlockTransactions> m_transactions;
        unique_ptr<const BlockUncles> m_uncles;
        unique_ptr<const BlockWithdrawals> m_withdrawals;
};

class BlockChain {
    public:
        BlockChain() = default;

        const Block* buildBlockFromRawRLP(ByteSet<8> &b, uint64_t block_height) {
            auto block = new Block();
            block->RLPparse(b);
            block->buildStructure();
            m_blocks.insert({block->getHeight(), block});
            return block;
        }
    
    private:
        std::map<uint, const Block*> m_blocks;
};

/*using Withdrawals = const BlockWithdrawals;
using Withdrawal = const BlockWithdrawal;
using Item = const ByteSetField;*/

#include <data/EthComposite.tpp>