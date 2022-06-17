/** how callback works for c++ events
  dynamic delegate is used for blueprint callback.
  (delegate, multicast delegate, event is for c++ only)
  when c++ callback is called, it calls sendEvent to trigger the delegate.
  ue4 async task is used for calling delegation.

  DECLARE_DYNAMIC_DELEGATE_OneParam(FWalletconnectSessionInfoDelegate, <-
  delegation name FWalletConnectSessionInfo, <- 1st parameter type SessionInfo);
  <- 1st parameter name

 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "walletcore/include/extra-cpp-bindings/src/lib.rs.h"
#include "walletcore/include/walletconnectcallback.h"

#include <mutex>
#include <queue>

#include "PlayCppSdkActor.generated.h"

UENUM(BlueprintType)
enum class EWalletconnectSessionState : uint8
{
  StateDisconnected UMETA(DisplayName = "Disconnected"),
  StateConnecting UMETA(DisplayName = "Connecting"),
  StateConnected UMETA(DisplayName = "Connected"),
  StateUpdated UMETA(DisplayName = "Updated")
};

USTRUCT(BlueprintType)
struct FWalletConnectSessionInfo
{
  GENERATED_BODY()

  /// state
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  EWalletconnectSessionState sessionstate;

  /// if the wallet approved the connection
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  bool connected;
  /// hex-string(0x...), the accounts returned by the wallet
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  TArray<FString> accounts;
  /// u64, the chain id returned by the wallet
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString chain_id;
  /// the bridge server URL
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString bridge;
  /// the secret key used in encrypting wallet requests
  /// and decrypting wallet responses as per WalletConnect 1.0
  /// hex-string(0x...), 32 bytes
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString key;
  /// this is the client's randomly generated ID
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString client_id;
  /// json, the client metadata (that will be presented to the wallet in the
  /// initial request)
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString client_meta;
  /// uuid, the wallet's ID
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString peer_id;
  /// json, the wallet's metadata
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString peer_meta;
  /// uuid, the one-time request ID
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString handshake_topic;
};
DECLARE_DYNAMIC_DELEGATE_OneParam(FWalletconnectSessionInfoDelegate,
                                  FWalletConnectSessionInfo, SessionInfo);

USTRUCT(BlueprintType)
struct FWalletConnectAddress
{
  GENERATED_USTRUCT_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  TArray<uint8> address;
};

USTRUCT(BlueprintType)
struct FWalletConnectEnsureSessionResult
{
  GENERATED_USTRUCT_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  TArray<FWalletConnectAddress> addresses;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  int64 chain_id;
};

USTRUCT(BlueprintType)
struct FWalletSignTXLegayResult
{
  GENERATED_USTRUCT_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  TArray<uint8> signature;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString result;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FWalletconnectSignLegacyTransactionDelegate,
                                  FWalletSignTXLegayResult, SigningResult);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FInitializeWalletBlockingDelegate, bool,
                                   Succeed,           // 1st parameter
                                   FString, message); // 2nd parameter

DECLARE_DYNAMIC_DELEGATE_TwoParams(FEnsureSessionBlockingDelegate,
                                   FWalletConnectEnsureSessionResult,
                                   SessionResult,    // 1st parameter
                                   FString, Result); // 2nd parameter

USTRUCT(BlueprintType)
struct FWalletConnectTxLegacy
{
  GENERATED_USTRUCT_BODY()
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString to;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString gas;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString gas_price;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString value;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  TArray<uint8> data;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString nonce;
};

UCLASS()
class DEFIWALLETCORE_API APlayCppSdkActor : public AActor
{
  GENERATED_BODY()

private:
  // for ue4 async
  static ::com::crypto::game_sdk::WalletconnectClient *_coreClient;
  static APlayCppSdkActor *_sdk;

public:
  static APlayCppSdkActor *getInstance();

  // Sets default values for this actor's properties
  APlayCppSdkActor();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
  virtual void Destroyed() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "DestroyClient", Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void DestroyClient();

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "InitializeWalletConnect",
                    Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void InitializeWalletConnect(FString description, FString url,
                               TArray<FString> icon_urls, FString name,
                               FInitializeWalletBlockingDelegate Out);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "EnsureSessionBlocking",
                    Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void EnsureSessionBlocking(FEnsureSessionBlockingDelegate Out);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SetupCallback", Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void
  SetupCallback(const FWalletconnectSessionInfoDelegate &sessioninfodelegate,
                bool &success, FString &output_message);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetConnectionString",
                    Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void GetConnectionString(FString &output, bool &success,
                           FString &output_message);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SaveClient", Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void SaveClient(FString &output, bool &success, FString &output_message);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "RestoreClient", Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void RestoreClient(FString jsondata, bool &success, FString &output_message);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SignPersonalBlocking",
                    Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void SignPersonalBlocking(FString usermessage, TArray<uint8> address,
                            TArray<uint8> &output, bool &success,
                            FString &output_message);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SignLegacyTransactionBlocking",
                    Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void SignLegacyTransactionBlocking(
      FWalletConnectTxLegacy info, TArray<uint8> address,
      FWalletconnectSignLegacyTransactionDelegate Out);

  UFUNCTION(BlueprintImplementableEvent,
            meta = (DisplayName = "OnReceiveWalletconnectSessionInfo",
                    Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void OnReceiveWalletconnectSessionInfo(int state,
                                         const FWalletConnectSessionInfo &Data);

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FWalletconnectSessionInfoDelegate OnReceiveWalletconnectSessionInfoDelegate;

  void sendEvent(const FWalletConnectSessionInfo &);
};
