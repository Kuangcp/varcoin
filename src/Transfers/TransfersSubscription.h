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

#include "ITransfersSynchronizer.h"
#include "TransfersContainer.h"
#include "IObservableImpl.h"

namespace VarNote {

class TransfersSubscription : public IObservableImpl < ITransfersObserver, ITransfersSubscription > {
public:
  TransfersSubscription(const VarNote::Currency& currency, const AccountSubscription& sub);

  SynchronizationStart getSyncStart();
  void onBlockchainDetach(uint32_t height);
  void onError(const std::error_code& ec, uint32_t height);
  bool advanceHeight(uint32_t height);
  const AccountKeys& getKeys() const;
  void addTransaction(const TransactionBlockInfo& blockInfo, const ITransactionReader& tx,
                      const std::vector<TransactionOutputInformationIn>& transfers);

  void deleteUnconfirmedTransaction(const Crypto::Hash& transactionHash);
  void markTransactionConfirmed(const TransactionBlockInfo& block, const Crypto::Hash& transactionHash, const std::vector<uint32_t>& globalIndices);

  // ITransfersSubscription
  virtual AccountPublicAddress getAddress() override;
  virtual ITransfersContainer& getContainer() override;

private:
  TransfersContainer transfers;
  AccountSubscription subscription;
};

}
