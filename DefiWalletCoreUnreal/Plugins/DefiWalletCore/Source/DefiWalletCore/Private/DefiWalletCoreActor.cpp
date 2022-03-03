// Fill out your copyright notice in the Description page of Project Settings.
#include "DefiWalletCoreActor.h"
#include "TxBuilder.h"
#include "cxx.h"
#include "lib.rs.h"
#include <iostream>
#include <sstream>
using namespace std;
using namespace org::defi_wallet_core;

// Sets default values
ADefiWalletCoreActor::ADefiWalletCoreActor()
    : myCosmosRpc("http://mynode:1317")
    , myTendermintRpc("http://mynode:26657")
    , myChainID("testnet-baseball-1")

{

    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADefiWalletCoreActor::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ADefiWalletCoreActor::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ADefiWalletCoreActor::Destroyed()
{
    Super::Destroyed();

    DestroyWallet();
    assert(NULL == _coreWallet);
}

void ADefiWalletCoreActor::SendAmount(FString fromaddress, FString toaddress, int64 amount, FString amountdenom, FString& output, bool& success, FString& message)
{

    try {
        if (NULL == _coreWallet) {
            success = false;
            message = TEXT("Invalid Wallet");
        }
        TxDirector director;
        CosmosSendAmountTxBuilder builder;
        director.setBuilder(&builder);
        CosmosSDKTxInfoRaw tx_info = director.makeTx();

        assert(NULL == _coreWallet);
        string mychainid = TCHAR_TO_UTF8(*myChainID);
        string myfrom = TCHAR_TO_UTF8(*fromaddress);
        string myto = TCHAR_TO_UTF8(*toaddress);
        uint64 myamount = (uint64)amount;
        string myamountdenom = TCHAR_TO_UTF8(*amountdenom);
        string myservercosmos = TCHAR_TO_UTF8(*myCosmosRpc); /* 1317 port */
        string myservertendermint = TCHAR_TO_UTF8(*myTendermintRpc); /* 26657 port */
        UE_LOG(LogTemp, Log,
            TEXT("DefiWalletCore SendAmount from %s to %s amount %lld %s"),
            UTF8_TO_TCHAR(myfrom.c_str()), UTF8_TO_TCHAR(myto.c_str()), myamount,
            UTF8_TO_TCHAR(myamountdenom.c_str()));

        rust::cxxbridge1::String result = _coreWallet->get_default_address(CoinType::CryptoOrgMainnet);
        rust::cxxbridge1::String balance = query_account_balance(myservercosmos, myfrom, tx_info.fee_denom, 1);

        rust::cxxbridge1::String detailjson = query_account_details(myservercosmos, myfrom);

        org::defi_wallet_core::CosmosAccountInfoRaw detailinfo = query_account_details_info(myservercosmos, myfrom);
        tx_info.account_number = detailinfo.account_number;
        tx_info.sequence_number = detailinfo.sequence_number;
        tx_info.chain_id = mychainid;
        char hdpath[100];
        snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/0", tx_info.coin_type);
        rust::cxxbridge1::Box<org::defi_wallet_core::PrivateKey> privatekey = _coreWallet->get_key(hdpath);

        rust::cxxbridge1::Vec<uint8_t> signedtx = get_single_bank_send_signed_tx(
            tx_info, *privatekey, myto, myamount, myamountdenom);
        rust::String broadcastResult = broadcast_tx(myservertendermint, signedtx);
        UE_LOG(LogTemp, Log, TEXT("DefiWalletCore BroadcastTX Result %s"),
            UTF8_TO_TCHAR(broadcastResult.c_str()));

        success = true;
        output = UTF8_TO_TCHAR(broadcastResult.c_str());
    } catch (const rust::cxxbridge1::Error& e) {
        success = false;
        message = FString::Printf(TEXT("DefiWalletCore SendAmount Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::DestroyWallet()
{
    if (_coreWallet != NULL) {
        // restored back
        rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> tmpwallet = rust::cxxbridge1::Box<org::defi_wallet_core::Wallet>::from_raw(
            _coreWallet);
        // drop called
        _coreWallet = NULL;
    }
}

void ADefiWalletCoreActor::CreateWallet(FString mnemonics, FString password)
{
    if (NULL == _coreWallet) {
        rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> tmpWallet = restore_wallet(*mnemonics, *password);
        // ownership transferred
        _coreWallet = tmpWallet.into_raw();
    }
}

void ADefiWalletCoreActor::InitializeWallet(FString mnemonics, FString password, FString& output, bool& success, FString& message)
{
    try {
        CreateWallet(mnemonics, password);
        assert(_coreWallet != NULL);
        rust::cxxbridge1::String result = _coreWallet->get_default_address(CoinType::CryptoOrgMainnet);
        output = UTF8_TO_TCHAR(result.c_str());
        success = true;
    } catch (const rust::cxxbridge1::Error& e) {
        success = false;
        output = message = FString::Printf(TEXT("DefiWalletCore InitializeWallet Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}
