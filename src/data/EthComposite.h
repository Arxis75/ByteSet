#pragma once
#include <data/ByteSetComposite.h>

class BlockHeader : public ByteSetComposite {
    public:
        BlockHeader() = default;
        virtual ~BlockHeader() = default;

        void buildStructure() {
            auto nest = getItem(0);
            if(nest && nest->getComposite()) {
                auto raw_nest = const_cast<ByteSetComposite*>(dynamic_cast<const ByteSetComposite*>(nest));
                uint64_t nb_fields = raw_nest->getChildrenCount();
                m_field = unique_arr<unique_ptr<const ByteSetField>>(nb_fields);
                for(int i =0;i<nb_fields;i++) {
                    if(auto raw_item = raw_nest->takeItem(i).release(); raw_item) {
                        if(raw_item->getComposite()) {
                            //TODO
                        }
                        else {
                            //NOT make_unique(): It would leak raw_item!
                            m_field[i] = unique_ptr<const ByteSetField>(dynamic_cast<const ByteSetField*>(raw_item));
                        }
                    }
                }
                deleteChildren();
            }
        }

        const ByteSet<8> RLPserialize() const override
        {
            ByteSet<8> rlp;
            for(uint64_t i=0; i<m_field.size(); i++) {
                if(m_field[i])
                    rlp.push_back(m_field[i]->RLPserialize());
            }               
            return rlp =  rlp.RLPserialize(true);
        }

        inline const ByteSetField* getField(uint64_t index = 0) const { return m_field[index].get(); }

    private:
        unique_arr<unique_ptr<const ByteSetField>> m_field;
};

class BlockTransaction : public ByteSetComposite {
    public:
        BlockTransaction() = default;
        virtual ~BlockTransaction() = default;
    private:
        unique_arr<unique_ptr<const ByteSetField>> m_field;
};

class BlockTransactions : public ByteSetComposite {
    public:
        BlockTransactions() = default;
        virtual ~BlockTransactions() = default;
    private:
        unique_arr<unique_ptr<const BlockTransaction>> m_transaction;
};

class BlockUncles : public ByteSetComposite {
    public:
        BlockUncles() = default;
        virtual ~BlockUncles() = default;
    private:
        unique_arr<unique_ptr<const BlockHeader>> m_uncle;
};

class BlockWithdrawal : public ByteSetComposite {
    public:
        BlockWithdrawal() = default;
        virtual ~BlockWithdrawal() = default;
    private:
        unique_arr<unique_ptr<const ByteSetField>> m_field;
};

class BlockWithdrawals : public ByteSetComposite {
    public:
        BlockWithdrawals() = default;
        virtual ~BlockWithdrawals() = default;
    private:
        unique_arr<unique_ptr<const BlockWithdrawal>> m_withdrawal;
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

            m_header.reset(takeChildrenAs<BlockHeader>(0).release());
            m_transactions.reset(takeChildrenAs<BlockTransactions>(1).release());
            m_uncles.reset(takeChildrenAs<BlockUncles>(2).release());
            m_withdrawals.reset(takeChildrenAs<BlockWithdrawals>(3).release());

            deleteChildren();

            m_header->buildStructure();
            /*m_transactions->buildStructure();
            m_uncles->buildStructure();
            m_withdrawals->buildStructure();*/

            m_header->DumpChildren();
            /*m_transactions->DumpChildren();
            m_uncles->DumpChildren();
            m_withdrawals->DumpChildren();*/
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