// Copyright 2022, Cronos Labs. All Rights Reserved

#include "PlayCppSdkBPLibrary.h"
#include "CronosPlayUnreal.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "PlayCppSdkDownloader.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/lib.rs.h"
#include "PlayCppSdkLibrary/Include/extra-cpp-bindings/src/lib.rs.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/ethereum.rs.h"
#include "PlayCppSdkLibrary/Include/rust/cxx.h"
#include "Runtime/Launch/Resources/Version.h"
using namespace std;
using namespace com::crypto::game_sdk;
using namespace rust;
using namespace org::defi_wallet_core;

void convertRawTx(Vec<RawTxDetail> &history, TArray<FRawTxDetail> &output) {
    output.Empty();
    for (auto &tx : history) {
        FRawTxDetail tx_detail;
        tx_detail.Hash = UTF8_TO_TCHAR(tx.hash.c_str());
        tx_detail.ToAddress = UTF8_TO_TCHAR(tx.to_address.c_str());
        tx_detail.FromAddress = UTF8_TO_TCHAR(tx.from_address.c_str());
        tx_detail.Value = UTF8_TO_TCHAR(tx.value.c_str());

        tx_detail.BlockNo = FString::Printf(TEXT("%llu"), tx.block_no);
        tx_detail.Timestamp = UTF8_TO_TCHAR(tx.timestamp.c_str());
        tx_detail.ContractAddress = UTF8_TO_TCHAR(tx.contract_address.c_str());
        output.Add(tx_detail);
    } // end of for loop
}
void convertQueryOption(EQueryOption &option, QueryOption &myoption) {
    switch (option) {
    case EQueryOption::TE_ByContract:
        myoption = QueryOption::ByContract;
    case EQueryOption::TE_ByAddressAndContract:
        myoption = QueryOption::ByAddressAndContract;
    case EQueryOption::TE_ByAddress:
        myoption = QueryOption::ByAddress;
    }
}
/// blackscout
void UPlayCppSdkBPLibrary::GetTokensBlocking(FString blockscoutBaseUrl,
                                             FString account_address,
                                             TArray<FRawTokenResult> &output,
                                             bool &success,
                                             FString &output_message) {
    try {
        std::string myblockscoutBaseUrl = TCHAR_TO_UTF8(*blockscoutBaseUrl);
        std::string myaccount_address = TCHAR_TO_UTF8(*account_address);
        Vec<RawTokenResult> history =
            get_tokens_blocking(myblockscoutBaseUrl, myaccount_address);

        output.Empty();
        for (auto &tx : history) {
            FRawTokenResult detail;
            detail.Balance = UTF8_TO_TCHAR(tx.balance.c_str());
            detail.ContractAddress = UTF8_TO_TCHAR(tx.contract_address.c_str());
            detail.Decimals = UTF8_TO_TCHAR(tx.decimals.c_str());
            detail.ID = UTF8_TO_TCHAR(tx.id.c_str());
            detail.Name = UTF8_TO_TCHAR(tx.name.c_str());
            detail.Symbol = UTF8_TO_TCHAR(tx.symbol.c_str());
            detail.TokenType = UTF8_TO_TCHAR(tx.token_type.c_str());
            output.Add(detail);
        }
        success = true;
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("PlayCppSdk GetTokensBlocking Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

/// blackscout
void UPlayCppSdkBPLibrary::GetTokenTransfersBlocking(
    FString blockscoutBaseUrl, FString address, FString contractAddress,
    EQueryOption option, TArray<FRawTxDetail> &output, bool &success,
    FString &output_message) {
    try {
        std::string myblockscoutBaseUrl = TCHAR_TO_UTF8(*blockscoutBaseUrl);
        std::string myaddress = TCHAR_TO_UTF8(*address);
        std::string mycontractAddress = TCHAR_TO_UTF8(*contractAddress);
        QueryOption myoption;
        convertQueryOption(option, myoption);

        Vec<RawTxDetail> history = get_token_transfers_blocking(
            myblockscoutBaseUrl, myaddress, mycontractAddress, myoption);
        convertRawTx(history, output);
        success = true;
    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("PlayCppSdk GetTokenTransfersBlocking Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

void UPlayCppSdkBPLibrary::GetErc20TransferHistoryBlocking(
    FString address, FString contractaddress, EQueryOption option,
    FString api_key, TArray<FRawTxDetail> &output, bool &success,
    FString &output_message) {
    try {
        std::string myaddress = TCHAR_TO_UTF8(*address);
        std::string mycontractaddress = TCHAR_TO_UTF8(*contractaddress);
        QueryOption myoption;
        std::string myapikey = TCHAR_TO_UTF8(*api_key);
        convertQueryOption(option, myoption);

        Vec<RawTxDetail> history = get_erc20_transfer_history_blocking(
            myaddress, mycontractaddress, myoption, myapikey);
        convertRawTx(history, output);
        success = true;
    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("PlayCppSdk GetErc20TransferHistoryBlocking Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

void UPlayCppSdkBPLibrary::GetErc721TransferHistoryBlocking(
    FString address, FString contractaddress, EQueryOption option,
    FString api_key, TArray<FRawTxDetail> &output, bool &success,
    FString &output_message) {
    try {
        std::string myaddress = TCHAR_TO_UTF8(*address);
        std::string mycontractaddress = TCHAR_TO_UTF8(*contractaddress);
        QueryOption myoption;
        std::string myapikey = TCHAR_TO_UTF8(*api_key);
        convertQueryOption(option, myoption);
        Vec<RawTxDetail> history = get_erc721_transfer_history_blocking(
            myaddress, mycontractaddress, myoption, myapikey);
        convertRawTx(history, output);
        success = true;
    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("PlayCppSdk GetErc721TransferHistoryBlocking Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

void UPlayCppSdkBPLibrary::GetTransactionHistoryBlocking(
    FString address, FString apikey, TArray<FRawTxDetail> &output,
    bool &success, FString &output_message) {
    try {
        std::string myaddress = TCHAR_TO_UTF8(*address);
        std::string myapikey = TCHAR_TO_UTF8(*apikey);
        Vec<RawTxDetail> history =
            get_transaction_history_blocking(myaddress, myapikey);
        convertRawTx(history, output);
        success = true;
    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("PlayCppSdk GetTransactionHistoryBlocking Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

FTexturePlatformData *GetTexturePlatformData(UTexture2D *Texture) {
#if ENGINE_MAJOR_VERSION == 4
    return Texture->PlatformData;
#else
    return Texture->GetPlatformData();
#endif
}

UTexture2D *UPlayCppSdkBPLibrary::GenerateQrCode(FString string) {
    try {
        ::com::crypto::game_sdk::WalletQrcode qr =
            generate_qrcode(TCHAR_TO_UTF8(*string));
        uint8 size = qr.size;
        TArray<FColor> pixels;
        pixels.SetNumZeroed(size * size);
        FColor black = FColor::Black;
        FColor white = FColor::White;
        for (uint8 y = 0; y < size; y++)
            for (uint8 x = 0; x < size; x++) {
                {
                    int index = x + y * size;
                    pixels[index] = qr.image[index] ? white : black;
                }
            }

        UTexture2D *texture = UTexture2D::CreateTransient(
            size, size, EPixelFormat::PF_B8G8R8A8, "QRCode");
        void *data = GetTexturePlatformData(texture)->Mips[0].BulkData.Lock(
            LOCK_READ_WRITE);
        FMemory::Memcpy(data, pixels.GetData(), size * size * 4);
        GetTexturePlatformData(texture)->Mips[0].BulkData.Unlock();
        texture->UpdateResource();
        texture->Filter = TextureFilter::TF_Nearest;
        return texture;

    } catch (const std::exception &e) {
        UE_LOG(LogTemp, Error, TEXT("PlayCppSdk GenerateQrCode Error: %s"),
               UTF8_TO_TCHAR(e.what()));
        return nullptr;
    }
}

UPlayCppSdkBPLibrary::UPlayCppSdkBPLibrary(
    const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer) {}

void UPlayCppSdkBPLibrary::SetupUserAgent(FString UserAgent) {
    UPlayCppSdkDownloader::UserAgent = UserAgent;
    try {
        org::defi_wallet_core::set_cronos_httpagent(TCHAR_TO_UTF8(*UserAgent));
    } catch (const std::exception &e) {
        // This captured exception only means the user agent is already set,
        // instead of an error
        UE_LOG(LogTemp, Display,
               TEXT("PlayCppSdk SetupUserAgent Already Setup: %s"),
               UTF8_TO_TCHAR(e.what()));
    }
}
