#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "PlayCppSdkBPLibrary.h"
#include "CronosPlayUnreal.h"
#include "cronosplay/include/extra-cpp-bindings/src/lib.rs.h"
#include "cronosplay/include/rust/cxx.h"
#include "qrcodegen.hpp"
using namespace std;
using namespace com::crypto::game_sdk;
using namespace rust;

static FString g_src_ipfs = TEXT("ipfs://");
static FString g_dst_ipfs = TEXT("https://ipfs.io/ipfs/");
static FString g_user_agent = "CronosPlay-UnrealEngine-Agent";

void UPlayCppSdkBPLibrary::SetupUserAgent(FString useragent) {
  g_user_agent = useragent;
}
// ipfs:// -> https://ipfs.io/ipfs/
FString convert_ipfs_url(const FString &src) {
  FString dst = src.Replace(*g_src_ipfs, *g_dst_ipfs);
  return dst;
}

void UPlayCppSdkBPLibrary::SetupIpfsConverting(FString src, FString dst) {
  g_src_ipfs = src;
  g_dst_ipfs = dst;
}

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
  } catch (const rust::cxxbridge1::Error &e) {
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
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message =
        FString::Printf(TEXT("PlayCppSdk GetTokenTransfersBlocking Error: %s"),
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
  } catch (const rust::cxxbridge1::Error &e) {
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
  } catch (const rust::cxxbridge1::Error &e) {
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
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(
        TEXT("PlayCppSdk GetTransactionHistoryBlocking Error: %s"),
        UTF8_TO_TCHAR(e.what()));
  }
}

UTexture2D *UPlayCppSdkBPLibrary::GenerateQrCode(FString string) {
  qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(
      TCHAR_TO_UTF8(*string), qrcodegen::QrCode::Ecc::LOW);
  uint8 size = qr.getSize();
  TArray<FColor> pixels;
  pixels.SetNumZeroed(size * size);
  FColor black = FColor::Black;
  FColor white = FColor::White;
  for (uint8 x = 0; x < size; x++) {
    for (uint8 y = 0; y < size; y++) {
      FColor color = qr.getModule(x, y) ? white : black;
      pixels[x + y * size] = color;
    }
  }
  UTexture2D *texture = UTexture2D::CreateTransient(
      size, size, EPixelFormat::PF_B8G8R8A8, "QRCode");
  void *data = texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
  FMemory::Memcpy(data, pixels.GetData(), size * size * 4);
  texture->PlatformData->Mips[0].BulkData.Unlock();
  texture->UpdateResource();
  texture->Filter = TextureFilter::TF_Nearest;
  return texture;
}

UPlayCppSdkBPLibrary::UPlayCppSdkBPLibrary(
    const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer) {}

void UPlayCppSdkBPLibrary::GetJsonStringFromUri(FGetJsonStringFromUri Out,
                                                FString tokenuriuser) {

  FString tokenuri = convert_ipfs_url(tokenuriuser);

  AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [Out, tokenuri]() {
    FHttpModule &httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httprequest =
        httpModule.CreateRequest();
    httprequest->SetVerb(TEXT("GET"));
    httprequest->SetHeader(TEXT("User-Agent"), g_user_agent);
    httprequest->SetHeader(TEXT("Content-Type"),
                           TEXT("application/x-www-form-urlencoded"));
    httprequest->SetURL(tokenuri);

    httprequest->OnProcessRequestComplete().BindLambda(
        [&](FHttpRequestPtr callbackrequest, FHttpResponsePtr callbackresponse,
            bool connectedSuccessfully) mutable {
          FString jsonstring;
          FString result;
          if (connectedSuccessfully) {
            FString response = callbackresponse->GetContentAsString();
            jsonstring = response;
            result = TEXT("");
          } else {
            switch (callbackrequest->GetStatus()) {
            case EHttpRequestStatus::Failed_ConnectionError:
              result = TEXT("Connection failed.");
            default:
              result = TEXT("Request failed.");
            }
          }

          AsyncTask(ENamedThreads::GameThread, [Out, jsonstring, result]() {
            Out.ExecuteIfBound(jsonstring, result);
          });
        });

    httprequest->ProcessRequest();
  });
}

void UPlayCppSdkBPLibrary::GetNftImageInfoFromUri(FGetNftImageInfoFromUri Out,
                                                  FString tokenuriuser) {
  FString tokenuri = convert_ipfs_url(tokenuriuser);

  AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [Out, tokenuri]() {
    FHttpModule &httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httprequest =
        httpModule.CreateRequest();
    httprequest->SetVerb(TEXT("GET"));
    httprequest->SetHeader(TEXT("Content-Type"),
                           TEXT("application/x-www-form-urlencoded"));
    httprequest->SetHeader(TEXT("User-Agent"), g_user_agent);
    httprequest->SetURL(tokenuri);

    httprequest->OnProcessRequestComplete().BindLambda(

        [&](FHttpRequestPtr callbackrequest, FHttpResponsePtr callbackresponse,
            bool connectedSuccessfully) mutable {
          FString result;
          FString jsonstring;
          FCronosNftInfo nftinfo;
          if (connectedSuccessfully) {
            FString response = callbackresponse->GetContentAsString();
            jsonstring = response;
            TSharedRef<TJsonReader<TCHAR>> jsonreader =
                TJsonReaderFactory<TCHAR>::Create(jsonstring);
            TSharedPtr<FJsonObject> jsonobject =
                MakeShareable(new FJsonObject());
            if (!FJsonSerializer::Deserialize(jsonreader, jsonobject) ||
                !jsonobject.IsValid()) {
              result = TEXT("GetNftImageInfoFromUri Failed to parse json");
            } else {
              nftinfo.JsonString = jsonstring;
              jsonobject->TryGetStringField(TEXT("name"), nftinfo.Name);
              jsonobject->TryGetStringField(TEXT("description"),
                                            nftinfo.Description);
              jsonobject->TryGetStringField(TEXT("image"), nftinfo.Image);
              nftinfo.ImageUrl = convert_ipfs_url(nftinfo.Image);
              result = TEXT("");
            }
          } else {
            switch (callbackrequest->GetStatus()) {
            case EHttpRequestStatus::Failed_ConnectionError:
              result = TEXT("Connection failed.");
            default:
              result = TEXT("Request failed.");
            }
          }

          AsyncTask(ENamedThreads::GameThread, [Out, nftinfo, result]() {
            Out.ExecuteIfBound(nftinfo, result);
          });
        });

    httprequest->ProcessRequest();
  });
}

// CAUTION: This function is dangerous. Validate imageurl and check validity
void UPlayCppSdkBPLibrary::GetNftImageFromUrl(FGetNftImageFromUrl Out,
                                              FString imageurl) {
  AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [Out, imageurl]() {
    FHttpModule &httpModule = FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httprequest =
        httpModule.CreateRequest();
    httprequest->SetVerb(TEXT("GET"));
    httprequest->SetHeader(TEXT("User-Agent"), g_user_agent);
    httprequest->SetURL(imageurl);

    httprequest->OnProcessRequestComplete().BindLambda(
        [&](FHttpRequestPtr callbackrequest, FHttpResponsePtr callbackresponse,
            bool connectedSuccessfully) mutable {
          UTexture2D *nftimage = NULL;
          FString result;
          if (connectedSuccessfully) {
            // dangerous. Validate imageurl and check validity
            nftimage = UKismetRenderingLibrary::ImportBufferAsTexture2D(
                NULL, callbackresponse->GetContent());
            if (NULL == nftimage) {
              result = TEXT("Failed to import image.");
            } else {
              result = TEXT("");
            }
          } else {
            switch (callbackrequest->GetStatus()) {
            case EHttpRequestStatus::Failed_ConnectionError:
              result = TEXT("Connection failed.");
            default:
              result = TEXT("Request failed.");
            }
          }

          AsyncTask(ENamedThreads::GameThread, [Out, nftimage, result]() {
            Out.ExecuteIfBound(nftimage, result);
          });
        });

    httprequest->ProcessRequest();
  });
}
