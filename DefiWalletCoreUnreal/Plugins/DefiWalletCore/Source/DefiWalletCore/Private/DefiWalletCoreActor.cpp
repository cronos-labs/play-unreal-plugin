// Fill out your copyright notice in the Description page of Project Settings.
#include "DefiWalletCoreActor.h"

#include <iostream>
#include <sstream>

#include "TxBuilder.h"
#include "cxx.h"
#include "lib.rs.h"
using namespace std;
using namespace org::defi_wallet_core;

// Sets default values
ADefiWalletCoreActor::ADefiWalletCoreActor()
    : myCosmosRpc("http://mynode:1317"),
      myTendermintRpc("http://mynode:26657"),
      myChainID("testnet-baseball-1"),
      myCronosRpc("http://mynode:8545")

{
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  _coreWallet = NULL;
}

// Called when the game starts or when spawned
void ADefiWalletCoreActor::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ADefiWalletCoreActor::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ADefiWalletCoreActor::Destroyed() {
  Super::Destroyed();

  if (GEngine)
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
                                     TEXT("ADefiWalletCoreActor  Destroyed"));

  DestroyWallet();

  assert(NULL == _coreWallet);
}

void ADefiWalletCoreActor::SendAmount(int32 walletIndex, FString fromaddress,
                                      FString toaddress, int64 amount,
                                      FString amountdenom, FString &output,
                                      bool &success, FString &message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      message = TEXT("Invalid Wallet");
      return;
    }
    TxDirector director;
    CosmosSendAmountTxBuilder builder;
    director.setBuilder(&builder);
    CosmosSDKTxInfoRaw tx_info = director.makeTx();

    assert(NULL != _coreWallet);
    string mychainid = TCHAR_TO_UTF8(*myChainID);
    string myfrom = TCHAR_TO_UTF8(*fromaddress);
    string myto = TCHAR_TO_UTF8(*toaddress);
    uint64 myamount = (uint64)amount;
    string myamountdenom = TCHAR_TO_UTF8(*amountdenom);
    string myservercosmos = TCHAR_TO_UTF8(*myCosmosRpc); /* 1317 port */
    string myservertendermint =
        TCHAR_TO_UTF8(*myTendermintRpc); /* 26657 port */
    UE_LOG(LogTemp, Log,
           TEXT("DefiWalletCore SendAmount from %s to %s amount %lld %s"),
           UTF8_TO_TCHAR(myfrom.c_str()), UTF8_TO_TCHAR(myto.c_str()), myamount,
           UTF8_TO_TCHAR(myamountdenom.c_str()));

    rust::cxxbridge1::String result =
        _coreWallet->get_default_address(CoinType::CryptoOrgMainnet);

    rust::cxxbridge1::String detailjson =
        query_account_details(myservercosmos, myfrom);

    org::defi_wallet_core::CosmosAccountInfoRaw detailinfo =
        query_account_details_info(myservercosmos, myfrom);
    tx_info.account_number = detailinfo.account_number;
    tx_info.sequence_number = detailinfo.sequence_number;
    tx_info.chain_id = mychainid;
    char hdpath[100];
    // m / purpose' / coin_type' / account' / change / address_index
    snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d", tx_info.coin_type,
             walletIndex);
    rust::cxxbridge1::Box<org::defi_wallet_core::PrivateKey> privatekey =
        _coreWallet->get_key(hdpath);

    rust::cxxbridge1::Vec<uint8_t> signedtx = get_single_bank_send_signed_tx(
        tx_info, *privatekey, myto, myamount, myamountdenom);
    rust::String broadcastResult = broadcast_tx(myservertendermint, signedtx);
    UE_LOG(LogTemp, Log, TEXT("DefiWalletCore BroadcastTX Result %s"),
           UTF8_TO_TCHAR(broadcastResult.c_str()));

    success = true;
    output = UTF8_TO_TCHAR(broadcastResult.c_str());
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    message = FString::Printf(TEXT("DefiWalletCore SendAmount Error: %s"),
                              UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::DestroyWallet() {
  if (_coreWallet != NULL) {
    // restored back
    rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> tmpwallet =
        rust::cxxbridge1::Box<org::defi_wallet_core::Wallet>::from_raw(
            _coreWallet);
    _coreWallet = NULL;
  }
}

void ADefiWalletCoreActor::CreateWallet(FString mnemonics, FString password) {
  if (NULL == _coreWallet) {
    rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> tmpWallet =
        restore_wallet(*mnemonics, *password);
    // ownership transferred
    _coreWallet = tmpWallet.into_raw();
  }
}

void ADefiWalletCoreActor::InitializeWallet(FString mnemonics, FString password,
                                            FString &output, bool &success,
                                            FString &message) {
  try {
    if (NULL != _coreWallet) {
      success = false;
      message = TEXT("Wallet Already Exists");
      return;
    }

    CreateWallet(mnemonics, password);
    assert(_coreWallet != NULL);
    rust::cxxbridge1::String result =
        _coreWallet->get_address(CoinType::CryptoOrgMainnet, 0);
    output = UTF8_TO_TCHAR(result.c_str());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output = TEXT("");
    message = FString::Printf(TEXT("DefiWalletCore InitializeWallet Error: %s"),
                              UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetAddress(int32 index, FString &output,
                                      bool &success, FString &message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      message = TEXT("Invalid Wallet");
      return;
    }

    assert(_coreWallet != NULL);
    rust::cxxbridge1::String result =
        _coreWallet->get_address(CoinType::CryptoOrgMainnet, index);
    output = UTF8_TO_TCHAR(result.c_str());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output = TEXT("");
    message = FString::Printf(TEXT("DefiWalletCore GetAddress Error: %s"),
                              UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetBalance(FString address, FString denom,
                                      FString &output, bool &success,
                                      FString &message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      message = TEXT("Invalid Wallet");
      return;
    }
    assert(_coreWallet != NULL);

    string myservercosmos = TCHAR_TO_UTF8(*myCosmosRpc); /* 1317 port */
    rust::cxxbridge1::String balance = query_account_balance(
        myservercosmos, TCHAR_TO_UTF8(*address), TCHAR_TO_UTF8(*denom), 1);

    output = UTF8_TO_TCHAR(balance.c_str());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output = TEXT("");
    message = FString::Printf(TEXT("DefiWalletCore GetAddress Error: %s"),
                              UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetEthAddress(int32 index, FString &output,
                                         bool &success, FString &message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      message = TEXT("Invalid Wallet");
      return;
    }

    assert(_coreWallet != NULL);
    rust::cxxbridge1::String result = _coreWallet->get_eth_address(index);
    output = UTF8_TO_TCHAR(result.c_str());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output = TEXT("");
    message = FString::Printf(TEXT("DefiWalletCore GetEthAddress Error: %s"),
                              UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetEthBalance(FString address, FString &output,
                                         bool &success, FString &message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      message = TEXT("Invalid Wallet");
      return;
    }

    assert(_coreWallet != NULL);
    string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc); /* 8545 port */
    string targetaddress = TCHAR_TO_UTF8(*address);
    rust::cxxbridge1::String result =
        get_eth_balance(targetaddress, mycronosrpc);
    output = UTF8_TO_TCHAR(result.c_str());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output = TEXT("");
    message = FString::Printf(TEXT("DefiWalletCore GetEthAddress Error: %s"),
                              UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::SendEthAmount(int32 walletIndex, int32 chainid,
                                         FString fromaddress, FString toaddress,
                                         FString amount, FString gasLimit,
                                         FString gasPrice, FString &output,
                                         bool &success, FString &message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      message = TEXT("Invalid Wallet");
      return;
    }

    string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc); /* 8545 port */

    rust::cxxbridge1::String myaddress1 =
        _coreWallet->get_eth_address(walletIndex);
    rust::cxxbridge1::String myaddress2 = TCHAR_TO_UTF8(*toaddress);
    rust::cxxbridge1::String nonce1 =
        get_eth_nonce(myaddress1.c_str(), mycronosrpc);
    char hdpath[100];
    int cointype = 60;
    snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d", cointype,
             walletIndex);
    rust::cxxbridge1::Box<PrivateKey> privatekey = _coreWallet->get_key(hdpath);
    rust::cxxbridge1::Vec<uint8_t> data;
    org::defi_wallet_core::EthTxInfoRaw eth_tx_info = new_eth_tx_info();
    cout << myaddress2 << endl;
    eth_tx_info.to_address = myaddress2.c_str();
    eth_tx_info.nonce = nonce1;
    eth_tx_info.gas_limit = TCHAR_TO_UTF8(*gasLimit);
    eth_tx_info.gas_price = TCHAR_TO_UTF8(*gasPrice);

    eth_tx_info.amount = TCHAR_TO_UTF8(*amount);
    eth_tx_info.amount_unit = org::defi_wallet_core::EthAmount::EthDecimal;
    rust::Vec<::std::uint8_t> signedtx =
        build_eth_signed_tx(eth_tx_info, (uint64)chainid, false, *privatekey);
    ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
        broadcast_eth_signed_raw_tx(signedtx, mycronosrpc);
    rust::cxxbridge1::String txhash = receipt.transaction_hash;

    success = true;
    output = UTF8_TO_TCHAR(txhash.c_str());
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    message = FString::Printf(TEXT("DefiWalletCore SendAmount Error: %s"),
                              UTF8_TO_TCHAR(e.what()));
  }
}

TArray<uint8> ADefiWalletCoreActor::SignEthAmount(
    int32 walletIndex, int32 chainid, FString fromaddress, FString toaddress,
    FString amount, FString gasLimit, FString gasPrice, bool &success,
    FString &message) {
  TArray<uint8> output;
  try {
    if (NULL == _coreWallet) {
      success = false;
      message = TEXT("Invalid Wallet");
      return output;
    }

    string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc); /* 8545 port */

    rust::cxxbridge1::String myaddress1 =
        _coreWallet->get_eth_address(walletIndex);
    rust::cxxbridge1::String myaddress2 = TCHAR_TO_UTF8(*toaddress);
    rust::cxxbridge1::String nonce1 =
        get_eth_nonce(myaddress1.c_str(), mycronosrpc);
    char hdpath[100];
    int cointype = 60;
    snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d", cointype,
             walletIndex);
    rust::cxxbridge1::Box<PrivateKey> privatekey = _coreWallet->get_key(hdpath);
    rust::cxxbridge1::Vec<uint8_t> data;
    org::defi_wallet_core::EthTxInfoRaw eth_tx_info = new_eth_tx_info();
    cout << myaddress2 << endl;
    eth_tx_info.to_address = myaddress2.c_str();
    eth_tx_info.nonce = nonce1;
    eth_tx_info.gas_limit = TCHAR_TO_UTF8(*gasLimit);
    eth_tx_info.gas_price = TCHAR_TO_UTF8(*gasPrice);

    eth_tx_info.amount = TCHAR_TO_UTF8(*amount);
    eth_tx_info.amount_unit = org::defi_wallet_core::EthAmount::EthDecimal;
    rust::Vec<::std::uint8_t> signedtx =
        build_eth_signed_tx(eth_tx_info, (uint64)chainid, false, *privatekey);

    int size = signedtx.size();
    int i;
    output.Init(0, size);  
    memcpy(output.GetData(), signedtx.data(), size);
    assert(output.size() == signedtx.size());

    success = true;

  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    message = FString::Printf(TEXT("DefiWalletCore SendAmount Error: %s"),
                              UTF8_TO_TCHAR(e.what()));
  }

  return output;
}

void ADefiWalletCoreActor::SignLogin(int32 walletIndex, FString document,
                                     TArray<uint8> &signatureOutput,
                                     bool &success, FString &message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      message = TEXT("Invalid Wallet");
      return;
    }

    rust::cxxbridge1::Box<CppLoginInfo> logininfo =
        new_logininfo(TCHAR_TO_UTF8(*document));

    char hdpath[100];
    int coin_type = 60;  // eth cointype
    snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d", coin_type,
             walletIndex);
    rust::cxxbridge1::Box<PrivateKey> privatekey = _coreWallet->get_key(hdpath);

    rust::cxxbridge1::String default_address =
        _coreWallet->get_address(CoinType::CronosMainnet, walletIndex);
    rust::cxxbridge1::Vec<uint8_t> signature =
        logininfo->sign_logininfo(*privatekey);
    assert(signature.size() == 65);
    // copy
    signatureOutput.Init(0, signature.size());
    int i, n = signature.size();
    for (i = 0; i < n; i++) {
      signatureOutput[i] = signature[i];
    }

    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    message = FString::Printf(TEXT("DefiWalletCore SignLogin Error: %s"),
                              UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::VerifyLogin(FString document,
                                       TArray<uint8> signatureOriginal,
                                       bool &success, FString &message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      message = TEXT("Invalid Wallet");
      return;
    }

    rust::cxxbridge1::Box<CppLoginInfo> logininfo =
        new_logininfo(TCHAR_TO_UTF8(*document));

    int i, n = signatureOriginal.Num();
    rust::cxxbridge1::Vec<uint8_t> signature;
    for (i = 0; i < n; i++) {
      signature.push_back(signatureOriginal[i]);
    }
    assert(signature.size() == signatureOriginal.Num());
    rust::Slice<const uint8_t> slice{signature.data(), signature.size()};
    logininfo->verify_logininfo(slice);
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    message = FString::Printf(TEXT("DefiWalletCore VerifyLogin Error: %s"),
                              UTF8_TO_TCHAR(e.what()));
  }
}
