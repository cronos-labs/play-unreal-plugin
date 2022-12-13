// Copyright 2022, Cronos Labs. All Rights Reserved

#ifndef TxBuilder_hpp
#define TxBuilder_hpp
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/lib.rs.h"
#include <stdio.h>

class TxBuilder {
  public:
    virtual void setData() = 0;
    virtual org::defi_wallet_core::CosmosSDKTxInfoRaw getTxInfo() = 0;
};

// top manager
class TxDirector {
    TxBuilder *builder;

  public:
    void setBuilder(TxBuilder *newbuilder);
    org::defi_wallet_core::CosmosSDKTxInfoRaw makeTx();
};

// various txs
class CosmosSendAmountTxBuilder : public TxBuilder {
  public:
    org::defi_wallet_core::CosmosSDKTxInfoRaw info;
    void setData();

    org::defi_wallet_core::CosmosSDKTxInfoRaw getTxInfo();
};

#endif /* TxBuilder_hpp */
