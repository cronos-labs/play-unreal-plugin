
#include "DefiWalletCoreActor.h"

#include <iostream>
#include <sstream>

#include "TxBuilder.h"
#include "walletcore/include/defi-wallet-core-cpp/src/lib.rs.h"
#include "walletcore/include/rust/cxx.h"
using namespace std;
using namespace org::defi_wallet_core;

std::string to_hex(const unsigned char *data, int len);
void from_hex(const std::string &s, unsigned char *data, int len);

FCosmosNFTDenom convertDenom(::org::defi_wallet_core::Denom src) {
  FCosmosNFTDenom ret;
  ret.ID = UTF8_TO_TCHAR(src.id.c_str());
  ret.Name = UTF8_TO_TCHAR(src.name.c_str());
  ret.Schema = UTF8_TO_TCHAR(src.schema.c_str());
  ret.Creator = UTF8_TO_TCHAR(src.creator.c_str());
  return ret;
}

FCosmosNFTToken convertToken(::org::defi_wallet_core::BaseNft src) {
  FCosmosNFTToken ret;
  ret.ID = UTF8_TO_TCHAR(src.id.c_str());
  ret.Name = UTF8_TO_TCHAR(src.name.c_str());
  ret.URI = UTF8_TO_TCHAR(src.uri.c_str());
  ret.Data = UTF8_TO_TCHAR(src.data.c_str());
  ret.Owner = UTF8_TO_TCHAR(src.owner.c_str());
  return ret;
}

// Sets default values
ADefiWalletCoreActor::ADefiWalletCoreActor()
    : myCosmosRpc("http://mynode:1317"), myTendermintRpc("http://mynode:26657"),
      myChainID("testnet-baseball-1"), myCronosRpc("http://mynode:8545"),
      myCronosChainID(777)

{
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false;

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

void ADefiWalletCoreActor::GetNFTSupply(FString mygrpc, FString denomid,
                                        FString nftowner, int64 &output,
                                        bool &success, FString &output_message)

{
  try {
    std::string mygrpcstring = TCHAR_TO_UTF8(*mygrpc);
    rust::cxxbridge1::Box<GrpcClient> grpc_client =
        new_grpc_client(mygrpcstring);

    output = (int64)grpc_client->supply(TCHAR_TO_UTF8(*denomid),
                                        TCHAR_TO_UTF8(*nftowner));

    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore GetNFTSupply Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}
void ADefiWalletCoreActor::GetNFTOwner(FString mygrpc, FString denomid,
                                       FString nftowner,
                                       FCosmosNFTOwner &output, bool &success,
                                       FString &output_message) {
  try {
    std::string mygrpcstring = TCHAR_TO_UTF8(*mygrpc);
    rust::cxxbridge1::Box<GrpcClient> grpc_client =
        new_grpc_client(mygrpcstring);
    new_grpc_client(mygrpcstring);
    ::org::defi_wallet_core::Pagination defaultpagination;
    ::org::defi_wallet_core::Owner owner = grpc_client->owner(
        TCHAR_TO_UTF8(*denomid), TCHAR_TO_UTF8(*nftowner), defaultpagination);

    output.Address = UTF8_TO_TCHAR(owner.address.c_str());
    output.IDCollections.Empty();
    // prepare idcollections
    for (::org::defi_wallet_core::IdCollection &idcollection :
         owner.id_collections) {
      FCosmosNFTIDCollection newone;
      newone.DenomID = UTF8_TO_TCHAR(idcollection.denom_id.c_str());

      // prepar tokenids
      newone.TokenIDs.Empty();
      for (rust::cxxbridge1::String &tokenid : idcollection.token_ids) {
        newone.TokenIDs.Add(UTF8_TO_TCHAR(tokenid.c_str()));
      }
      assert(newone.TokenIDs.Num() == idcollection.token_ids.size());

      output.IDCollections.Add(newone);
    }
    assert(output.IDCollections.Num() == owner.id_collections.size());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore GetNFTOwner Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetNFTCollection(FString mygrpc, FString denomid,
                                            FCosmosNFTCollection &output,
                                            bool &success,
                                            FString &output_message) {
  try {
    std::string mygrpcstring = TCHAR_TO_UTF8(*mygrpc);
    rust::cxxbridge1::Box<GrpcClient> grpc_client =
        new_grpc_client(mygrpcstring);
    new_grpc_client(mygrpcstring);
    ::org::defi_wallet_core::Pagination defaultpagination;
    ::org::defi_wallet_core::Collection collection =
        grpc_client->collection(TCHAR_TO_UTF8(*denomid), defaultpagination);

    output.DenomOption = collection.denom_option;
    output.DenomValue = convertDenom(collection.denom_value);

    output.NFTs.Empty();
    for (::org::defi_wallet_core::BaseNft &nft : collection.nfts) {
      output.NFTs.Add(convertToken(nft));
    }
    assert(output.NFTs.Num() == collection.nfts.size());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message =
        FString::Printf(TEXT("DefiWalletCore GetNFTCollection Error: %s"),
                        UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetNFTDenom(FString mygrpc, FString denomid,
                                       FCosmosNFTDenom &output, bool &success,
                                       FString &output_message) {
  try {
    std::string mygrpcstring = TCHAR_TO_UTF8(*mygrpc);
    rust::cxxbridge1::Box<GrpcClient> grpc_client =
        new_grpc_client(mygrpcstring);
    new_grpc_client(mygrpcstring);

    ::org::defi_wallet_core::Denom denom =
        grpc_client->denom(TCHAR_TO_UTF8(*denomid));
    output = convertDenom(denom);

    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore GetNFTDenom Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetNFTDenomByName(FString mygrpc, FString denomname,
                                             FCosmosNFTDenom &output,
                                             bool &success,
                                             FString &output_message) {
  try {
    std::string mygrpcstring = TCHAR_TO_UTF8(*mygrpc);
    rust::cxxbridge1::Box<GrpcClient> grpc_client =
        new_grpc_client(mygrpcstring);
    new_grpc_client(mygrpcstring);
    ::org::defi_wallet_core::Denom denom =
        grpc_client->denom_by_name(TCHAR_TO_UTF8(*denomname));
    output = convertDenom(denom);

    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message =
        FString::Printf(TEXT("DefiWalletCore GetNFTDenomByName Error: %s"),
                        UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetNFTAllDenoms(FString mygrpc,
                                           TArray<FCosmosNFTDenom> &output,
                                           bool &success,
                                           FString &output_message) {
  try {
    std::string mygrpcstring = TCHAR_TO_UTF8(*mygrpc);

    rust::cxxbridge1::Box<GrpcClient> grpc_client =
        new_grpc_client(mygrpcstring);

    ::org::defi_wallet_core::Pagination defaultpagination;
    ::rust::Vec<::org::defi_wallet_core::Denom> denoms =
        grpc_client->denoms(defaultpagination);
    output.Empty();
    for (::org::defi_wallet_core::Denom &denom : denoms) {
      output.Add(convertDenom(denom));
    }
    assert(output.Num() == denoms.size());

    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message =
        FString::Printf(TEXT("DefiWalletCore GetNFTAllDenoms Error: %s"),
                        UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetNFTToken(FString mygrpc, FString denomid,
                                       FString tokenid, FCosmosNFTToken &output,
                                       bool &success, FString &output_message) {
  try {
    std::string mygrpcstring = TCHAR_TO_UTF8(*mygrpc);
    rust::cxxbridge1::Box<GrpcClient> grpc_client =
        new_grpc_client(mygrpcstring);

    ::org::defi_wallet_core::BaseNft nft =
        grpc_client->nft(TCHAR_TO_UTF8(*denomid), TCHAR_TO_UTF8(*tokenid));
    output.ID = UTF8_TO_TCHAR(nft.id.c_str());
    output.Name = UTF8_TO_TCHAR(nft.name.c_str());
    output.URI = UTF8_TO_TCHAR(nft.uri.c_str());
    output.Data = UTF8_TO_TCHAR(nft.data.c_str());
    output.Owner = UTF8_TO_TCHAR(nft.owner.c_str());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore GetNFTToken Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::SendAmount(int32 walletIndex, FString fromaddress,
                                      FString toaddress, int64 amount,
                                      FString amountdenom, FString &output,
                                      bool &success, FString &output_message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      output_message = TEXT("Invalid Wallet");
      return;
    }
    TxDirector director;
    CosmosSendAmountTxBuilder builder;
    director.setBuilder(&builder);
    CosmosSDKTxInfoRaw tx_info = director.makeTx();

    assert(NULL != _coreWallet);
    std::string mychainid = TCHAR_TO_UTF8(*myChainID);
    std::string myfrom = TCHAR_TO_UTF8(*fromaddress);
    std::string myto = TCHAR_TO_UTF8(*toaddress);
    uint64 myamount = (uint64)amount;
    std::string myamountdenom = TCHAR_TO_UTF8(*amountdenom);
    std::string myservercosmos = TCHAR_TO_UTF8(*myCosmosRpc); /* 1317 port */
    std::string myservertendermint =
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

    ::org::defi_wallet_core::CosmosTransactionReceiptRaw broadcastResult =
        broadcast_tx(myservertendermint, signedtx);
    rust::cxxbridge1::String txhash = broadcastResult.tx_hash_hex;

    UE_LOG(LogTemp, Log, TEXT("DefiWalletCore BroadcastTX Result %s"),
           UTF8_TO_TCHAR(txhash.c_str()));

    success = true;
    output = UTF8_TO_TCHAR(txhash.c_str());
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore SendAmount Error: %s"), UTF8_TO_TCHAR(e.what()));
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
        restore_wallet(TCHAR_TO_UTF8(*mnemonics), TCHAR_TO_UTF8(*password));
    // ownership transferred
    _coreWallet = tmpWallet.into_raw();
  }
}

void ADefiWalletCoreActor::InitializeWallet(FString mnemonics, FString password,
                                            FString &output, bool &success,
                                            FString &output_message) {
  try {
    if (NULL != _coreWallet) {
      success = false;
      output_message = TEXT("Wallet Already Exists");
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
    output_message =
        FString::Printf(TEXT("DefiWalletCore InitializeWallet Error: %s"),
                        UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetAddress(int32 index, FString &output,
                                      bool &success, FString &output_message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      output_message = TEXT("Invalid Wallet");
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
    output_message = FString::Printf(
        TEXT("DefiWalletCore GetAddress Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetBalance(FString address, FString denom,
                                      FString &output, bool &success,
                                      FString &output_message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      output_message = TEXT("Invalid Wallet");
      return;
    }
    assert(_coreWallet != NULL);

    std::string myservercosmos = TCHAR_TO_UTF8(*myCosmosRpc); /* 1317 port */
    rust::cxxbridge1::String balance = query_account_balance(
        myservercosmos, TCHAR_TO_UTF8(*address), TCHAR_TO_UTF8(*denom), 1);

    output = UTF8_TO_TCHAR(balance.c_str());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output = TEXT("");
    output_message = FString::Printf(
        TEXT("DefiWalletCore GetAddress Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetEthAddress(int32 index, FString &output,
                                         bool &success,
                                         FString &output_message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      output_message = TEXT("Invalid Wallet");
      return;
    }

    assert(_coreWallet != NULL);
    rust::cxxbridge1::String result = _coreWallet->get_eth_address(index);
    output = UTF8_TO_TCHAR(result.c_str());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output = TEXT("");
    output_message =
        FString::Printf(TEXT("DefiWalletCore GetEthAddress Error: %s"),
                        UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::GetEthBalance(FString address, FString &output,
                                         bool &success,
                                         FString &output_message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      output_message = TEXT("Invalid Wallet");
      return;
    }

    assert(_coreWallet != NULL);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc); /* 8545 port */
    std::string targetaddress = TCHAR_TO_UTF8(*address);
    rust::cxxbridge1::String result =
        get_eth_balance(targetaddress, mycronosrpc).to_string();
    output = UTF8_TO_TCHAR(result.c_str());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output = TEXT("");
    output_message =
        FString::Printf(TEXT("DefiWalletCore GetEthAddress Error: %s"),
                        UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::SendEthAmount(int32 walletIndex, FString fromaddress,
                                         FString toaddress, FString amount,
                                         FString gasLimit, FString gasPrice,
                                         FString &output, bool &success,
                                         FString &output_message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      output_message = TEXT("Invalid Wallet");
      return;
    }

    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc); /* 8545 port */

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
    rust::Vec<::std::uint8_t> signedtx = build_eth_signed_tx(
        eth_tx_info, (uint64)myCronosChainID, false, *privatekey);
    ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
        broadcast_eth_signed_raw_tx(signedtx, mycronosrpc, 1000);

    std::string txhash = to_hex(receipt.transaction_hash.data(),
                                receipt.transaction_hash.size());

    success = true;
    output = UTF8_TO_TCHAR(txhash.c_str());
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore SendAmount Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

TArray<uint8>
ADefiWalletCoreActor::SignEthAmount(int32 walletIndex, FString fromaddress,
                                    FString toaddress, FString amount,
                                    FString gasLimit, FString gasPrice,
                                    bool &success, FString &output_message) {
  TArray<uint8> output;
  try {
    if (NULL == _coreWallet) {
      success = false;
      output_message = TEXT("Invalid Wallet");
      return output;
    }

    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc); /* 8545 port */

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
    rust::Vec<::std::uint8_t> signedtx = build_eth_signed_tx(
        eth_tx_info, (uint64)myCronosChainID, false, *privatekey);

    int size = signedtx.size();
    output.Init(0, size);
    memcpy(output.GetData(), signedtx.data(), size);
    assert(output.size() == signedtx.size());

    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore SendAmount Error: %s"), UTF8_TO_TCHAR(e.what()));
  }

  return output;
}

void ADefiWalletCoreActor::SignLogin(int32 walletIndex, FString document,
                                     TArray<uint8> &signatureOutput,
                                     bool &success, FString &output_message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      output_message = TEXT("Invalid Wallet");
      return;
    }

    rust::cxxbridge1::Box<CppLoginInfo> logininfo =
        new_logininfo(TCHAR_TO_UTF8(*document));

    char hdpath[100];
    int coin_type = 60; // eth cointype
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
    output_message = FString::Printf(TEXT("DefiWalletCore SignLogin Error: %s"),
                                     UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::VerifyLogin(FString document,
                                       TArray<uint8> signatureOriginal,
                                       bool &success, FString &output_message) {
  try {
    if (NULL == _coreWallet) {
      success = false;
      output_message = TEXT("Invalid Wallet");
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
    output_message = FString::Printf(
        TEXT("DefiWalletCore VerifyLogin Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

std::string to_hex(const unsigned char *data, int len) {
  std::string s;
  for (int i = 0; i < len; i++) {
    s += "0123456789abcdef"[data[i] >> 4];
    s += "0123456789abcdef"[data[i] & 0xf];
  }
  return s;
}
// convert hex string to array
void from_hex(const std::string &s, unsigned char *data, int len) {
  for (int i = 0; i < len; i++) {
    data[i] = (unsigned char)strtol(s.substr(i * 2, 2).c_str(), NULL, 16);
  }
}

void ADefiWalletCoreActor::Erc20Balance(FString contractAddress,
                                        FString accountAddress,
                                        FString &balance, bool &success,
                                        FString &output_message) {
  try {
    std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
    std::string myaccountaddress = TCHAR_TO_UTF8(*accountAddress);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);

    Erc20 erc20 =
        new_erc20(mycontractaddress, mycronosrpc, myCronosChainID).legacy();
    U256 erc20_balance = erc20.balance_of(myaccountaddress);
    balance = UTF8_TO_TCHAR(erc20_balance.to_string().c_str());
    success = true;

  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore Erc20Balance Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::Erc721Balance(FString contractAddress,
                                         FString accountAddress,
                                         FString &balance, bool &success,
                                         FString &output_message) {
  try {
    std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
    std::string myaccountaddress = TCHAR_TO_UTF8(*accountAddress);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
    Erc721 erc721 =
        new_erc721(mycontractaddress, mycronosrpc, myCronosChainID).legacy();
    U256 erc721_balance = erc721.balance_of(myaccountaddress);
    balance = UTF8_TO_TCHAR(erc721_balance.to_string().c_str());
    success = true;

  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message =
        FString::Printf(TEXT("DefiWalletCore Erc721Balance Error: %s"),
                        UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::Erc1155Balance(FString contractAddress,
                                          FString accountAdress,
                                          FString tokenID, FString &balance,
                                          bool &success,
                                          FString &output_message) {
  try {
    std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
    std::string myaccountaddress = TCHAR_TO_UTF8(*accountAdress);
    std::string mytokenid = TCHAR_TO_UTF8(*tokenID);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
    Erc1155 erc1155 =
        new_erc1155(mycontractaddress, mycronosrpc, myCronosChainID).legacy();
    U256 erc1155_balance = erc1155.balance_of(myaccountaddress, mytokenid);
    balance = UTF8_TO_TCHAR(erc1155_balance.to_string().c_str());
    success = true;

  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message =
        FString::Printf(TEXT("DefiWalletCore Erc1155Balance Error: %s"),
                        UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::Erc721Name(FString contractAddress, FString &name,
                                      bool &success, FString &output_message) {
  try {
    std::string myaddress = TCHAR_TO_UTF8(*contractAddress);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
    ::org::defi_wallet_core::Erc721 erc721 =
        new_erc721(myaddress, mycronosrpc, myCronosChainID).legacy();
    rust::cxxbridge1::String erc721name = erc721.name();
    name = UTF8_TO_TCHAR(erc721name.c_str());
    success = true;

  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore Erc721Name Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::Erc721Symbol(FString contractAddress,
                                        FString &symbol, bool &success,
                                        FString &output_message) {
  try {
    std::string myaddress = TCHAR_TO_UTF8(*contractAddress);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
    ::org::defi_wallet_core::Erc721 erc721 =
        new_erc721(myaddress, mycronosrpc, myCronosChainID).legacy();
    rust::cxxbridge1::String erc721symbol = erc721.symbol();
    symbol = UTF8_TO_TCHAR(erc721symbol.c_str());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore Erc721Symbol Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::Erc721Uri(FString contractAddress, FString tokenID,
                                     FString &uri, bool &success,
                                     FString &output_message) {
  try {
    std::string myaddress = TCHAR_TO_UTF8(*contractAddress);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
    ::org::defi_wallet_core::Erc721 erc721 =
        new_erc721(myaddress, mycronosrpc, myCronosChainID).legacy();
    std::string mytokenid = TCHAR_TO_UTF8(*uri);
    rust::cxxbridge1::String erc721uri = erc721.token_uri(mytokenid);
    uri = UTF8_TO_TCHAR(erc721uri.c_str());
    success = true;

  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(TEXT("DefiWalletCore Erc721Uri Error: %s"),
                                     UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::Erc1155Uri(FString contractAddress, FString tokenID,
                                      FString &uri, bool &success,
                                      FString &output_message) {
  try {
    std::string myaddress = TCHAR_TO_UTF8(*contractAddress);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
    ::org::defi_wallet_core::Erc1155 erc1155 =
        new_erc1155(myaddress, mycronosrpc, myCronosChainID).legacy();
    std::string mytokenid = TCHAR_TO_UTF8(*uri);
    rust::cxxbridge1::String erc1155uri = erc1155.uri(mytokenid);
    uri = UTF8_TO_TCHAR(erc1155uri.c_str());
    success = true;

  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore Erc1155Uri Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::Erc721Owner(FString contractAddress, FString tokenID,
                                       FString &ercowner, bool &success,
                                       FString &output_message) {
  try {
    std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
    std::string mytokenid = TCHAR_TO_UTF8(*tokenID);
    Erc721 erc721 =
        new_erc721(mycontractaddress, mycronosrpc, myCronosChainID).legacy();
    rust::cxxbridge1::String erc721owner = erc721.owner_of(mytokenid);

    ercowner = UTF8_TO_TCHAR(erc721owner.c_str());
    success = true;

  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore Erc721Owner Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

// erc-20
void ADefiWalletCoreActor::Erc20Name(FString contractAddress, FString &name,
                                     bool &success, FString &output_message) {
  try {
    std::string myaddress = TCHAR_TO_UTF8(*contractAddress);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
    ::org::defi_wallet_core::Erc20 erc20 =
        new_erc20(myaddress, mycronosrpc, myCronosChainID).legacy();
    rust::cxxbridge1::String erc20name = erc20.name();
    name = UTF8_TO_TCHAR(erc20name.c_str());
    success = true;

  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(TEXT("DefiWalletCore Erc20Name Error: %s"),
                                     UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::Erc20Symbol(FString contractAddress, FString &symbol,
                                       bool &success, FString &output_message) {
  try {
    std::string myaddress = TCHAR_TO_UTF8(*contractAddress);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
    ::org::defi_wallet_core::Erc20 erc20 =
        new_erc20(myaddress, mycronosrpc, myCronosChainID).legacy();
    rust::cxxbridge1::String erc20symbol = erc20.symbol();
    symbol = UTF8_TO_TCHAR(erc20symbol.c_str());
    success = true;

  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("DefiWalletCore Erc20Symbol Error: %s"), UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::Erc20Decimals(FString contractAddress,
                                         int32 &decimals, bool &success,
                                         FString &output_message) {
  try {
    std::string myaddress = TCHAR_TO_UTF8(*contractAddress);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
    ::org::defi_wallet_core::Erc20 erc20 =
        new_erc20(myaddress, mycronosrpc, myCronosChainID).legacy();
    ::std::uint8_t erc20decimals = erc20.decimals();
    decimals = (int32)erc20decimals;
    success = true;

  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message =
        FString::Printf(TEXT("DefiWalletCore Erc20Decimals Error: %s"),
                        UTF8_TO_TCHAR(e.what()));
  }
}

void ADefiWalletCoreActor::Erc20TotalSupply(FString contractAddress,
                                            FString &totalSupply, bool &success,
                                            FString &output_message) {
  try {
    std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
    std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
    Erc20 erc20 =
        new_erc20(mycontractaddress, mycronosrpc, myCronosChainID).legacy();
    rust::cxxbridge1::String erc20totalSupply =
        erc20.total_supply().to_string();
    totalSupply = UTF8_TO_TCHAR(erc20totalSupply.c_str());

    success = true;

  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message =
        FString::Printf(TEXT("DefiWalletCore Erc20TotalSupply Error: %s"),
                        UTF8_TO_TCHAR(e.what()));
  }
}
