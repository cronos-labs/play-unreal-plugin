// Fill out your copyright notice in the Description page of Project Settings.

#include "DefiWalletCoreFunctions.h"
#include "lib.rs.h"
#include <iostream>
#include <sstream>
using namespace std;
using namespace org::defi_wallet_core;

CosmosSDKTxInfoRaw build_CosmosTxInfo() {
  CosmosSDKTxInfoRaw ret;
  ret.account_number = 0;
  ret.sequence_number = 0;
  ret.gas_limit = 100000;
  ret.fee_amount = 1000000;
  ret.fee_denom = "basecro";
  ret.timeout_height = 0;
  ret.memo_note = "";
  ret.chain_id = "";
  ret.coin_type = 394;
  ret.bech32hrp = "cro";
  return ret;
}

FString UDefiWalletCoreFunctions::GetDefaultAddress(FString mnemonics) {
  try {
    rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> tmpWallet =
        restore_wallet(*mnemonics, "");
    rust::cxxbridge1::String result =
        tmpWallet->get_default_address(CoinType::CryptoOrgMainnet);
    FString defaultAddress = UTF8_TO_TCHAR(result.c_str());

    UE_LOG(LogTemp, Log, TEXT("DefiWalletCore DefaultAddress %s"),
           *defaultAddress);

    return defaultAddress;
  } catch (const std::exception &e) {
    return FString::Printf(TEXT("DefiWalletCore GetDefaultAddress Error: %s"),
                           UTF8_TO_TCHAR(e.what()));
  }
}

FString UDefiWalletCoreFunctions::SendAmount(FString mnemonics, FString chainid,
                                             FString from, FString to,
                                             int64 amount, FString amountdenom,
                                             FString cosmosrpc,
                                             FString tendermintrpc) {

  try {

    CosmosSDKTxInfoRaw tx_info = build_CosmosTxInfo();
    rust::cxxbridge1::String mymnemonics = TCHAR_TO_UTF8(*mnemonics);
    string mychainid = TCHAR_TO_UTF8(*chainid);
    string myfrom = TCHAR_TO_UTF8(*from);
    string myto = TCHAR_TO_UTF8(*to);
    uint64 myamount = (uint64)amount;
    string myamountdenom = TCHAR_TO_UTF8(*amountdenom);
    string myservercosmos = TCHAR_TO_UTF8(*cosmosrpc);         /* 1317 port */
    string myservertendermint = TCHAR_TO_UTF8(*tendermintrpc); /* 26657 port */
    rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> mywallet =
        restore_wallet(mymnemonics, "");
    UE_LOG(LogTemp, Log,
           TEXT("DefiWalletCore SendAmount from %s to %s amount %lld %s"),
           UTF8_TO_TCHAR(myfrom.c_str()), UTF8_TO_TCHAR(myto.c_str()), myamount,
           UTF8_TO_TCHAR(myamountdenom.c_str()));

    rust::cxxbridge1::String success, fail;
    rust::cxxbridge1::String result =
        mywallet->get_default_address(CoinType::CryptoOrgMainnet);
    rust::cxxbridge1::String balance =
        query_account_balance(myservercosmos, myfrom, tx_info.fee_denom, 1);

    rust::cxxbridge1::String detailjson =
        query_account_details(myservercosmos, myfrom);

    org::defi_wallet_core::CosmosAccountInfoRaw detailinfo =
        query_account_details_info(myservercosmos, myfrom);
    tx_info.account_number = detailinfo.account_number;
    tx_info.sequence_number = detailinfo.sequence_number;
    tx_info.chain_id = mychainid;
    char hdpath[100];
    snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/0", tx_info.coin_type);
    rust::cxxbridge1::Box<org::defi_wallet_core::PrivateKey> privatekey =
        mywallet->get_key(hdpath);

    rust::cxxbridge1::Vec<uint8_t> signedtx = get_single_bank_send_signed_tx(
        tx_info, *privatekey, myto, myamount, myamountdenom);
    rust::String broadcastResult = broadcast_tx(myservertendermint, signedtx);
    UE_LOG(LogTemp, Log, TEXT("DefiWalletCore BroadcastTX Result %s"),
           UTF8_TO_TCHAR(broadcastResult.c_str()));

    return UTF8_TO_TCHAR(broadcastResult.c_str());
  } catch (const std::exception &e) {
    return FString::Printf(TEXT("DefiWalletCore SendAmount Error: %s"),
                           UTF8_TO_TCHAR(e.what()));
  }
}
