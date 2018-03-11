// Copyright (c) 2012-2018, The CryptoNote developers, YxomTech
//
// This file is part of Varcoin.
//
// Varcoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Varcoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Varcoin.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <vector>
#include <unordered_map>
#include <limits>

#include "INode.h"
#include "VarNoteCore/VarNoteBasic.h"
#include "TestBlockchainGenerator.h"
#include "Common/ObserverManager.h"
#include "Wallet/WalletAsyncContextCounter.h"


class INodeDummyStub : public VarNote::INode
{
public:
  virtual bool addObserver(VarNote::INodeObserver* observer) override;
  virtual bool removeObserver(VarNote::INodeObserver* observer) override;

  virtual void init(const VarNote::INode::Callback& callback) {callback(std::error_code());};
  virtual bool shutdown() { return true; };

  virtual size_t getPeerCount() const { return 0; };
  virtual uint32_t getLastLocalBlockHeight() const override { return 0; };
  virtual uint32_t getLastKnownBlockHeight() const override { return 0; };
  virtual uint32_t getLocalBlockCount() const override { return 0; };
  virtual uint32_t getKnownBlockCount() const override { return 0; };
  virtual uint64_t getLastLocalBlockTimestamp() const override { return 0; }

  virtual void getNewBlocks(std::vector<Crypto::Hash>&& knownBlockIds, std::vector<VarNote::block_complete_entry>& newBlocks, uint32_t& height, const Callback& callback) override { callback(std::error_code()); };

  virtual void relayTransaction(const VarNote::Transaction& transaction, const Callback& callback) override { callback(std::error_code()); };
  virtual void getRandomOutsByAmounts(std::vector<uint64_t>&& amounts, uint64_t outsCount, std::vector<VarNote::COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS::outs_for_amount>& result, const Callback& callback) override { callback(std::error_code()); };
  virtual void getTransactionOutsGlobalIndices(const Crypto::Hash& transactionHash, std::vector<uint32_t>& outsGlobalIndices, const Callback& callback) override { callback(std::error_code()); };
  virtual void getPoolSymmetricDifference(std::vector<Crypto::Hash>&& known_pool_tx_ids, Crypto::Hash known_block_id, bool& is_bc_actual,
          std::vector<std::unique_ptr<VarNote::ITransactionReader>>& new_txs, std::vector<Crypto::Hash>& deleted_tx_ids, const Callback& callback) override {
    is_bc_actual = true; callback(std::error_code());
  };
  virtual void queryBlocks(std::vector<Crypto::Hash>&& knownBlockIds, uint64_t timestamp, std::vector<VarNote::BlockShortEntry>& newBlocks,
          uint32_t& startHeight, const Callback& callback) override { callback(std::error_code()); };

  virtual void getBlocks(const std::vector<uint32_t>& blockHeights, std::vector<std::vector<VarNote::BlockDetails>>& blocks, const Callback& callback) { callback(std::error_code()); };
  virtual void getBlocks(const std::vector<Crypto::Hash>& blockHashes, std::vector<VarNote::BlockDetails>& blocks, const Callback& callback) { callback(std::error_code()); };
  virtual void getBlocks(uint64_t timestampBegin, uint64_t timestampEnd, uint32_t blocksNumberLimit, std::vector<VarNote::BlockDetails>& blocks, uint32_t& blocksNumberWithinTimestamps, const Callback& callback) { callback(std::error_code()); };
  virtual void getTransactions(const std::vector<Crypto::Hash>& transactionHashes, std::vector<VarNote::TransactionDetails>& transactions, const Callback& callback) { callback(std::error_code()); };
  virtual void getTransactionsByPaymentId(const Crypto::Hash& paymentId, std::vector<VarNote::TransactionDetails>& transactions, const Callback& callback) { callback(std::error_code()); };
  virtual void getPoolTransactions(uint64_t timestampBegin, uint64_t timestampEnd, uint32_t transactionsNumberLimit, std::vector<VarNote::TransactionDetails>& transactions, uint64_t& transactionsNumberWithinTimestamps, const Callback& callback) { callback(std::error_code()); };
  virtual void isSynchronized(bool& syncStatus, const Callback& callback) { callback(std::error_code()); };
  virtual void getMultisignatureOutputByGlobalIndex(uint64_t amount, uint32_t gindex, VarNote::MultisignatureOutput& out, const Callback& callback) override { callback(std::error_code()); }

  void updateObservers();

  Tools::ObserverManager<VarNote::INodeObserver> observerManager;

};

class INodeTrivialRefreshStub : public INodeDummyStub {
public:
  INodeTrivialRefreshStub(TestBlockchainGenerator& generator, bool consumerTests = false) :
    m_lastHeight(1), m_blockchainGenerator(generator), m_nextTxError(false), m_getMaxBlocks(std::numeric_limits<size_t>::max()), m_nextTxToPool(false), m_synchronized(false), consumerTests(consumerTests) {
  };

  void setGetNewBlocksLimit(size_t maxBlocks) { m_getMaxBlocks = maxBlocks; }

  virtual uint32_t getLastLocalBlockHeight() const { return static_cast<uint32_t>(m_blockchainGenerator.getBlockchain().size() - 1); }
  virtual uint32_t getLastKnownBlockHeight() const { return static_cast<uint32_t>(m_blockchainGenerator.getBlockchain().size() - 1); }

  virtual uint32_t getLocalBlockCount() const override { return static_cast<uint32_t>(m_blockchainGenerator.getBlockchain().size()); }
  virtual uint32_t getKnownBlockCount() const override { return static_cast<uint32_t>(m_blockchainGenerator.getBlockchain().size()); }

  virtual void getNewBlocks(std::vector<Crypto::Hash>&& knownBlockIds, std::vector<VarNote::block_complete_entry>& newBlocks, uint32_t& startHeight, const Callback& callback);

  virtual void relayTransaction(const VarNote::Transaction& transaction, const Callback& callback);
  virtual void getRandomOutsByAmounts(std::vector<uint64_t>&& amounts, uint64_t outsCount, std::vector<VarNote::COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS::outs_for_amount>& result, const Callback& callback);
  virtual void getTransactionOutsGlobalIndices(const Crypto::Hash& transactionHash, std::vector<uint32_t>& outsGlobalIndices, const Callback& callback) override;
  virtual void queryBlocks(std::vector<Crypto::Hash>&& knownBlockIds, uint64_t timestamp, std::vector<VarNote::BlockShortEntry>& newBlocks, uint32_t& startHeight, const Callback& callback) override;
  virtual void getPoolSymmetricDifference(std::vector<Crypto::Hash>&& known_pool_tx_ids, Crypto::Hash known_block_id, bool& is_bc_actual,
          std::vector<std::unique_ptr<VarNote::ITransactionReader>>& new_txs, std::vector<Crypto::Hash>& deleted_tx_ids, const Callback& callback) override;

  virtual void getBlocks(const std::vector<uint32_t>& blockHeights, std::vector<std::vector<VarNote::BlockDetails>>& blocks, const Callback& callback) override;
  virtual void getBlocks(const std::vector<Crypto::Hash>& blockHashes, std::vector<VarNote::BlockDetails>& blocks, const Callback& callback) override;
  virtual void getBlocks(uint64_t timestampBegin, uint64_t timestampEnd, uint32_t blocksNumberLimit, std::vector<VarNote::BlockDetails>& blocks, uint32_t& blocksNumberWithinTimestamps, const Callback& callback) override;
  virtual void getTransactions(const std::vector<Crypto::Hash>& transactionHashes, std::vector<VarNote::TransactionDetails>& transactions, const Callback& callback) override;
  virtual void getTransactionsByPaymentId(const Crypto::Hash& paymentId, std::vector<VarNote::TransactionDetails>& transactions, const Callback& callback)  override;
  virtual void getPoolTransactions(uint64_t timestampBegin, uint64_t timestampEnd, uint32_t transactionsNumberLimit, std::vector<VarNote::TransactionDetails>& transactions, uint64_t& transactionsNumberWithinTimestamps, const Callback& callback)  override;
  virtual void isSynchronized(bool& syncStatus, const Callback& callback) override;
  virtual void getMultisignatureOutputByGlobalIndex(uint64_t amount, uint32_t gindex, VarNote::MultisignatureOutput& out, const Callback& callback) override;


  virtual void startAlternativeChain(uint32_t height);
  void setNextTransactionError();
  void setNextTransactionToPool();
  void cleanTransactionPool();
  void setMaxMixinCount(uint64_t maxMixin);
  void includeTransactionsFromPoolToBlock();

  void setSynchronizedStatus(bool status);

  void sendPoolChanged();
  void sendLocalBlockchainUpdated();

  std::vector<Crypto::Hash> calls_getTransactionOutsGlobalIndices;

  virtual ~INodeTrivialRefreshStub();

  std::function<void(const Crypto::Hash&, std::vector<uint32_t>&)> getGlobalOutsFunctor = [](const Crypto::Hash&, std::vector<uint32_t>&) {};

  void waitForAsyncContexts();

protected:
  void doGetNewBlocks(std::vector<Crypto::Hash> knownBlockIds, std::vector<VarNote::block_complete_entry>& newBlocks,
          uint32_t& startHeight, std::vector<VarNote::Block> blockchain, const Callback& callback);
  void doGetTransactionOutsGlobalIndices(const Crypto::Hash& transactionHash, std::vector<uint32_t>& outsGlobalIndices, const Callback& callback);
  void doRelayTransaction(const VarNote::Transaction& transaction, const Callback& callback);
  void doGetRandomOutsByAmounts(std::vector<uint64_t> amounts, uint64_t outsCount, std::vector<VarNote::COMMAND_RPC_GET_RANDOM_OUTPUTS_FOR_AMOUNTS::outs_for_amount>& result, const Callback& callback);
  void doGetPoolSymmetricDifference(std::vector<Crypto::Hash>&& known_pool_tx_ids, Crypto::Hash known_block_id, bool& is_bc_actual,
          std::vector<std::unique_ptr<VarNote::ITransactionReader>>& new_txs, std::vector<Crypto::Hash>& deleted_tx_ids, const Callback& callback);

  void doGetBlocks(const std::vector<uint32_t>& blockHeights, std::vector<std::vector<VarNote::BlockDetails>>& blocks, const Callback& callback);
  void doGetBlocks(const std::vector<Crypto::Hash>& blockHashes, std::vector<VarNote::BlockDetails>& blocks, const Callback& callback);
  void doGetBlocks(uint64_t timestampBegin, uint64_t timestampEnd, uint32_t blocksNumberLimit, std::vector<VarNote::BlockDetails>& blocks, uint32_t& blocksNumberWithinTimestamps, const Callback& callback);
  void doGetTransactions(const std::vector<Crypto::Hash>& transactionHashes, std::vector<VarNote::TransactionDetails>& transactions, const Callback& callback);
  void doGetPoolTransactions(uint64_t timestampBegin, uint64_t timestampEnd, uint32_t transactionsNumberLimit, std::vector<VarNote::TransactionDetails>& transactions, uint64_t& transactionsNumberWithinTimestamps, const Callback& callback);
  void doGetTransactionsByPaymentId(const Crypto::Hash& paymentId, std::vector<VarNote::TransactionDetails>& transactions, const Callback& callback);
  void doGetOutByMSigGIndex(uint64_t amount, uint32_t gindex, VarNote::MultisignatureOutput& out, const Callback& callback);


  size_t m_getMaxBlocks;
  uint32_t m_lastHeight;
  TestBlockchainGenerator& m_blockchainGenerator;
  bool m_nextTxError;
  bool m_nextTxToPool;
  std::mutex m_walletLock;
  VarNote::WalletAsyncContextCounter m_asyncCounter;
  uint64_t m_maxMixin = std::numeric_limits<uint64_t>::max();
  bool m_synchronized;
  bool consumerTests;
};

