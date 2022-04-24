//
//  TxBuilder.hpp
//  WalletSample
//
//  Created by leejw51 on 2022/01/27.
//  Copyright © 2022 Epic Games, Inc. All rights reserved.
//

#ifndef TxBuilder_hpp
#define TxBuilder_hpp
#include "walletcore/include/defi-wallet-core-cpp/src/lib.rs.h"
#include <stdio.h>


class TxBuilder {
public:
    virtual void setData()=0;
    virtual  org::defi_wallet_core::CosmosSDKTxInfoRaw getTxInfo()=0;
};

// top manager
class TxDirector {
    TxBuilder* builder;
public:
    void setBuilder(TxBuilder* newbuilder);
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
