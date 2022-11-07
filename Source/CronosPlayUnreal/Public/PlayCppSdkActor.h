// Copyright 2022, Cronos Labs. All Rights Reserved

/**
 * wallet-connect apis
 * how callback works for c++ events
  dynamic delegate is used for blueprint callback.
  (delegate, multicast delegate, event is for c++ only)
  when c++ callback is called, it calls sendEvent to trigger the delegate.
  ue4 async task is used for calling delegation.

  DECLARE_DYNAMIC_DELEGATE_OneParam(FWalletconnectSessionInfoDelegate, <-
  delegation name FWalletConnectSessionInfo, <- 1st parameter type SessionInfo);
  <- 1st parameter name

 */

#pragma once

#include "Async/Async.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayCppSdkLibrary/Include/extra-cpp-bindings/src/lib.rs.h"
#include "PlayCppSdkLibrary/Include/walletconnectcallback.h"

#include <mutex>
#include <queue>

#include "PlayCppSdkActor.generated.h"

/**
 Cronos Signed Transaction
 */
USTRUCT(BlueprintType)
struct FCronosSignedTransactionRaw {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
  TArray<uint8> SignedTx;
};

// build signed-tx
DECLARE_DYNAMIC_DELEGATE_TwoParams(FCronosSignedTransactionDelegate,
                                   FCronosSignedTransactionRaw, TxResult,
                                   FString, Result);

/// wallet connect session state
UENUM(BlueprintType)
enum class EWalletconnectSessionState : uint8 {
  StateDisconnected UMETA(DisplayName = "Disconnected"),
  StateConnecting UMETA(DisplayName = "Connecting"),
  StateConnected UMETA(DisplayName = "Connected"),
  StateUpdated UMETA(DisplayName = "Updated")
};

/// wallet connect session info
USTRUCT(BlueprintType)
struct FWalletConnectSessionInfo {
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
/// wallet connect callback
DECLARE_DYNAMIC_DELEGATE_OneParam(FWalletconnectSessionInfoDelegate,
                                  FWalletConnectSessionInfo, SessionInfo);

/// wallet connect address, 20 bytes
USTRUCT(BlueprintType)
struct FWalletConnectAddress {
  GENERATED_USTRUCT_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  TArray<uint8> address;
};

/// wallet connect session information
USTRUCT(BlueprintType)
struct FWalletConnectEnsureSessionResult {
  GENERATED_USTRUCT_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  TArray<FWalletConnectAddress> addresses;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  int64 chain_id;
};

/// wallet connect sign tx result
USTRUCT(BlueprintType)
struct FWalletSignTXEip155Result {
  GENERATED_USTRUCT_BODY()

  /// signature, 65 bytes, if successful
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  TArray<uint8> signature;

  /// error message, if successful, ""
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString result;
};

/// sign eip155 tx callback
DECLARE_DYNAMIC_DELEGATE_OneParam(FWalletconnectSignEip155TransactionDelegate,
                                  FWalletSignTXEip155Result, SigningResult);

/// sign personal callback
DECLARE_DYNAMIC_DELEGATE_OneParam(FWalletconnectSignPersonalDelegate,
                                  FWalletSignTXEip155Result, SigningResult);

/// initialize wallet connect callback
DECLARE_DYNAMIC_DELEGATE_TwoParams(FInitializeWalletConnectDelegate, bool,
                                   Succeed, FString, message);

/// wallet connect ensure sssion callback
DECLARE_DYNAMIC_DELEGATE_TwoParams(FEnsureSessionDelegate,
                                   FWalletConnectEnsureSessionResult,
                                   SessionResult, FString, Result);

/// wallet connect eip155 tx information
USTRUCT(BlueprintType)
struct FWalletConnectTxEip155 {
  GENERATED_USTRUCT_BODY()
  /** hexstring, "0x..." */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString to;
  /** gas limit in decimal string */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString gas;
  /** gas price in decimal string */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString gas_price;
  /** decimal string, in wei units */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString value;
  /** data, as bytes */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  TArray<uint8> data;
  /** nonce in decimal string */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString nonce;
  /** chain_id */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  int64 chain_id;
};

/// facade for wallet connect
UCLASS()
class CRONOSPLAYUNREAL_API APlayCppSdkActor : public AActor {
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
  /**
   *  Cronos rpc address
   *  for example , devnet:  http://127.0.0.1:8545
   * testnet: https://evm-dev-t3.cronos.org
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
  FString myCronosRpc;

  /**
   * Cronos chain-id
   * for example, devnet: 777
   * testnet: 338
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
  int32 myCronosChainID;

  // Called every frame
  virtual void Tick(float DeltaTime) override;

  /**
   * destroy wallet-connect client
   *
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "DestroyClient", Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void DestroyClient();

  /**
   * intialize wallet-connect client
   * @param description wallet-connect client description
   * @param url wallet-connect server url
   * @param icon_urls wallet-connect icon urls
   * @param name wallet-connect name
   * @param Out InitializeWalletConnect callback
   *
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "InitializeWalletConnect",
                    Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void InitializeWalletConnect(FString description, FString url,
                               TArray<FString> icon_urls, FString name,
                               int64 chain_id,
                               FInitializeWalletConnectDelegate Out);

  /**
   * create session or restore ession, ensure session
   * @param Out EnsureSession callback
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "EnsureSession", Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void EnsureSession(FEnsureSessionDelegate Out);

  /**
   * setup callback to receive event
   * @param sessioninfodelegate callback to receive session info
   * @param success succeed or fail
   * @output_message  error message
   *
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SetupCallback", Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void
  SetupCallback(const FWalletconnectSessionInfoDelegate &sessioninfodelegate,
                bool &success, FString &output_message);

  /**
   * get qr code string
   * @param output qr code string
   * @param success succeed or fail
   * @param output_message  error message
   *
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetConnectionString",
                    Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void GetConnectionString(FString &output, bool &success,
                           FString &output_message);

  /**
   * get crypto wallet url
   * @param uri WalletConnect uri
   * @return url starts with cryptowallet://
   *
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetCryptoWalletUrl",
                    Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  FString GetCryptoWalletUrl(FString uri);
  /**
   * save session information as string
   * @param output session information string
   * @param success succeed or fail
   * @param output_message  error message
   *
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SaveClient", Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void SaveClient(FString &output, bool &success, FString &output_message);

  /**
   * restore session information from string(json)
   * @param jsondata session information string(json)
   * @param success   succeed or fail
   * @param output_message  error message
   *
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "RestoreClient", Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void RestoreClient(FString jsondata, bool &success, FString &output_message);

  /**
   * sign general message
   * @param user_message user message to sign
   * @param address address to sign
   * @param signature signature byte arrays
   * @param success succeed or fail
   * @param output_message  error message
   *
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SignPersonal", Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void SignPersonal(FString user_message, TArray<uint8> address,
                    FWalletconnectSignPersonalDelegate Out);

  /**
   * sign EIP155 tx
   * @param info EIP 155 tx information
   * @param address address to sign
   * @param Out sign legacy tx result callback
   *
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SignEip155Transaction",
                    Keywords = "PlayCppSdk"),
            Category = "PlayCppSdk")
  void SignEip155Transaction(FWalletConnectTxEip155 info, TArray<uint8> address,
                             FWalletconnectSignEip155TransactionDelegate Out);

  /**
   * WalletConnect Session Information callback
   *
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FWalletconnectSessionInfoDelegate OnReceiveWalletconnectSessionInfoDelegate;

  /**
   * send wallet-connect information to unreal game thread
   *
   */
  void sendEvent(const FWalletConnectSessionInfo &);

  static void destroyCoreClient();

  /**
   * Safely transfers `token_id` token from `from_address` to `to_address`.
   * @param contractAddress erc721 contract
   * @param fromAddress from address to move
   * @param toAddress to address
   * @param tokenid token id
   * @param gasLimit gas limit
   * @param gasPrice gas price
   * @param Out Erc721SafeTransferFrom callback
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc721SafeTransferFrom",
                    Keywords = "Wallet"),
            Category = "CronosPlayUnreal")
  void Erc721SafeTransferFrom(FString contractAddress, FString fromAddress,
                              FString toAddress, FString tokenid,
                              FString gasLimit, FString gasPrice,
                              FCronosSignedTransactionDelegate Out);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "WalletConnectHelloWorld",
                    Keywords = "Wallet"),
            Category = "CronosPlayUnreal")
  static void WalletConnectHelloWorld(FString message, FString &output);
};
