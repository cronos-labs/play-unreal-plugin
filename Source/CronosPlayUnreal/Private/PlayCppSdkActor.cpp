// Copyright 2022, Cronos Labs. All Rights Reserved

#include "PlayCppSdkActor.h"

#include "Async/TaskGraphInterfaces.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Kismet/GameplayStatics.h"
#include "PlayCppSdkLibrary/Include/extra-cpp-bindings/src/lib.rs.h"
#include "PlayCppSdkLibrary/Include/rust/cxx.h"
#include <iostream>
#include <memory>

using namespace std;
using namespace rust;
using namespace com::crypto::game_sdk;

::com::crypto::game_sdk::WalletconnectClient *APlayCppSdkActor::_coreClient =
    NULL;
APlayCppSdkActor *APlayCppSdkActor::_sdk = NULL;

class UserWalletConnectCallback : public WalletConnectCallback {
public:
  UserWalletConnectCallback() {}
  virtual ~UserWalletConnectCallback() {}
  void onConnected(const WalletConnectSessionInfo &sessioninfo) const;
  void onDisconnected(const WalletConnectSessionInfo &sessioninfo) const;
  void onConnecting(const WalletConnectSessionInfo &sessioninfo) const;
  void onUpdated(const WalletConnectSessionInfo &sessioninfo) const;
};

APlayCppSdkActor *APlayCppSdkActor::getInstance() { return _sdk; }

// Sets default values
APlayCppSdkActor::APlayCppSdkActor() {
  // Set this actor to call Tick() every frame.  You can turn this off to
  // improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false;
  _coreClient = NULL;
  _sdk = this;
}

// Called when the game starts or when spawned
void APlayCppSdkActor::BeginPlay() { Super::BeginPlay(); }

void convertSession(::com::crypto::game_sdk::WalletConnectSessionInfo src,
                    FWalletConnectSessionInfo &dst) {
  dst.connected = src.connected;
  dst.accounts.Empty();
  for (int i = 0; i < src.accounts.size(); i++) {
    FString account = UTF8_TO_TCHAR(src.accounts[i].c_str());
    dst.accounts.Add(account);
  }
  assert(src.accounts.size() == dst.accounts.Num());
  dst.chain_id = UTF8_TO_TCHAR(src.chain_id.c_str());
  dst.bridge = UTF8_TO_TCHAR(src.bridge.c_str());
  dst.key = UTF8_TO_TCHAR(src.key.c_str());
  dst.client_id = UTF8_TO_TCHAR(src.client_id.c_str());
  dst.client_meta = UTF8_TO_TCHAR(src.client_meta.c_str());
  dst.peer_id = UTF8_TO_TCHAR(src.peer_id.c_str());
  dst.peer_meta = UTF8_TO_TCHAR(src.peer_meta.c_str());
  dst.handshake_topic = UTF8_TO_TCHAR(src.handshake_topic.c_str());
}
// Called every frame
void APlayCppSdkActor::Tick(float DeltaTime) { Super::Tick(DeltaTime); }

void APlayCppSdkActor::DestroyClient() { destroyCoreClient(); }

void APlayCppSdkActor::Destroyed() {
  Super::Destroyed();

  DestroyClient();

  _sdk = NULL;
  assert(NULL == _coreClient);
  assert(NULL == _sdk);
}

void APlayCppSdkActor::InitializeWalletConnect(
    FString description, FString url, TArray<FString> icon_urls, FString name,
    int64 chain_id, FInitializeWalletConnectDelegate Out) {

  AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask,
            [this, Out, description, url, icon_urls, name, chain_id]() {
              FWalletConnectEnsureSessionResult output;
              bool success = false;
              FString message;
              try {

                std::string mydescription = TCHAR_TO_UTF8(*description);
                std::string myurl = TCHAR_TO_UTF8(*url);
                Vec<String> myiconurls;
                for (int i = 0; i < icon_urls.Num(); i++) {
                  myiconurls.push_back(TCHAR_TO_UTF8(*icon_urls[i]));
                }

                std::string myname = TCHAR_TO_UTF8(*name);

                Box<WalletconnectClient> tmpClient = walletconnect_new_client(
                    mydescription, myurl, myiconurls, myname, (uint64)chain_id);
                _coreClient = tmpClient.into_raw();
                assert(_coreClient != NULL);

                success = true;

              } catch (const rust::cxxbridge1::Error &e) {

                success = false;
                message = FString::Printf(
                    TEXT("PlayCppSdk InitializeWalletConnect Error: %s"),
                    UTF8_TO_TCHAR(e.what()));
              }

              AsyncTask(ENamedThreads::GameThread, [Out, success, message]() {
                Out.ExecuteIfBound(success, message);
              });
            });
}

void APlayCppSdkActor::RestoreClient(FString jsondata, bool &success,
                                     FString &output_message) {
  try {
    if (NULL != _coreClient) {
      success = false;
      output_message = TEXT("Client Already Exists");
      return;
    }

    std::string sessioninfostring = TCHAR_TO_UTF8(*jsondata);
    Box<WalletconnectClient> tmpClient =
        walletconnect_restore_client(sessioninfostring);
    _coreClient = tmpClient.into_raw();
    assert(_coreClient != NULL);

    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(TEXT("PlayCppSdk RestoreClient Error: %s"),
                                     UTF8_TO_TCHAR(e.what()));
  }
}

void APlayCppSdkActor::EnsureSession(FEnsureSessionDelegate Out) {

  AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out]() {
    FWalletConnectEnsureSessionResult output;
    FString result;
    try {

      WalletConnectEnsureSessionResult sessionresult;
      if (_coreClient != NULL) {
        sessionresult = _coreClient->ensure_session_blocking();
        output.addresses.Empty();
        for (int i = 0; i < sessionresult.addresses.size(); i++) {
          FWalletConnectAddress newaddress;
          ::std::array<::std::uint8_t, 20> src =
              sessionresult.addresses[i].address;

          newaddress.address.Empty();
          for (int j = 0; j < src.size(); j++) {
            newaddress.address.Add(src[j]);
          }
          assert(20 == newaddress.address.Num());
          output.addresses.Add(newaddress);
        }
        assert(output.addresses.Num() == sessionresult.addresses.size());
        output.chain_id = sessionresult.chain_id;
      } else {
        result = FString::Printf(
            TEXT("PlayCppSdk EnsureSession Error Invalid Client"));
      }

    } catch (const rust::cxxbridge1::Error &e) {
      result = FString::Printf(TEXT("PlayCppSdk EnsureSession Error: %s"),
                               UTF8_TO_TCHAR(e.what()));
    }

    AsyncTask(ENamedThreads::GameThread,
              [Out, output, result]() { Out.ExecuteIfBound(output, result); });
  });
}

void APlayCppSdkActor::SetupCallback(
    const FWalletconnectSessionInfoDelegate &sessioninfodelegate, bool &success,
    FString &output_message) {
  try {
    if (NULL == _coreClient) {
      success = false;
      output_message = TEXT("Invalid Client");
      return;
    }

    // connect dynamic multicast delegate
    OnReceiveWalletconnectSessionInfoDelegate = sessioninfodelegate;

    WalletConnectCallback *usercallbackraw = new UserWalletConnectCallback();
    std::unique_ptr<WalletConnectCallback> usercallback(usercallbackraw);
    _coreClient->setup_callback_blocking(std::move(usercallback));

    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(TEXT("PlayCppSdk EnsureSession Error: %s"),
                                     UTF8_TO_TCHAR(e.what()));
  }
}

void APlayCppSdkActor::GetConnectionString(FString &output, bool &success,
                                           FString &output_message) {
  try {
    if (NULL == _coreClient) {
      success = false;
      output_message = TEXT("Invalid Client");
      return;
    }
    String qrcode = _coreClient->get_connection_string();
    output = UTF8_TO_TCHAR(qrcode.c_str());

    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(TEXT("PlayCppSdk GetQrcode Error: %s"),
                                     UTF8_TO_TCHAR(e.what()));
  }
}

FString APlayCppSdkActor::GetCryptoWalletUrl(FString uri) {
  FString temp = FGenericPlatformHttp::UrlEncode(uri);
  FString platform = UGameplayStatics::GetPlatformName();
  if (platform == TEXT("IOS")) {
    // Use universal link on ios
    FString url =
        FString::Printf(TEXT("https://wallet.crypto.com/wc?uri=%s"), *temp);
    return url;
  } else if (platform == TEXT("Android")) {
    // Android simply needs uri
    return uri;
  } else {
    // Use deep link on other os
    FString url = FString::Printf(TEXT("cryptowallet://wc?uri=%s"), *temp);
    return url;
  }
}

void APlayCppSdkActor::SaveClient(FString &output, bool &success,
                                  FString &output_message) {
  try {
    if (NULL == _coreClient) {
      success = false;
      output_message = TEXT("Invalid Client");
      return;
    }

    String sessioninfo = _coreClient->save_client();
    output = UTF8_TO_TCHAR(sessioninfo.c_str());
    success = true;
  } catch (const rust::cxxbridge1::Error &e) {
    success = false;
    output_message = FString::Printf(TEXT("PlayCppSdk SaveClient Error: %s"),
                                     UTF8_TO_TCHAR(e.what()));
  }
}

void copyTArrayToVec(const TArray<uint8> &src, Vec<uint8_t> &dst) {
  dst.clear();
  for (int i = 0; i < src.Num(); i++) {
    dst.push_back(src[i]);
  }
  assert(dst.size() == src.Num());
}
void copyVecToTArray(const Vec<uint8_t> &src, TArray<uint8> &dst) {
  dst.Empty();
  for (int i = 0; i < src.size(); i++) {
    dst.Add(src[i]);
  }
  assert(dst.Num() == src.size());
}

void APlayCppSdkActor::SignPersonal(FString user_message, TArray<uint8> address,
                                    FWalletconnectSignPersonalDelegate Out) {
  ::com::crypto::game_sdk::WalletconnectClient *coreclient = _coreClient;
  assert(coreclient != NULL);
  AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask,
            [Out, coreclient, user_message, address, this]() {
              FWalletSignTXEip155Result output;
              try {

                assert(20 == address.Num());
                ::std::array<::std::uint8_t, 20> dstaddress;
                for (int i = 0; i < address.Num(); i++) {
                  dstaddress[i] = address[i];
                }
                Vec<uint8_t> sig1 = _coreClient->sign_personal_blocking(
                    TCHAR_TO_UTF8(*user_message), dstaddress);

                copyVecToTArray(sig1, output.signature);
                assert(sig1.size() == output.Num());

              } catch (const rust::cxxbridge1::Error &e) {
                output.result =
                    FString::Printf(TEXT("PlayCppSdk SignPersonal Error: %s"),
                                    UTF8_TO_TCHAR(e.what()));
              }

              AsyncTask(ENamedThreads::GameThread,
                        [Out, output]() { Out.ExecuteIfBound(output); });
            });
}

void APlayCppSdkActor::SignEip155Transaction(
    FWalletConnectTxEip155 info, TArray<uint8> address,
    FWalletconnectSignEip155TransactionDelegate Out) {

  ::com::crypto::game_sdk::WalletconnectClient *coreclient = _coreClient;
  assert(coreclient != NULL);
  AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [Out, coreclient, address,
                                                      info, this]() {
    FWalletSignTXEip155Result output;

    try {

      assert(20 == address.Num());
      ::std::array<::std::uint8_t, 20> dstaddress;
      for (int i = 0; i < address.Num(); i++) {
        dstaddress[i] = address[i];
      }

      WalletConnectTxEip155 myinfo;
      myinfo.to = TCHAR_TO_UTF8(*info.to);
      myinfo.common.gas_limit = TCHAR_TO_UTF8(*info.gas);
      myinfo.common.gas_price = TCHAR_TO_UTF8(*info.gas_price);
      myinfo.value = TCHAR_TO_UTF8(*info.value);
      copyTArrayToVec(info.data, myinfo.data);
      myinfo.common.nonce = TCHAR_TO_UTF8(*info.nonce);
      myinfo.common.chainid = (uint64)info.chain_id;
      if (_coreClient != NULL) {

        Vec<uint8_t> sig1 =
            _coreClient->sign_eip155_transaction_blocking(myinfo, dstaddress);

        copyVecToTArray(sig1, output.signature);
        assert(sig1.size() == output.Num());
      } else {
        output.result = FString::Printf(
            TEXT("PlayCppSdk SignEip155Transaction Invalid Client"));
      }

    } catch (const rust::cxxbridge1::Error &e) {
      output.result =
          FString::Printf(TEXT("PlayCppSdk SignEip155Transaction Error: %s"),
                          UTF8_TO_TCHAR(e.what()));
    }

    AsyncTask(ENamedThreads::GameThread,
              [Out, output]() { Out.ExecuteIfBound(output); });
  });
}

void APlayCppSdkActor::sendEvent(const FWalletConnectSessionInfo &info) {

  AsyncTask(ENamedThreads::GameThread, [this, info]() {
    this->OnReceiveWalletconnectSessionInfoDelegate.ExecuteIfBound(info);
  });
}

void UserWalletConnectCallback::onConnected(
    const WalletConnectSessionInfo &sessioninfo) const {
  UE_LOG(LogTemp, Log, TEXT("user c++ onConnected"));
  if (APlayCppSdkActor::getInstance()) {
    FWalletConnectSessionInfo output;
    convertSession(sessioninfo, output);
    output.sessionstate = EWalletconnectSessionState::StateConnected;
    APlayCppSdkActor::getInstance()->sendEvent(output);
  }
}
void UserWalletConnectCallback::onDisconnected(
    const WalletConnectSessionInfo &sessioninfo) const {
  UE_LOG(LogTemp, Log, TEXT("user c++ onDisconnected"));
  if (APlayCppSdkActor::getInstance()) {
    FWalletConnectSessionInfo output;
    convertSession(sessioninfo, output);
    output.sessionstate = EWalletconnectSessionState::StateDisconnected;

    APlayCppSdkActor::getInstance()->sendEvent(output);
  }
}
void UserWalletConnectCallback::onConnecting(
    const WalletConnectSessionInfo &sessioninfo) const {
  UE_LOG(LogTemp, Log, TEXT("user c++ onConnecting"));
  if (APlayCppSdkActor::getInstance()) {
    FWalletConnectSessionInfo output;
    convertSession(sessioninfo, output);
    output.sessionstate = EWalletconnectSessionState::StateConnecting;

    APlayCppSdkActor::getInstance()->sendEvent(output);
  }
}
void UserWalletConnectCallback::onUpdated(
    const WalletConnectSessionInfo &sessioninfo) const {

  UE_LOG(LogTemp, Log, TEXT("user c++ onUpdated"));
  if (APlayCppSdkActor::getInstance()) {
    FWalletConnectSessionInfo output;
    convertSession(sessioninfo, output);
    output.sessionstate = EWalletconnectSessionState::StateUpdated;

    APlayCppSdkActor::getInstance()->sendEvent(output);
  }
}

void APlayCppSdkActor::destroyCoreClient() {
  if (_coreClient != NULL) {
    UE_LOG(LogTemp, Log, TEXT("PlayCppSdkActor destroyCoreClient"));

    // restored back, close tokio-runtime
    // ue4 editor gracefully closed
    Box<WalletconnectClient> tmpwallet =
        Box<WalletconnectClient>::from_raw(_coreClient);
    _coreClient = NULL;
  }
}

void StopWalletConnect() { APlayCppSdkActor::destroyCoreClient(); }
