
// Copyright 2022, Cronos Labs. All Rights Reserved
#include "DefiWalletCoreActor.h"

#include <iostream>
#include <sstream>

#include "Json.h"

#include "GenericPlatform/GenericPlatformHttp.h"
#include "Interfaces/IPluginManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayCppSdkBPLibrary.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/lib.rs.h"
#include "PlayCppSdkLibrary/Include/rust/cxx.h"
#include "TxBuilder.h"

#define SECURE_STORAGE_CLASS "com/cronos/play/SecureStorage"

#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#endif

using namespace std;
using namespace org::defi_wallet_core;
const int EthCoinType = 60;
auto to_hex(const unsigned char *data, int len) -> std::string;
void from_hex(const std::string &s, unsigned char *data, int len);
void zeroize_buffer(char *dst, char value, int length);
auto isSameAddress(std::string address1, std::string address2) -> bool;
void convertCronosTXReceipt(
    ::org::defi_wallet_core::CronosTransactionReceiptRaw &src,
    FCronosTransactionReceiptRaw &dst);
auto convertDenom(::org::defi_wallet_core::Denom src) -> FCosmosNFTDenom {
    FCosmosNFTDenom ret; // NOLINT
    ret.ID = UTF8_TO_TCHAR(src.id.c_str());
    ret.Name = UTF8_TO_TCHAR(src.name.c_str());
    ret.Schema = UTF8_TO_TCHAR(src.schema.c_str());
    ret.Creator = UTF8_TO_TCHAR(src.creator.c_str());
    return ret;
}

auto convertToken(::org::defi_wallet_core::BaseNft src) -> FCosmosNFTToken {
    FCosmosNFTToken ret; // NOLINT
    ret.ID = UTF8_TO_TCHAR(src.id.c_str());
    ret.Name = UTF8_TO_TCHAR(src.name.c_str());
    ret.URI = UTF8_TO_TCHAR(src.uri.c_str());
    ret.Data = UTF8_TO_TCHAR(src.data.c_str());
    ret.Owner = UTF8_TO_TCHAR(src.owner.c_str());
    return ret;
}

// Sets default values
ADefiWalletCoreActor::ADefiWalletCoreActor()
    : myGrpc("http://mynode:1316"), myCosmosRpc("http://mynode:1317"),
      myTendermintRpc("http://mynode:26657"), myChainID("testnet-baseball-1"),
      myCronosRpc("http://mynode:8545"), myCronosChainID(777)

{
    // Set this actor to call Tick() every frame.  You can turn this off to
    // improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    _coreWallet = NULL;

    IPluginManager &PluginManager = IPluginManager::Get();
    TSharedPtr<IPlugin> Plugin = PluginManager.FindPlugin("CronosPlayUnreal");
    FString useragentinfo =
        FString::Printf(TEXT("%s CronosPlayUnreal/%s"),
                        *FGenericPlatformHttp::GetDefaultUserAgent(),
                        *Plugin->GetDescriptor().VersionName);
    // UE_LOG(LogTemp, Display, TEXT("User Agent: %s"), *ua);
    UPlayCppSdkBPLibrary::SetupUserAgent(useragentinfo);
}

// Called when the game starts or when spawned
void ADefiWalletCoreActor::BeginPlay() { Super::BeginPlay(); }

// Called every frame
void ADefiWalletCoreActor::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void ADefiWalletCoreActor::Destroyed() {
    Super::Destroyed();

    DestroyWallet();

    assert(NULL == _coreWallet);
}

void ADefiWalletCoreActor::GetNFTSupply(FString denomid, FString nftowner,
                                        int64 &output, bool &success,
                                        FString &output_message)

{
    try {
        std::string mygrpcstring = TCHAR_TO_UTF8(*myGrpc);
        rust::cxxbridge1::Box<GrpcClient> grpc_client =
            new_grpc_client(mygrpcstring);

        output = (int64)grpc_client->supply(TCHAR_TO_UTF8(*denomid),
                                            TCHAR_TO_UTF8(*nftowner));

        success = true;
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal GetNFTSupply Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}
void ADefiWalletCoreActor::GetNFTOwner(FString denomid, FString nftowner,
                                       FCosmosNFTOwner &output, bool &success,
                                       FString &output_message) {
    try {
        std::string mygrpcstring = TCHAR_TO_UTF8(*myGrpc);
        rust::cxxbridge1::Box<GrpcClient> grpc_client =
            new_grpc_client(mygrpcstring);
        new_grpc_client(mygrpcstring);
        ::org::defi_wallet_core::Pagination defaultpagination;
        ::org::defi_wallet_core::Owner owner =
            grpc_client->owner(TCHAR_TO_UTF8(*denomid),
                               TCHAR_TO_UTF8(*nftowner), defaultpagination);

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
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal GetNFTOwner Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::GetNFTCollection(FString denomid,
                                            FCosmosNFTCollection &output,
                                            bool &success,
                                            FString &output_message) {
    try {
        std::string mygrpcstring = TCHAR_TO_UTF8(*myGrpc);
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
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal GetNFTCollection Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::GetNFTDenom(FString denomid, FCosmosNFTDenom &output,
                                       bool &success, FString &output_message) {
    try {
        std::string mygrpcstring = TCHAR_TO_UTF8(*myGrpc);
        rust::cxxbridge1::Box<GrpcClient> grpc_client =
            new_grpc_client(mygrpcstring);
        new_grpc_client(mygrpcstring);

        ::org::defi_wallet_core::Denom denom =
            grpc_client->denom(TCHAR_TO_UTF8(*denomid));
        output = convertDenom(denom);

        success = true;
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal GetNFTDenom Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::GetNFTDenomByName(FString denomname,
                                             FCosmosNFTDenom &output,
                                             bool &success,
                                             FString &output_message) {
    try {
        std::string mygrpcstring = TCHAR_TO_UTF8(*myGrpc);
        rust::cxxbridge1::Box<GrpcClient> grpc_client =
            new_grpc_client(mygrpcstring);
        new_grpc_client(mygrpcstring);
        ::org::defi_wallet_core::Denom denom =
            grpc_client->denom_by_name(TCHAR_TO_UTF8(*denomname));
        output = convertDenom(denom);

        success = true;
    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal GetNFTDenomByName Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::GetNFTAllDenoms(TArray<FCosmosNFTDenom> &output,
                                           bool &success,
                                           FString &output_message) {
    try {
        std::string mygrpcstring = TCHAR_TO_UTF8(*myGrpc);

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
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal GetNFTAllDenoms Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::GetNFTToken(FString denomid, FString tokenid,
                                       FCosmosNFTToken &output, bool &success,
                                       FString &output_message) {
    try {
        std::string mygrpcstring = TCHAR_TO_UTF8(*myGrpc);
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
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal GetNFTToken Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
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
        std::string myservercosmos =
            TCHAR_TO_UTF8(*myCosmosRpc); /* 1317 port */
        std::string myservertendermint =
            TCHAR_TO_UTF8(*myTendermintRpc); /* 26657 port */
        UE_LOG(LogTemp, Log,
               TEXT("CronosPlayUnreal SendAmount from %s to %s amount %lld %s"),
               UTF8_TO_TCHAR(myfrom.c_str()), UTF8_TO_TCHAR(myto.c_str()),
               myamount, UTF8_TO_TCHAR(myamountdenom.c_str()));

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

        rust::cxxbridge1::Vec<uint8_t> signedtx =
            get_single_bank_send_signed_tx(tx_info, *privatekey, myto, myamount,
                                           myamountdenom);

        ::org::defi_wallet_core::CosmosTransactionReceiptRaw broadcastResult =
            broadcast_tx(myservertendermint, signedtx);
        rust::cxxbridge1::String txhash = broadcastResult.tx_hash_hex;

        UE_LOG(LogTemp, Log, TEXT("CronosPlayUnreal BroadcastTX Result %s"),
               UTF8_TO_TCHAR(txhash.c_str()));

        success = true;
        output = UTF8_TO_TCHAR(txhash.c_str());
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal SendAmount Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::DestroyWallet() {
    UE_LOG(LogTemp, Log, TEXT("ADefiWalletCoreActor  DestroyWallet"));
    if (_coreWallet != NULL) {
        UE_LOG(LogTemp, Log, TEXT("ADefiWalletCoreActor  Removed CoreWallet"));

        // restored back
        rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> tmpwallet =
            rust::cxxbridge1::Box<org::defi_wallet_core::Wallet>::from_raw(
                _coreWallet);
        _coreWallet = NULL;
    }
}

/**
 * CAUTION: because mnemonics are not safe to be stored.
 * use only for testing & development purpose
 * use RestoreWalletSaveToSecureStorage for production
 *
 * WARNING!!!: never transfer menmonics to 3rd party library or networking, ipc,
 * logs or any kind of remote and zeroize after copying
 */
void ADefiWalletCoreActor::CreateWallet(FString mnemonics, FString password) {
    if (NULL == _coreWallet) {
        rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> tmpWallet =
            restore_wallet(TCHAR_TO_UTF8(*mnemonics), TCHAR_TO_UTF8(*password));
        // ownership transferred
        _coreWallet = tmpWallet.into_raw();
    }
}

/**
 * CAUTION: use only for testing & development purpose
 * storing mnemonics need caution, please not to expose user mnemonics for
 * public such as github, logs, files , etc.
 *
 * WARNING!!!: never transfer menmonics to 3rd party library or networking, ipc,
 * logs or any kind of remote and zeroize after copying
 */
void ADefiWalletCoreActor::RestoreWallet(FString mnemonics, FString password,
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
    } catch (const std::exception &e) {
        success = false;
        output = TEXT("");
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal InitializeWallet Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

TSharedPtr<FJsonObject> parseJson(FString json) {
    TSharedRef<TJsonReader<TCHAR>> Reader =
        TJsonReaderFactory<TCHAR>::Create(json);
    TSharedPtr<FJsonObject> JsonObject;
    FJsonSerializer::Deserialize(Reader, JsonObject);
    return JsonObject;
}

#if PLATFORM_ANDROID

jclass getSecureStorageClass(JNIEnv *env) {
    string secureStorageClass = SECURE_STORAGE_CLASS;
    jclass kotlinClass =
        FAndroidApplication::FindJavaClass(secureStorageClass.c_str());
    return kotlinClass;
}
jobject getContext(JNIEnv *env) {
    jclass activityThreadClass =
        FAndroidApplication::FindJavaClass("android/app/ActivityThread");
    jmethodID currentActivityThreadMethod =
        env->GetStaticMethodID(activityThreadClass, "currentActivityThread",
                               "()Landroid/app/ActivityThread;");
    jobject activityThread = env->CallStaticObjectMethod(
        activityThreadClass, currentActivityThreadMethod);
    jmethodID getApplicationMethod = env->GetMethodID(
        activityThreadClass, "getApplication", "()Landroid/app/Application;");
    jobject context =
        env->CallObjectMethod(activityThread, getApplicationMethod);
    return context;
}
int secureStorageWriteBasic(JNIEnv *env, string userkey, string uservalue) {

    jobject context = getContext(env);
    jclass secureStorageClass = getSecureStorageClass(env);
    jmethodID functionMethod = env->GetStaticMethodID(
        secureStorageClass, "writeSecureStorage",
        "(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;)I");
    jstring key = env->NewStringUTF(userkey.c_str());
    jstring value = env->NewStringUTF(uservalue.c_str());
    jint ret = env->CallStaticIntMethod(secureStorageClass, functionMethod,
                                        context, key, value);

    return (int)ret;
}

string secureStorageReadBasic(JNIEnv *env, string userkey) {
    jobject context = getContext(env);
    jclass secureStorageClass = getSecureStorageClass(env);
    jmethodID functionMethod = env->GetStaticMethodID(
        secureStorageClass, "readSecureStorage",
        "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;");
    jstring userkeyarg = env->NewStringUTF(userkey.c_str());
    jobject ret = env->CallStaticObjectMethod(
        secureStorageClass, functionMethod, context, userkeyarg);
    string retstring = string(env->GetStringUTFChars((jstring)ret, 0));

    return retstring;
}

void ADefiWalletCoreActor::RestoreWalletSaveToSecureStorage(
    FString mnemonics, FString password, FString servicename, FString username,
    FString &output, bool &success, FString &output_message) {

    try {
        if (NULL != _coreWallet) {
            success = false;
            output_message = TEXT("Wallet Already Exists");
            return;
        }

        char tmp[1000];
        snprintf(tmp, sizeof(tmp), "%s_%s", TCHAR_TO_UTF8(*servicename),
                 TCHAR_TO_UTF8(*username));
        string keyvalue(tmp);

        snprintf(tmp, sizeof(tmp),
                 "{\\\"mnemonic\\\":\\\"%s\\\",\\\"password\\\":\\\"%s\\\"}",
                 TCHAR_TO_UTF8(*mnemonics), TCHAR_TO_UTF8(*password));

        string datavalue(tmp);
        JNIEnv *env = FAndroidApplication::GetJavaEnv(true);
        int ret = secureStorageWriteBasic(env, keyvalue, datavalue);
        if (ret == 0) {
            success = false;
            output_message = TEXT("Save Secure Storage Failed");
            return;
        }
        rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> tmpWallet =
            restore_wallet(TCHAR_TO_UTF8(*mnemonics), TCHAR_TO_UTF8(*password));

        _coreWallet = tmpWallet.into_raw();
        assert(_coreWallet != NULL);
        rust::cxxbridge1::String result =
            _coreWallet->get_address(CoinType::CryptoOrgMainnet, 0);
        output = UTF8_TO_TCHAR(result.c_str());
        success = true;
    } catch (const std::exception &e) {
        success = false;
        output = TEXT("");
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal RestoreWalletSaveToSecureStorage Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::RestoreWalletLoadFromSecureStorage(
    FString servicename, FString username, FString &output, bool &success,
    FString &output_message) {

    try {
        if (NULL != _coreWallet) {
            success = false;
            output_message = TEXT("Wallet Already Exists");
            return;
        }
        char tmp[1000];
        snprintf(tmp, sizeof(tmp), "%s_%s", TCHAR_TO_UTF8(*servicename),
                 TCHAR_TO_UTF8(*username));
        string keyvalue(tmp);
        // success: {"result":"value","success":"1","error":""}
        // fail: {"result":"","success":"0","error":"encrypt file not found"}
        JNIEnv *env = FAndroidApplication::GetJavaEnv(true);
        string datavalue = secureStorageReadBasic(env, keyvalue);
        TSharedPtr<FJsonObject> resultjsonobject =
            parseJson(UTF8_TO_TCHAR(datavalue.c_str()));
        FString SuccessValue =
            resultjsonobject->GetStringField(TEXT("success"));
        FString ErrorValue = resultjsonobject->GetStringField(TEXT("error"));
        if (SuccessValue == TEXT("0")) {
            success = false;
            // format TEXT
            output_message = FString::Printf(
                TEXT("CronosPlayUnreal RestoreWalletLoadFromSecureStorage "
                     "Error: %s"),
                *ErrorValue);
            return;
        }
        // {"mnemonic":"value","password":"value"}
        FString InfoValue = resultjsonobject->GetStringField(TEXT("result"));
        TSharedPtr<FJsonObject> infojsonobject = parseJson(InfoValue);
        FString mnemonics = infojsonobject->GetStringField(TEXT("mnemonic"));
        FString password = infojsonobject->GetStringField(TEXT("password"));
        rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> tmpWallet =
            restore_wallet(TCHAR_TO_UTF8(*mnemonics), TCHAR_TO_UTF8(*password));
        _coreWallet = tmpWallet.into_raw();
        assert(_coreWallet != NULL);
        rust::cxxbridge1::String result =
            _coreWallet->get_address(CoinType::CryptoOrgMainnet, 0);
        output = UTF8_TO_TCHAR(result.c_str());
        success = true;
    } catch (const std::exception &e) {
        success = false;
        output = TEXT("");
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal RestoreWalletLoadFromSecureStorage Error: "
                 "%s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

#else

void ADefiWalletCoreActor::RestoreWalletSaveToSecureStorage(
    FString mnemonics, FString password, FString servicename, FString username,
    FString &output, bool &success, FString &output_message) {
    try {
        if (NULL != _coreWallet) {
            success = false;
            output_message = TEXT("Wallet Already Exists");
            return;
        }

        rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> tmpWallet =
            restore_wallet_save_to_securestorage(
                TCHAR_TO_UTF8(*mnemonics), TCHAR_TO_UTF8(*password),
                TCHAR_TO_UTF8(*servicename), TCHAR_TO_UTF8(*username));
        _coreWallet = tmpWallet.into_raw();

        assert(_coreWallet != NULL);
        rust::cxxbridge1::String result =
            _coreWallet->get_address(CoinType::CryptoOrgMainnet, 0);
        output = UTF8_TO_TCHAR(result.c_str());
        success = true;
    } catch (const std::exception &e) {
        success = false;
        output = TEXT("");
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal RestoreWalletSaveToSecureStorage Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::RestoreWalletLoadFromSecureStorage(
    FString servicename, FString username, FString &output, bool &success,
    FString &output_message) {
    try {
        if (NULL != _coreWallet) {
            success = false;
            output_message = TEXT("Wallet Already Exists");
            return;
        }

        rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> tmpWallet =
            restore_wallet_load_from_securestorage(TCHAR_TO_UTF8(*servicename),
                                                   TCHAR_TO_UTF8(*username));
        _coreWallet = tmpWallet.into_raw();

        assert(_coreWallet != NULL);
        rust::cxxbridge1::String result =
            _coreWallet->get_address(CoinType::CryptoOrgMainnet, 0);
        output = UTF8_TO_TCHAR(result.c_str());
        success = true;
    } catch (const std::exception &e) {
        success = false;
        output = TEXT("");
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal RestoreWalletLoadFromSecureStorage Error: "
                 "%s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

#endif

/**
 * CAUTION: use only for testing & development purpose
 * storing mnemonics need caution, please not to expose user mnemonics for
 * public such as github, logs, files , etc.
 *
 * WARNING!!!: never transfer menmonics to 3rd party library or networking, ipc,
 * logs or any kind of remote and zeroize after copying
 */
void ADefiWalletCoreActor::InitializeNewWallet(FString password,
                                               EMnemonicsWordCount wordcount,
                                               FString &output, bool &success,
                                               FString &output_message) {
    try {
        if (NULL != _coreWallet) {
            success = false;
            output_message = TEXT("Wallet Already Exists");
            return;
        }

        ::org::defi_wallet_core::MnemonicWordCount mywordcount;
        switch (wordcount) {
        case EMnemonicsWordCount::Twelve:
            mywordcount = ::org::defi_wallet_core::MnemonicWordCount::Twelve;
            break;
        case EMnemonicsWordCount::Eighteen:
            mywordcount = ::org::defi_wallet_core::MnemonicWordCount::Eighteen;
            break;
        case EMnemonicsWordCount::TwentyFour:
            mywordcount =
                ::org::defi_wallet_core::MnemonicWordCount::TwentyFour;
            break;
        default:
            throw "Invalid Word Count";
            break;
        }
        rust::cxxbridge1::Box<org::defi_wallet_core::Wallet> tmpWallet =
            new_wallet(TCHAR_TO_UTF8(*password), mywordcount);
        // ownership transferred
        _coreWallet = tmpWallet.into_raw();

        assert(_coreWallet != NULL);
        rust::cxxbridge1::String result =
            _coreWallet->get_address(CoinType::CryptoOrgMainnet, 0);
        output = UTF8_TO_TCHAR(result.c_str());
        // zeroize mnemonics

        zeroize_buffer((char *)result.data(), 0, result.size());

        success = true;
    } catch (const std::exception &e) {
        success = false;
        output = TEXT("");
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal InitializeWallet Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

/**
 * CAUTION: use only for testing & development purpose
 * storing mnemonics need caution, please not to expose user mnemonics for
 * public such as github, logs, files , etc.
 *
 * WARNING!!!: never transfer menmonics to 3rd party library or networking, ipc,
 * logs or any kind of remote and zeroize after copying
 */
void ADefiWalletCoreActor::GetBackupMnemonicPhrase(FString &output,
                                                   bool &success,
                                                   FString &output_message) {

    try {
        if (NULL == _coreWallet) {
            success = false;
            output_message = TEXT("Invalid Wallet");
            return;
        }

        assert(_coreWallet != NULL);
        rust::cxxbridge1::String result =
            _coreWallet->get_backup_mnemonic_phrase();
        output = UTF8_TO_TCHAR(result.c_str());
        // zeroize mnemonics
        zeroize_buffer((char *)result.data(), 0, result.size());

        success = true;
    } catch (const std::exception &e) {
        success = false;
        output = TEXT("");
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal GetBackupMnemonicPhrase Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

/**
 * CAUTION: use only for testing & development purpose
 * storing mnemonics need caution, please not to expose user mnemonics for
 * public such as github, logs, files , etc.
 *
 * WARNING!!!: never transfer menmonics to 3rd party library or networking, ipc,
 * logs or any kind of remote and zeroize after copying
 */
void ADefiWalletCoreActor::GenerateMnemonics(FString password,
                                             EMnemonicsWordCount wordcount,
                                             FString &output, bool &success,
                                             FString &output_message) {
    try {

        ::org::defi_wallet_core::MnemonicWordCount mywordcount;
        switch (wordcount) {
        case EMnemonicsWordCount::Twelve:
            mywordcount = ::org::defi_wallet_core::MnemonicWordCount::Twelve;
            break;
        case EMnemonicsWordCount::Eighteen:
            mywordcount = ::org::defi_wallet_core::MnemonicWordCount::Eighteen;
            break;
        case EMnemonicsWordCount::TwentyFour:
            mywordcount =
                ::org::defi_wallet_core::MnemonicWordCount::TwentyFour;
            break;
        default:
            throw "Invalid Word Count";
            break;
        }
        rust::String result =
            generate_mnemonics(TCHAR_TO_UTF8(*password), mywordcount);

        output = UTF8_TO_TCHAR(result.c_str());
        // zeroize mnemonics
        zeroize_buffer((char *)result.data(), 0, result.size());

        success = true;
    } catch (const std::exception &e) {
        success = false;
        output = TEXT("");
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal DevelopmentOnlyGenerateMnemonics Error: %s"),
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
    } catch (const std::exception &e) {
        success = false;
        output = TEXT("");
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal GetAddress Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
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

        std::string myservergrpc = TCHAR_TO_UTF8(*myGrpc); /* 1316 port */
        rust::cxxbridge1::String balance = query_account_balance(
            myservergrpc, TCHAR_TO_UTF8(*address), TCHAR_TO_UTF8(*denom));

        output = UTF8_TO_TCHAR(balance.c_str());
        success = true;
    } catch (const std::exception &e) {
        success = false;
        output = TEXT("");
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal GetAddress Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
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
    } catch (const std::exception &e) {
        success = false;
        output = TEXT("");
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal GetEthAddress Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::GetEthBalance(FString address, FString &output,
                                         bool &success,
                                         FString &output_message) {
    try {
        std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
        std::string targetaddress = TCHAR_TO_UTF8(*address);
        rust::cxxbridge1::String result =
            get_eth_balance(targetaddress, mycronosrpc).to_string();
        output = UTF8_TO_TCHAR(result.c_str());
        success = true;
    } catch (const std::exception &e) {
        success = false;
        output = TEXT("");
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal GetEthAddress Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::BroadcastEthTxAsync(FWalletBroadcastDelegate Out,
                                               TArray<uint8> usersignedtx,
                                               FString rpc) {
    AsyncTask(
        ENamedThreads::AnyHiPriThreadNormalTask, [Out, usersignedtx, rpc]() {
            rust::Vec<::std::uint8_t> signedtx;
            for (int i = 0; i < usersignedtx.Num(); i++) {
                signedtx.push_back(usersignedtx[i]);
            }
            assert(signedtx.size() == usersignedtx.size());

            std::string mycronosrpc = TCHAR_TO_UTF8(*rpc);

            FString txhashtext;
            FString result;
            try {
                ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
                    broadcast_eth_signed_raw_tx(signedtx, mycronosrpc, 1000);

                std::string txhash = to_hex(receipt.transaction_hash.data(),
                                            receipt.transaction_hash.size());

                txhashtext = UTF8_TO_TCHAR(txhash.c_str());
            } catch (const std::exception &e) {
                result = FString::Printf(
                    TEXT("CronosPlayUnreal BroadcastEthTxAsync Error: %s"),
                    UTF8_TO_TCHAR(e.what()));
            }

            AsyncTask(ENamedThreads::GameThread, [Out, txhashtext, result]() {
                Out.ExecuteIfBound(txhashtext, result);
            });
        });
}

auto toLower(std::string strToConvert) -> std::string {
    std::transform(strToConvert.begin(), strToConvert.end(),
                   strToConvert.begin(), ::tolower);
    return strToConvert;
}

auto isSameAddress(std::string address1, std::string address2) -> bool {
    return toLower(address1) == toLower(address2);
}

void ADefiWalletCoreActor::SendEthAmount(
    int32 walletIndex, FString fromaddress, FString toaddress,
    FString amountInEthDecimal, FString gasLimit, FString gasPriceInWei,
    TArray<uint8> txdata, FSendEthTransferDelegate Out) {

    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out, walletIndex,
                                                        fromaddress, toaddress,
                                                        amountInEthDecimal,
                                                        gasLimit, gasPriceInWei,
                                                        txdata]() {
        FString result;
        FCronosTransactionReceiptRaw txresult;

        try {
            rust::cxxbridge1::String myfromaddress =
                _coreWallet->get_eth_address(walletIndex);
            rust::cxxbridge1::String mytoaddress = TCHAR_TO_UTF8(*toaddress);

            if (NULL == _coreWallet) {
                result = TEXT("Invalid Wallet");
            } else if (!isSameAddress(myfromaddress.c_str(),
                                      TCHAR_TO_UTF8(*fromaddress))) {
                result = TEXT("Invalid From Address");
            } else {
                std::string mycronosrpc =
                    TCHAR_TO_UTF8(*myCronosRpc); /* 8545 port */

                rust::cxxbridge1::String nonce1 =
                    get_eth_nonce(myfromaddress.c_str(), mycronosrpc);
                char hdpath[100];
                snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d",
                         EthCoinType, walletIndex);
                rust::cxxbridge1::Box<PrivateKey> privatekey =
                    _coreWallet->get_key(hdpath);
                rust::cxxbridge1::Vec<uint8_t> data;
                org::defi_wallet_core::EthTxInfoRaw eth_tx_info =
                    new_eth_tx_info();
                eth_tx_info.to_address = mytoaddress.c_str();
                eth_tx_info.nonce = nonce1;
                eth_tx_info.gas_limit = TCHAR_TO_UTF8(*gasLimit);
                eth_tx_info.gas_price = TCHAR_TO_UTF8(*gasPriceInWei);

                eth_tx_info.amount = TCHAR_TO_UTF8(*amountInEthDecimal);
                eth_tx_info.amount_unit =
                    org::defi_wallet_core::EthAmount::EthDecimal;

                // data
                eth_tx_info.data.clear();
                for (int i = 0; i < txdata.Num(); i++) {
                    eth_tx_info.data.push_back(txdata[i]);
                }
                assert(eth_tx_info.data.size() == txdata.Num());

                rust::Vec<::std::uint8_t> signedtx = build_eth_signed_tx(
                    eth_tx_info, (uint64)myCronosChainID, false, *privatekey);
                ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
                    broadcast_eth_signed_raw_tx(signedtx, mycronosrpc, 1000);

                convertCronosTXReceipt(receipt, txresult);
            }
        } catch (const std::exception &e) {
            result =
                FString::Printf(TEXT("CronosPlayUnreal SendAmount Error: %s"),
                                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, txresult, result]() {
            Out.ExecuteIfBound(txresult, result);
        });
    });
}

TArray<uint8> ADefiWalletCoreActor::SignEthAmount(
    int32 walletIndex, FString fromaddress, FString toaddress, FString amount,
    FString gasLimit, FString gasPrice, TArray<uint8> txdata, bool &success,
    FString &output_message) {
    TArray<uint8> output;
    try {
        if (NULL == _coreWallet) {
            success = false;
            output_message = TEXT("Invalid Wallet");
            return output;
        }
        if (walletIndex < 0) {
            success = false;
            output_message = TEXT("Wallet Index is invalid");
            return output;
        }

        std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc); /* 8545 port */

        rust::cxxbridge1::String myfromaddress =
            _coreWallet->get_eth_address(walletIndex);
        if (!isSameAddress(myfromaddress.c_str(),
                           TCHAR_TO_UTF8(*fromaddress))) {
            success = false;
            output_message = TEXT("Invalid From Address");
            return output;
        }

        rust::cxxbridge1::String mytoaddress = TCHAR_TO_UTF8(*toaddress);
        rust::cxxbridge1::String nonce1 =
            get_eth_nonce(myfromaddress.c_str(), mycronosrpc);
        char hdpath[100];
        snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d", EthCoinType,
                 walletIndex);
        rust::cxxbridge1::Box<PrivateKey> privatekey =
            _coreWallet->get_key(hdpath);
        rust::cxxbridge1::Vec<uint8_t> data;
        org::defi_wallet_core::EthTxInfoRaw eth_tx_info = new_eth_tx_info();
        eth_tx_info.to_address = mytoaddress.c_str();
        eth_tx_info.nonce = nonce1;
        eth_tx_info.gas_limit = TCHAR_TO_UTF8(*gasLimit);
        eth_tx_info.gas_price = TCHAR_TO_UTF8(*gasPrice);

        eth_tx_info.amount = TCHAR_TO_UTF8(*amount);
        eth_tx_info.amount_unit = org::defi_wallet_core::EthAmount::EthDecimal;

        // data
        eth_tx_info.data.clear();
        for (int i = 0; i < txdata.Num(); i++) {
            eth_tx_info.data.push_back(txdata[i]);
        }
        assert(eth_tx_info.data.size() == txdata.Num());

        // sign
        rust::Vec<::std::uint8_t> signedtx = build_eth_signed_tx(
            eth_tx_info, (uint64)myCronosChainID, false, *privatekey);

        int size = signedtx.size();
        output.Init(0, size);
        memcpy(output.GetData(), signedtx.data(), size);
        assert(output.size() == signedtx.size());

        success = true;
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal SendAmount Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
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
        if (walletIndex < 0) {
            success = false;
            output_message = TEXT("Wallet Index is invalid");
            return;
        }

        rust::cxxbridge1::Box<CppLoginInfo> logininfo =
            new_logininfo(TCHAR_TO_UTF8(*document));

        char hdpath[100];
        int coin_type = 60; // eth cointype
        snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d", coin_type,
                 walletIndex);
        rust::cxxbridge1::Box<PrivateKey> privatekey =
            _coreWallet->get_key(hdpath);

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
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal SignLogin Error: %s"),
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
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal VerifyLogin Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

auto to_hex(const unsigned char *data, int len) -> std::string {
    std::string s; // NOLINT
    std::string hexstring = "0123456789abcdef";
    for (int i = 0; i < len; i++) {
        s += hexstring[(int)(data[i] >> 4)];
        s += hexstring[(int)(data[i] & 0xf)];
    }
    return s;
}
// convert hex string to array
void from_hex(const std::string &s, unsigned char *data, int len) {
    for (int i = 0; i < len; i++) {
        // NOLINTNEXTLINE
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

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc20Balance Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
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
            new_erc721(mycontractaddress, mycronosrpc, myCronosChainID)
                .legacy();
        U256 erc721_balance = erc721.balance_of(myaccountaddress);
        balance = UTF8_TO_TCHAR(erc721_balance.to_string().c_str());
        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc721Balance Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::Erc1155Balance(FString contractAddress,
                                          FString accountAddress,
                                          FString tokenID, FString &balance,
                                          bool &success,
                                          FString &output_message) {
    try {
        std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
        std::string myaccountaddress = TCHAR_TO_UTF8(*accountAddress);
        std::string mytokenid = TCHAR_TO_UTF8(*tokenID);
        std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
        Erc1155 erc1155 =
            new_erc1155(mycontractaddress, mycronosrpc, myCronosChainID)
                .legacy();
        U256 erc1155_balance = erc1155.balance_of(myaccountaddress, mytokenid);
        balance = UTF8_TO_TCHAR(erc1155_balance.to_string().c_str());
        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc1155Balance Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::Erc1155BalanceOfBatch(
    FString contractAddress, TArray<FString> accountAddresses,
    TArray<FString> tokenIDs, TArray<FString> &balanceofbatch, bool &success,
    FString &output_message) {
    try {
        std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);

        rust::Vec<rust::String> myaccountaddresses;
        for (int i = 0; i < accountAddresses.Num(); i++) {
            myaccountaddresses.push_back(TCHAR_TO_UTF8(*accountAddresses[i]));
        }

        rust::Vec<rust::String> mytokenids;
        for (int i = 0; i < tokenIDs.Num(); i++) {
            mytokenids.push_back(TCHAR_TO_UTF8(*tokenIDs[i]));
        }
        std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
        Erc1155 erc1155 =
            new_erc1155(mycontractaddress, mycronosrpc, myCronosChainID)
                .legacy();
        ::rust::Vec<::rust::String> erc1155_balances =
            erc1155.balance_of_batch(myaccountaddresses, mytokenids);

        balanceofbatch.Empty();
        for (int i = 0; i < erc1155_balances.size(); i++) {
            balanceofbatch.Add(UTF8_TO_TCHAR(erc1155_balances[i].c_str()));
        }
        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal Erc1155BalanceOfBatch Error: %s"),
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

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc721Name Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
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
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc721Symbol Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
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
        std::string mytokenid = TCHAR_TO_UTF8(*tokenID); /* bug fix*/
        rust::cxxbridge1::String erc721uri = erc721.token_uri(mytokenid);
        uri = UTF8_TO_TCHAR(erc721uri.c_str());
        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc721Uri Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::Erc721GetApproved(FString contractAddress,
                                             FString tokenID, FString &result,
                                             bool &success,
                                             FString &output_message) {
    try {
        std::string myaddress = TCHAR_TO_UTF8(*contractAddress);
        std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
        ::org::defi_wallet_core::Erc721 erc721 =
            new_erc721(myaddress, mycronosrpc, myCronosChainID).legacy();
        std::string mytokenid = TCHAR_TO_UTF8(*tokenID); /* bug fix*/
        rust::cxxbridge1::String erc721getapproved =
            erc721.get_approved(mytokenid);
        result = UTF8_TO_TCHAR(erc721getapproved.c_str());
        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal Erc721GetApproved Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::Erc721IsApprovedForAll(FString contractAddress,
                                                  FString erc721owner,
                                                  FString erc721approvedaddress,
                                                  bool &result, bool &success,
                                                  FString &output_message) {
    try {
        std::string myaddress = TCHAR_TO_UTF8(*contractAddress);
        std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
        ::org::defi_wallet_core::Erc721 erc721 =
            new_erc721(myaddress, mycronosrpc, myCronosChainID).legacy();
        std::string myowner = TCHAR_TO_UTF8(*erc721owner);
        std::string myapprovedaddress = TCHAR_TO_UTF8(*erc721approvedaddress);
        result = erc721.is_approved_for_all(myowner, myapprovedaddress);
        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal Erc721IsApprovedForAll Error: %s"),
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
        std::string mytokenid = TCHAR_TO_UTF8(*tokenID);
        rust::cxxbridge1::String erc1155uri = erc1155.uri(mytokenid);
        uri = UTF8_TO_TCHAR(erc1155uri.c_str());
        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc1155Uri Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::Erc1155IsApprovedForAll(
    FString contractAddress, FString erc1155owner,
    FString erc1155approvedaddress, bool &result, bool &success,
    FString &output_message) {
    try {
        std::string myaddress = TCHAR_TO_UTF8(*contractAddress);
        std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
        ::org::defi_wallet_core::Erc1155 erc1155 =
            new_erc1155(myaddress, mycronosrpc, myCronosChainID).legacy();
        std::string myowner = TCHAR_TO_UTF8(*erc1155owner);
        std::string myapprovedaddress = TCHAR_TO_UTF8(*erc1155approvedaddress);
        result = erc1155.is_approved_for_all(myowner, myapprovedaddress);
        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal Erc11551IsApprovedForAll Error: %s"),
            UTF8_TO_TCHAR(e.what()));
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
            new_erc721(mycontractaddress, mycronosrpc, myCronosChainID)
                .legacy();
        rust::cxxbridge1::String erc721owner = erc721.owner_of(mytokenid);

        ercowner = UTF8_TO_TCHAR(erc721owner.c_str());
        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc721Owner Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::Erc721TotalSupply(FString contractAddress,
                                             FString &totalsupply,
                                             bool &success,
                                             FString &output_message) {
    try {
        std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
        std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
        Erc721 erc721 =
            new_erc721(mycontractaddress, mycronosrpc, myCronosChainID)
                .legacy();
        rust::cxxbridge1::String erc721totalsupply =
            erc721.total_supply().to_string();

        totalsupply = UTF8_TO_TCHAR(erc721totalsupply.c_str());
        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal Erc721TotalSupply Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::Erc721TokenByIndex(FString contractAddress,
                                              FString erc721index,
                                              FString &token, bool &success,
                                              FString &output_message) {
    try {
        std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
        std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
        std::string myindex = TCHAR_TO_UTF8(*erc721index);
        Erc721 erc721 =
            new_erc721(mycontractaddress, mycronosrpc, myCronosChainID)
                .legacy();
        rust::cxxbridge1::String erc721token = erc721.token_by_index(myindex);

        token = UTF8_TO_TCHAR(erc721token.c_str());
        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal Erc721TokenByIndex Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::Erc721TokenOwnerByIndex(
    FString contractAddress, FString erc721owner, FString erc721index,
    FString &token, bool &success, FString &output_message) {
    try {
        std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
        std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
        std::string myowner = TCHAR_TO_UTF8(*erc721owner);
        std::string myindex = TCHAR_TO_UTF8(*erc721index);
        Erc721 erc721 =
            new_erc721(mycontractaddress, mycronosrpc, myCronosChainID)
                .legacy();
        rust::cxxbridge1::String erc721token =
            erc721.token_of_owner_by_index(myowner, myindex);

        token = UTF8_TO_TCHAR(erc721token.c_str());
        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal Erc721TokenOwnerByIndex Error: %s"),
            UTF8_TO_TCHAR(e.what()));
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

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc20Name Error: %s"),
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

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc20Symbol Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
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

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc20Decimals Error: %s"),
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

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc20TotalSupply Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void convertVecToTArray(const ::rust::Vec<::std::uint8_t> &src,
                        TArray<uint8> &dst) {
    dst.Empty();
    for (int i = 0; i < src.size(); i++) {
        dst.Add(src[i]);
    }
    assert(src.size() == dst.Num());
}
void convertCronosTXReceipt(
    ::org::defi_wallet_core::CronosTransactionReceiptRaw &src,
    FCronosTransactionReceiptRaw &dst) {
    convertVecToTArray(src.transaction_hash, dst.TransationHash);
    convertVecToTArray(src.block_hash, dst.BlockHash);
    dst.BlockNumber = UTF8_TO_TCHAR(src.block_number.c_str());
    dst.CumulativeGasUsed = UTF8_TO_TCHAR(src.cumulative_gas_used.c_str());
    dst.GasUsed = UTF8_TO_TCHAR(src.gas_used.c_str());
    dst.ContractAddress = UTF8_TO_TCHAR(src.contract_address.c_str());
    dst.Logs.Empty();
    for (int i = 0; i < src.logs.size(); i++) {
        dst.Logs.Add(UTF8_TO_TCHAR(src.logs[i].c_str()));
    }
    dst.Status = UTF8_TO_TCHAR(src.status.c_str());
    convertVecToTArray(src.root, dst.Root);
    convertVecToTArray(src.logs_bloom, dst.LogsBloom);
    dst.TransactionType = UTF8_TO_TCHAR(src.transaction_type.c_str());
    dst.EffectiveGasPrice = UTF8_TO_TCHAR(src.effective_gas_price.c_str());
}

void ADefiWalletCoreActor::Erc20Transfer(FString contractAddress,
                                         int32 walletindex, FString toAddress,
                                         FString amount,
                                         FErc20TransferFromDelegate Out) {

    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out,
                                                        contractAddress,
                                                        walletindex, toAddress,
                                                        amount]() {
        FString result;
        FCronosTransactionReceiptRaw txresult;

        try {
            if (NULL == _coreWallet) {
                result = TEXT("Invalid Wallet");
            } else {
                char hdpath[100];
                snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d",
                         EthCoinType, walletindex);
                rust::cxxbridge1::Box<PrivateKey> privatekey =
                    _coreWallet->get_key(hdpath);

                std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
                std::string mytoaddress = TCHAR_TO_UTF8(*toAddress);
                std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
                std::string myamount = TCHAR_TO_UTF8(*amount);

                Erc20 erc20 =
                    new_erc20(mycontractaddress, mycronosrpc, myCronosChainID)
                        .legacy();
                ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
                    erc20.transfer(mytoaddress, myamount, *privatekey);
                convertCronosTXReceipt(receipt, txresult);
            }

        } catch (const std::exception &e) {
            result = FString::Printf(
                TEXT("CronosPlayUnreal Erc20Transfer Error: %s"),
                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, txresult, result]() {
            Out.ExecuteIfBound(txresult, result);
        });
    });
}

void ADefiWalletCoreActor::Erc20TransferFrom(FString contractAddress,
                                             int32 walletindex,
                                             FString fromAddress,
                                             FString toAddress, FString amount,
                                             FErc20TransferFromDelegate Out) {

    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out,
                                                        contractAddress,
                                                        walletindex,
                                                        fromAddress, toAddress,
                                                        amount]() {
        FString result;
        FCronosTransactionReceiptRaw txresult;

        try {
            if (NULL == _coreWallet) {

                result = TEXT("Invalid Wallet");

            } else {
                char hdpath[100];
                snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d",
                         EthCoinType, walletindex);
                rust::cxxbridge1::Box<PrivateKey> privatekey =
                    _coreWallet->get_key(hdpath);

                std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
                std::string myfromaddress = TCHAR_TO_UTF8(*fromAddress);
                std::string mytoaddress = TCHAR_TO_UTF8(*toAddress);
                std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
                std::string myamount = TCHAR_TO_UTF8(*amount);

                Erc20 erc20 =
                    new_erc20(mycontractaddress, mycronosrpc, myCronosChainID)
                        .legacy();
                ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
                    erc20.transfer_from(myfromaddress, mytoaddress, myamount,
                                        *privatekey);
                convertCronosTXReceipt(receipt, txresult);
            }

        } catch (const std::exception &e) {
            result = FString::Printf(
                TEXT("CronosPlayUnreal Erc20TransferFrom Error: %s"),
                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, txresult, result]() {
            Out.ExecuteIfBound(txresult, result);
        });
    });
}

void ADefiWalletCoreActor::Erc20Approve(FString contractAddress,
                                        int32 walletindex,
                                        FString approvedAddress, FString amount,
                                        FErc20ApproveDelegate Out) {

    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out,
                                                        contractAddress,
                                                        walletindex,
                                                        approvedAddress,
                                                        amount]() {
        FString result;
        FCronosTransactionReceiptRaw txresult;
        try {
            if (NULL == _coreWallet) {
                result = TEXT("Invalid Wallet");
            } else {
                char hdpath[100];
                snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d",
                         EthCoinType, walletindex);
                rust::cxxbridge1::Box<PrivateKey> privatekey =
                    _coreWallet->get_key(hdpath);

                std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
                std::string myapprovedAddress = TCHAR_TO_UTF8(*approvedAddress);
                std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
                std::string myamount = TCHAR_TO_UTF8(*amount);

                Erc20 erc20 =
                    new_erc20(mycontractaddress, mycronosrpc, myCronosChainID)
                        .legacy();
                ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
                    erc20.approve(myapprovedAddress, myamount, *privatekey);
                convertCronosTXReceipt(receipt, txresult);
            }

        } catch (const std::exception &e) {
            result =
                FString::Printf(TEXT("CronosPlayUnreal Erc20Approve Error: %s"),
                                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, txresult, result]() {
            Out.ExecuteIfBound(txresult, result);
        });
    });
}

void ADefiWalletCoreActor::Erc20Allowance(FString contractAddress,
                                          FString erc20owner,
                                          FString erc20spender, FString &result,
                                          bool &success,
                                          FString &output_message) {
    try {
        std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
        std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);

        std::string myowner = TCHAR_TO_UTF8(*erc20owner);
        std::string myspender = TCHAR_TO_UTF8(*erc20spender);
        Erc20 erc20 =
            new_erc20(mycontractaddress, mycronosrpc, myCronosChainID).legacy();
        rust::cxxbridge1::String erc20allowance =
            erc20.allowance(myowner, myspender);
        result = UTF8_TO_TCHAR(erc20allowance.c_str());

        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal Erc20Allowance Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void ADefiWalletCoreActor::Erc721TransferFrom(
    FString contractAddress, int32 walletindex, FString fromAddress,
    FString toAddress, FString tokenid, FErc721TransferFromDelegate Out) {
    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out,
                                                        contractAddress,
                                                        walletindex,
                                                        fromAddress, toAddress,
                                                        tokenid]() {
        FCronosTransactionReceiptRaw txresult;
        FString result;
        try {
            if (NULL == _coreWallet) {
                result = TEXT("Invalid Wallet");
            } else {
                char hdpath[100];
                snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d",
                         EthCoinType, walletindex);
                rust::cxxbridge1::Box<PrivateKey> privatekey =
                    _coreWallet->get_key(hdpath);

                std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
                std::string myfromaddress = TCHAR_TO_UTF8(*fromAddress);
                std::string mytoaddress = TCHAR_TO_UTF8(*toAddress);
                std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
                std::string mytokenid = TCHAR_TO_UTF8(*tokenid);

                Erc721 erc721 =
                    new_erc721(mycontractaddress, mycronosrpc, myCronosChainID)
                        .legacy();
                ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
                    erc721.transfer_from(myfromaddress, mytoaddress, mytokenid,
                                         *privatekey);
                convertCronosTXReceipt(receipt, txresult);
            }
        } catch (const std::exception &e) {
            result = FString::Printf(
                TEXT("CronosPlayUnreal Erc721TransferFrom Error: %s"),
                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, txresult, result]() {
            Out.ExecuteIfBound(txresult, result);
        });
    });
}

void ADefiWalletCoreActor::Erc721SafeTransferFrom(
    FString contractAddress, int32 walletindex, FString fromAddress,
    FString toAddress, FString tokenid, FErc721TransferFromDelegate Out) {
    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out,
                                                        contractAddress,
                                                        walletindex,
                                                        fromAddress, toAddress,
                                                        tokenid]() {
        FString result;
        FCronosTransactionReceiptRaw txresult;
        try {
            if (NULL == _coreWallet) {
                result = TEXT("Invalid Wallet");
            } else {
                char hdpath[100];
                snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d",
                         EthCoinType, walletindex);
                rust::cxxbridge1::Box<PrivateKey> privatekey =
                    _coreWallet->get_key(hdpath);

                std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
                std::string myfromaddress = TCHAR_TO_UTF8(*fromAddress);
                std::string mytoaddress = TCHAR_TO_UTF8(*toAddress);
                std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
                std::string mytokenid = TCHAR_TO_UTF8(*tokenid);

                Erc721 erc721 =
                    new_erc721(mycontractaddress, mycronosrpc, myCronosChainID)
                        .legacy();
                ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
                    erc721.safe_transfer_from(myfromaddress, mytoaddress,
                                              mytokenid, *privatekey);
                convertCronosTXReceipt(receipt, txresult);
            }

        } catch (const std::exception &e) {
            result = FString::Printf(
                TEXT("CronosPlayUnreal Erc721SafeTransferFrom Error: %s"),
                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, txresult, result]() {
            Out.ExecuteIfBound(txresult, result);
        });
    });
}

void ADefiWalletCoreActor::Erc721SafeTransferFromWithData(
    FString contractAddress, int32 walletindex, FString fromAddress,
    FString toAddress, FString tokenid, TArray<uint8> additionaldata,
    FErc721TransferFromDelegate Out) {
    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out,
                                                        contractAddress,
                                                        walletindex,
                                                        fromAddress, toAddress,
                                                        tokenid,
                                                        additionaldata]() {
        FString result;
        FCronosTransactionReceiptRaw txresult;
        try {
            if (NULL == _coreWallet) {
                result = TEXT("Invalid Wallet");
            } else {
                char hdpath[100];
                snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d",
                         EthCoinType, walletindex);
                rust::cxxbridge1::Box<PrivateKey> privatekey =
                    _coreWallet->get_key(hdpath);

                std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
                std::string myfromaddress = TCHAR_TO_UTF8(*fromAddress);
                std::string mytoaddress = TCHAR_TO_UTF8(*toAddress);
                std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
                std::string mytokenid = TCHAR_TO_UTF8(*tokenid);

                ::rust::Vec<::std::uint8_t> myadditionaldata;
                myadditionaldata.clear();
                for (int i = 0; i < additionaldata.Num(); i++) {
                    myadditionaldata.push_back(additionaldata[i]);
                }
                assert(myadditionaldata.size() == additionaldata.Num());

                Erc721 erc721 =
                    new_erc721(mycontractaddress, mycronosrpc, myCronosChainID)
                        .legacy();
                ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
                    erc721.safe_transfer_from_with_data(
                        myfromaddress, mytoaddress, mytokenid, myadditionaldata,
                        *privatekey);
                convertCronosTXReceipt(receipt, txresult);
            }

        } catch (const std::exception &e) {
            result = FString::Printf(
                TEXT("CronosPlayUnreal Erc721SafeTransferFromWithData Error: "
                     "%s"),
                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, txresult, result]() {
            Out.ExecuteIfBound(txresult, result);
        });
    });
}

void ADefiWalletCoreActor::Erc721Approve(FString contractAddress,
                                         int32 walletindex,
                                         FString approvedAddress,
                                         FString tokenid,
                                         FErc721ApproveDelegate Out) {
    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out,
                                                        contractAddress,
                                                        walletindex,
                                                        approvedAddress,
                                                        tokenid]() {
        FString result;
        FCronosTransactionReceiptRaw txresult;
        try {
            if (NULL == _coreWallet) {
                result = TEXT("Invalid "
                              "Wallet");
            } else {
                char hdpath[100];
                snprintf(hdpath, sizeof(hdpath),
                         "m/44'/%d'/0'/"
                         "0/%d",
                         EthCoinType, walletindex);
                rust::cxxbridge1::Box<PrivateKey> privatekey =
                    _coreWallet->get_key(hdpath);

                std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
                std::string myapprovedAddress = TCHAR_TO_UTF8(*approvedAddress);
                std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
                std::string mytokenid = TCHAR_TO_UTF8(*tokenid);

                Erc721 erc721 =
                    new_erc721(mycontractaddress, mycronosrpc, myCronosChainID)
                        .legacy();
                ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
                    erc721.approve(myapprovedAddress, mytokenid, *privatekey);
                convertCronosTXReceipt(receipt, txresult);
            }

        } catch (const std::exception &e) {
            result = FString::Printf(
                TEXT("CronosPlayUnreal Erc721Approve Error: %s"),
                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, txresult, result]() {
            Out.ExecuteIfBound(txresult, result);
        });
    });
}

void ADefiWalletCoreActor::Erc1155SafeTransferFrom(
    FString contractAddress, int32 walletindex, FString fromAddress,
    FString toAddress, FString tokenid, FString amount,
    TArray<uint8> additionaldata, FErc1155TransferFromDelegate Out) {
    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out,
                                                        contractAddress,
                                                        walletindex,
                                                        fromAddress, toAddress,
                                                        tokenid, amount,
                                                        additionaldata]() {
        FString result;
        FCronosTransactionReceiptRaw txresult;
        try {
            if (NULL == _coreWallet) {
                result = TEXT("Invalid Wallet");
            } else {
                char hdpath[100];
                snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d",
                         EthCoinType, walletindex);
                rust::cxxbridge1::Box<PrivateKey> privatekey =
                    _coreWallet->get_key(hdpath);

                std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
                std::string myfromaddress = TCHAR_TO_UTF8(*fromAddress);
                std::string mytoaddress = TCHAR_TO_UTF8(*toAddress);
                std::string mytokenid = TCHAR_TO_UTF8(*tokenid);
                std::string myamount = TCHAR_TO_UTF8(*amount);
                std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
                ::rust::Vec<::std::uint8_t> myadditionaldata;
                myadditionaldata.clear();
                for (int i = 0; i < additionaldata.Num(); i++) {
                    myadditionaldata.push_back(additionaldata[i]);
                }
                assert(myadditionaldata.size() == additionaldata.Num());

                Erc1155 erc1155 =
                    new_erc1155(mycontractaddress, mycronosrpc, myCronosChainID)
                        .legacy();
                ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
                    erc1155.safe_transfer_from(myfromaddress, mytoaddress,
                                               mytokenid, myamount,
                                               myadditionaldata, *privatekey);

                convertCronosTXReceipt(receipt, txresult);
            }

        } catch (const std::exception &e) {
            result = FString::Printf(
                TEXT("CronosPlayUnreal Erc1155SafeTransferFrom Error: %s"),
                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, txresult, result]() {
            Out.ExecuteIfBound(txresult, result);
        });
    });
}

void ADefiWalletCoreActor::Erc1155SafeBatchTransferFrom(
    FString contractAddress, int32 walletindex, FString fromAddress,
    FString toAddress, TArray<FString> tokenids, TArray<FString> amounts,
    TArray<uint8> additionaldata, FErc1155TransferFromDelegate Out) {

    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out,
                                                        contractAddress,
                                                        walletindex,
                                                        fromAddress, toAddress,
                                                        tokenids, amounts,
                                                        additionaldata]() {
        FString result;
        FCronosTransactionReceiptRaw txresult;

        try {
            if (NULL == _coreWallet) {
                result = TEXT("Invalid Wallet");
            } else {
                char hdpath[100];
                snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d",
                         EthCoinType, walletindex);
                rust::cxxbridge1::Box<PrivateKey> privatekey =
                    _coreWallet->get_key(hdpath);

                std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
                std::string myfromaddress = TCHAR_TO_UTF8(*fromAddress);
                std::string mytoaddress = TCHAR_TO_UTF8(*toAddress);
                rust::Vec<rust::String> mytokenids;
                for (int i = 0; i < tokenids.Num(); i++) {
                    mytokenids.push_back(TCHAR_TO_UTF8(*tokenids[i]));
                }

                rust::Vec<rust::String> myamounts;
                for (int i = 0; i < amounts.Num(); i++) {
                    myamounts.push_back(TCHAR_TO_UTF8(*amounts[i]));
                }
                std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);
                ::rust::Vec<::std::uint8_t> myadditionaldata;
                myadditionaldata.clear();
                for (int i = 0; i < additionaldata.Num(); i++) {
                    myadditionaldata.push_back(additionaldata[i]);
                }
                assert(myadditionaldata.size() == additionaldata.Num());

                Erc1155 erc1155 =
                    new_erc1155(mycontractaddress, mycronosrpc, myCronosChainID)
                        .legacy();
                ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
                    erc1155.safe_batch_transfer_from(
                        myfromaddress, mytoaddress, mytokenids, myamounts,
                        myadditionaldata, *privatekey);
                convertCronosTXReceipt(receipt, txresult);
            }
        } catch (const std::exception &e) {
            result = FString::Printf(
                TEXT("CronosPlayUnreal Erc1155SafeBatchTransferFrom Error: %s"),
                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, txresult, result]() {
            Out.ExecuteIfBound(txresult, result);
        });
    });
}

void ADefiWalletCoreActor::Erc1155Approve(FString contractAddress,
                                          int32 walletindex,
                                          FString approvedAddress,
                                          bool approved,
                                          FErc1155ApproveDelegate Out) {

    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out,
                                                        contractAddress,
                                                        walletindex,
                                                        approvedAddress,
                                                        approved]() {
        FString result;
        FCronosTransactionReceiptRaw txresult;

        try {
            if (NULL == _coreWallet) {
                result = TEXT("Invalid Wallet");
            } else {
                char hdpath[100];
                snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d",
                         EthCoinType, walletindex);
                rust::cxxbridge1::Box<PrivateKey> privatekey =
                    _coreWallet->get_key(hdpath);

                std::string mycontractaddress = TCHAR_TO_UTF8(*contractAddress);
                std::string myapprovedAddress = TCHAR_TO_UTF8(*approvedAddress);
                std::string mycronosrpc = TCHAR_TO_UTF8(*myCronosRpc);

                Erc1155 erc1155 =
                    new_erc1155(mycontractaddress, mycronosrpc, myCronosChainID)
                        .legacy();
                ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
                    erc1155.set_approval_for_all(myapprovedAddress, approved,
                                                 *privatekey);
                convertCronosTXReceipt(receipt, txresult);
            }
        } catch (const std::exception &e) {
            result = FString::Printf(
                TEXT("CronosPlayUnreal Erc1155Approve Error: %s"),
                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, txresult, result]() {
            Out.ExecuteIfBound(txresult, result);
        });
    });
}

UDynamicContractObject *
ADefiWalletCoreActor::CreateDynamicContract(FString contractaddress,
                                            FString abijson, bool &success,
                                            FString &output_message) {
    UDynamicContractObject *ret = NewObject<UDynamicContractObject>();
    ret->defiWallet = this;
    ret->NewEthContract(contractaddress, abijson, success, output_message);
    return ret;
}

org::defi_wallet_core::Wallet *ADefiWalletCoreActor::getCoreWallet() {
    return _coreWallet;
}

UDynamicContractObject *ADefiWalletCoreActor::CreateDynamicSigningContract(
    FString contractaddress, FString abijson, int32 walletindex, bool &success,
    FString &output_message) {
    UDynamicContractObject *ret = NewObject<UDynamicContractObject>();
    ret->defiWallet = this;
    ret->NewSigningEthContract(contractaddress, abijson, walletindex, success,
                               output_message);
    return ret;
}

void zeroize_buffer(char *dst, char value, int length) {
    for (int i = 0; i < length; i++) {
        dst[i] = value; // NOLINT
    }
}
