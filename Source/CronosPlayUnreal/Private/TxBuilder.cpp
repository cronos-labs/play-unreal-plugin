// Copyright 2022, Cronos Labs. All Rights Reserved

#include "TxBuilder.h"

#include <iostream>
#include <sstream>

#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/lib.rs.h"

using namespace std;
using namespace org::defi_wallet_core;

void TxDirector::setBuilder(TxBuilder *newbuilder) { builder = newbuilder; }

CosmosSDKTxInfoRaw TxDirector::makeTx() {
    assert(builder != NULL);
    builder->setData();
    CosmosSDKTxInfoRaw txinfo = builder->getTxInfo();
    return txinfo;
}

void CosmosSendAmountTxBuilder::setData() {
    info.account_number = 0;
    info.sequence_number = 0;
    info.gas_limit = 100000;
    info.fee_amount = 1000000;
    info.fee_denom = "basecro";
    info.timeout_height = 0;
    info.memo_note = "";
    info.chain_id = "";
    info.coin_type = 394;
    info.bech32hrp = "cro";
}

CosmosSDKTxInfoRaw CosmosSendAmountTxBuilder::getTxInfo() { return info; }
