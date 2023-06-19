// Copyright 2022, Cronos Labs. All Rights Reserved

/**
 * wallet-connect apis
 *
 */

#pragma once
#include "Async/Async.h" // clang-diagnostic-error: false positive, can be ignored
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayCppSdkLibrary/Include/extra-cpp-bindings/src/lib.rs.h"
#include "PlayCppSdkLibrary/Include/walletconnectcallback.h"
#include <mutex>
#include <queue>

#include "PlayCppSdkActor.generated.h"

/// wallet connect session state
UENUM(BlueprintType)
enum class EWalletconnectSessionState : uint8 {
    StateInit UMETA(DisplayName = "Init"),
    StateDisconnected UMETA(DisplayName = "Disconnected"),
    StateConnecting UMETA(DisplayName = "Connecting"),
    StateConnected UMETA(DisplayName = "Connected"),
    StateUpdated UMETA(DisplayName = "Updated"),
    StateRestored UMETA(DisplayName = "Restored")
};

UENUM(BlueprintType)
enum class EConnectionType : uint8 {
    URI_STRING UMETA(DisplayName = "Generate uri as String"),
    QR_TEXTURE UMETA(DisplayName = "Generate uri as a QR code 2D Texture"),
    LAUNCH_URL UMETA(DisplayName = "Launch uri with native wallet directly"),
};

/// wallet connect session info
USTRUCT(BlueprintType)
struct FWalletConnectSessionInfo {
    GENERATED_USTRUCT_BODY()
    FWalletConnectSessionInfo()
        : sessionstate(EWalletconnectSessionState::StateInit), connected(false),
          accounts(TArray<FString>{}), chain_id("0") {}

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

DECLARE_DYNAMIC_DELEGATE_TwoParams(FWalletconnectPollingDelegate, FString,
                                   jsonevent, FString, result);

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
    FWalletConnectEnsureSessionResult()
        : addresses(TArray<FWalletConnectAddress>{}), chain_id(0) {}

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

/// wallet connect send tx result
USTRUCT(BlueprintType)
struct FWalletSendTXEip155Result {
    GENERATED_USTRUCT_BODY()

    /// transaction hash, if successful
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
    TArray<uint8> tx_hash;

    /// error message, if successful, ""
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
    FString result;
};

/// sign eip155 tx delegate
DECLARE_DYNAMIC_DELEGATE_OneParam(FWalletconnectSignEip155TransactionDelegate,
                                  FWalletSignTXEip155Result, SigningResult);

/// send eip155 tx delegate
DECLARE_DYNAMIC_DELEGATE_OneParam(FWalletconnectSendEip155TransactionDelegate,
                                  FWalletSendTXEip155Result, TxResult);

/// sign personal delegate
DECLARE_DYNAMIC_DELEGATE_OneParam(FWalletconnectSignPersonalDelegate,
                                  FWalletSignTXEip155Result, SigningResult);

/// send contract transaciton delegate
DECLARE_DYNAMIC_DELEGATE_OneParam(FCronosSendContractTransactionDelegate,
                                  FWalletSendTXEip155Result, TxResult);

/// initialize wallet connect delegate
DECLARE_DYNAMIC_DELEGATE_TwoParams(FInitializeWalletConnectDelegate, bool,
                                   Succeed, FString, message);

/// called when new session is created
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnNewSessionReady,
                                   FWalletConnectEnsureSessionResult,
                                   SessionResult, FString, Result);

/// restore wallet connect delegate
DECLARE_DYNAMIC_DELEGATE_TwoParams(FRestoreClientDelegate, bool, Succeed,
                                   FString, message);

/// called when session is restored
DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnRestoreSessionReady,
                                   FWalletConnectEnsureSessionResult,
                                   SessionResult, FString, Result);

/// wallet connect ensure session delegate
DECLARE_DYNAMIC_DELEGATE_TwoParams(FEnsureSessionDelegate,
                                   FWalletConnectEnsureSessionResult,
                                   SessionResult, FString, Result);

/// called when QR is ready
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnQRReady, UTexture2D *, Texture);

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
};

/// facade for wallet connect
UCLASS()
class CRONOSPLAYUNREAL_API APlayCppSdkActor : public AActor {
    GENERATED_BODY()

  public:
    bool _pollingEvents;
    bool _removeClient;

    ::com::crypto::game_sdk::Walletconnect2Client *_coreClient;

    // Internal session info, it will be updated every time walletconnect status
    // changes
    FWalletConnectSessionInfo _session_info;

    // Internal session result, it will be set after successfully calling
    // `EnsureSession`
    FWalletConnectEnsureSessionResult _session_result;

    /**
     * RestoreClient delegate, called after calling
     * `RestoreClient`
     */
    FRestoreClientDelegate OnRestoreClientDelegate;

    /**
     * InitializeWalletConnect delegate, called after calling
     * `InitializeWalletConnect`
     */
    FInitializeWalletConnectDelegate OnInitializeWalletConnectDelegate;

    /**
     * EnsureSession delegate, called after calling `EnsureSession`
     */
    FEnsureSessionDelegate OnEnsureSessionDelegate;

    FORCEINLINE void InitWalletconnectSessionState() {
        _session_info.sessionstate = EWalletconnectSessionState::StateInit;
        _session_info.connected = false;
        _session_info.chain_id = FString::FromInt(0);
    }

    /**
     * Used for `ConnectWalletConnect`
     */
    FString _relayserver;
    FString _projectid;
    FString _walletnamespace;
    FString _clientmeta;

    EConnectionType _connection_type = EConnectionType::URI_STRING;

  public:
    // Sets default values for this actor's properties
    APlayCppSdkActor();

    ::com::crypto::game_sdk::Walletconnect2Client *GetClient() const {
        return _coreClient;
    };

    void SetWalletConnectEnsureSessionResult(
        FWalletConnectEnsureSessionResult InWalletConnectEnsureSessionResult) {
        _session_result = InWalletConnectEnsureSessionResult;
    }

    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetWalletConnectEnsureSessionResult",
                      Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    FWalletConnectEnsureSessionResult GetWalletConnectEnsureSessionResult() {
        return _session_result;
    }

    const TArray<uint8> GetAddress() const {
        if (_session_result.addresses.Num() > 0) {
            return _session_result.addresses[0].address;
        } else {
            TArray<uint8> IntArray;
            return IntArray;
        }
    }
    const int64 GetChainId() const { return _session_result.chain_id; }

    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetWalletConnectSessionInfo",
                      Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    FWalletConnectSessionInfo GetWalletConnectSessionInfo() {
        return _session_info;
    }

    FWalletConnectSessionInfo SetWalletConnectSessionInfo(
        com::crypto::game_sdk::WalletConnectSessionInfo src,
        EWalletconnectSessionState sessionstate);

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void Destroyed() override;

  public:
    void setCommon(com::crypto::game_sdk::WalletConnectTxCommon &common,
                   FString gaslimit, FString gasprice);

    // Called every frame
    virtual void Tick(float DeltaTime) override;

    /**
     * destroy wallet-connect client
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "DestroyClient", Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void DestroyClient();

    /**
     * Connect wallet client with walletconnect (Only Crypto.com Defi Wallet is
     * suppported at this moment)
     * @param description wallet-connect client description
     * @param url wallet-connect server url
     * @param icon_urls wallet-connect icon urls
     * @param name wallet-connect name
     * @param chain_id the network chain id (if 0, retrived and decided by
     * wallet, if > 0, decided by the client)
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "ConnectWalletConnect",
                      Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void ConnectWalletConnect(

        FString relayserver, FString projectid, FString walletnamespace,
        FString clientmeta,

        EConnectionType connection_type);

    /**
     * intialize wallet-connect client
     * @param description wallet-connect client description
     * @param url wallet-connect server url
     * @param icon_urls wallet-connect icon urls
     * @param name wallet-connect name
     * @param Out InitializeWalletConnect callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "InitializeWalletConnect",
                      Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void InitializeWalletConnect(FString relayserver, FString projectid,
                                 FString walletnamespace, FString clientmeta,

                                 FInitializeWalletConnectDelegate Out);

    UFUNCTION()
    void OnInitializeWalletConnectFinished(bool succeed, FString message);

    /**
     * Create session or restore ession, ensure session
     * @param Out EnsureSession callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "EnsureSession", Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void EnsureSession(FEnsureSessionDelegate Out);

    UFUNCTION()
    void OnNewSession(FWalletConnectEnsureSessionResult SessionResult,
                      FString Result);

    UFUNCTION()
    void OnReceiveWalletconnectPolling(FString jsonevent, FString result);

    /**
     * On NewSessionReady delegate, called if session is created
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
    FOnNewSessionReady OnNewSessionReady;

    UFUNCTION()
    void OnRestoreSession(FWalletConnectEnsureSessionResult SessionResult,
                          FString Result);

    /**
     * On RestoreSessionReady delegate, called if session is restored
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
    FOnRestoreSessionReady OnRestoreSessionReady;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
    FWalletconnectPollingDelegate OnReceiveWalletconnectPollingDelegate;

    /**
     * Enable polling events
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
    bool PollingEventsEnabled;

    /**
     * Include chainid in signing,sending tx
     *
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
    bool EnableEipTx155Tx; // include chainid in tx

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
    int64
        EnsureSessionTimeoutMilliSeconds; // in milli seconds, 1 seconds = 1000

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
    int64 PollingEventsIntervalInMilliseconds; // in milli seconds, 1 seconds =
                                               // 1000

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
    float DestroyClientWaitSeconds; // in seconds

    /**
     * Clear Session
     * @param success whether clearing session succeed or not
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "ClearSession", Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void ClearSession(bool &success);

    /**
     * Begin polling
     * @param sessioninfodelegate   session info delegate
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "BeginPolling", Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    bool BeginPolling(const FWalletconnectPollingDelegate &sessioninfodelegate);

    /**
     * On QR Ready delegate, called after QR is ready
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
    FOnQRReady OnQRReady;

    UFUNCTION()
    void OnWalletconnectSessionInfo(FWalletConnectSessionInfo SessionInfo);

    /**
     * get qr code string
     * @param output qr code string
     * @param success succeed or fail
     * @param output_message  error message
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
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "SaveClient", Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void SaveClient(FString &output, bool &success, FString &output_message);

    /**
     * restore session information from string(json)
     * @param Out RestoreClient callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "RestoreClient", Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void RestoreClient(FRestoreClientDelegate Out);

    UFUNCTION()
    void OnRestoreClientFinished(bool succeed, FString message);

    /**
     * sign general message
     * @param user_message user message to sign
     * @param signature signature byte arrays
     * @param success succeed or fail
     * @param output_message  error message
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "SignPersonal", Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void SignPersonal(FString user_message,
                      FWalletconnectSignPersonalDelegate Out);

    /**
     * sign EIP155 tx
     * @param info EIP 155 tx information
     * @param Out sign legacy tx result callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "SignEip155Transaction",
                      Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void SignEip155Transaction(FWalletConnectTxEip155 info,
                               FWalletconnectSignEip155TransactionDelegate Out);

    /**
     * send EIP155 tx
     * @param info EIP 155 tx information
     * @param Out sign legacy tx result callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "SendEip155Transaction",
                      Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void SendEip155Transaction(FWalletConnectTxEip155 info,
                               FWalletconnectSendEip155TransactionDelegate Out);

    void destroyCoreClient();

    /**
     * Transfers `token_id` token from `from_address` to `to_address`.
     * @param contractAddress erc721 contract
     * @param toAddress to address
     * @param tokenId token id
     * @param gasLimit gas limit
     * @param gasPrice gas price
     * @param Out FCronosSendContractTransactionDelegate callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721TransferFrom",
                      Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void Erc721TransferFrom(FString contractAddress, FString toAddress,
                            FString tokenId, FString gasLimit, FString gasPrice,
                            FCronosSendContractTransactionDelegate Out);
    /**
     * Transfers `token_id` token from `from_address` to `to_address`.
     * @param contractAddress erc721 contract
     * @param toAddress to address
     * @param tokenId token id
     * @param gasLimit gas limit
     * @param gasPrice gas price
     * @param Out FCronosSendContractTransactionDelegate callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721SafeTransferFrom",
                      Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void Erc721SafeTransferFrom(FString contractAddress, FString toAddress,
                                FString tokenId, FString gasLimit,
                                FString gasPrice,
                                FCronosSendContractTransactionDelegate Out);
    /**
     * Transfers `token_id` token from `from_address` to `to_address`.
     * @param contractAddress erc721 contract
     * @param toAddress to address
     * @param tokenId token id
     * @param additionalData additional data
     * @param gasLimit gas limit
     * @param gasPrice gas price
     * @param Out FCronosSendContractTransactionDelegate callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721SafeTransferFromWithAdditionalData",
                      Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void Erc721SafeTransferFromWithAdditionalData(
        FString contractAddress, FString toAddress, FString tokenId,
        TArray<uint8> additionalData, FString gasLimit, FString gasPrice,
        FCronosSendContractTransactionDelegate Out);

    /**
     * Gives permission to `approved_address` to transfer `token_id` token to
     another account.
     * The approval is cleared when the token is transferred. Only a single
     account can be
     * approved at a time, so approving the zero address clears previous
     approvals.
     * @param contractAddress erc721 contract
     * @param approvedAddress  address to approve
     * @param tokenId token id
     * @param gasLimit gas limit
     * @param gasPrice gas price
     * @param Out FCronosSendContractTransactionDelegate callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721Approve", Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void Erc721Approve(FString contractAddress, FString approvedAddress,
                       FString tokenId, FString gasLimit, FString gasPrice,
                       FCronosSendContractTransactionDelegate Out);

    /**
     * Enable or disable approval for a third party `approved_address` to manage
     * all of sender's assets
     * @param contractAddress erc721 contract
     * @param approvedAddress address to approve
     * @param approved approved or not
     * @param gasLimit gas limit
     * @param gasPrice gas price
     * @param Out FCronosSendContractTransactionDelegate callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721SetApprovalForAll",
                      Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void Erc721SetApprovalForAll(FString contractAddress,
                                 FString approvedAddress, bool approved,
                                 FString gasLimit, FString gasPrice,
                                 FCronosSendContractTransactionDelegate Out);

    /**
     * Transfers `amount` tokens of `token_id` from
     * `from_address` to `to_address` with `additional_data`
     * @param contractAddress erc1155 contract
     * @param toAddress to address
     * @param tokenId token id
     * @param amount amount
     * @param additionalData additional data
     * @param gasLimit gas limit
     * @param gasPrice gas price
     * @param Out FCronosSendContractTransactionDelegate callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc1155SafeTransferFrom",
                      Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void Erc1155SafeTransferFrom(FString contractAddress, FString toAddress,
                                 FString tokenId, FString amount,
                                 TArray<uint8> additionalData, FString gasLimit,
                                 FString gasPrice,
                                 FCronosSendContractTransactionDelegate Out);

    /**
     * Enable or disable approval for a third party `approved_address` to manage
     * all of sender's assets
     * @param contractAddress erc1155 contract
     * @param approvedAddress address to approve
     * @param approved approved or not
     * @param gasLimit gas limit
     * @param gasPrice gas price
     * @param Out FCronosSendContractTransactionDelegate callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc1155Approve", Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void Erc1155Approve(FString contractAddress, FString approvedAddress,
                        bool approved, FString gasLimit, FString gasPrice,
                        FCronosSendContractTransactionDelegate Out);

    /**
     * Moves `amount` tokens from the caller’s account to `to_address`.
     * @param contractAddress erc20 contract
     * @param toAddress to address
     * @param amount amount
     * @param gasLimit gas limit
     * @param gasPrice gas price
     * @param Out FCronosSendContractTransactionDelegate callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc20Transfer", Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void Erc20Transfer(FString contractAddress, FString toAddress,
                       FString amount, FString gasLimit, FString gasPrice,
                       FCronosSendContractTransactionDelegate Out);
    /**
     * Moves `amount` tokens from the caller’s account to `to_address`.
     * @param contractAddress erc20 contract
     * @param toAddress to address
     * @param amount amount
     * @param gasLimit gas limit
     * @param gasPrice gas price
     * @param Out FCronosSendContractTransactionDelegate callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc20TransferFrom",
                      Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void Erc20TransferFrom(FString contractAddress, FString toAddress,
                           FString amount, FString gasLimit, FString gasPrice,
                           FCronosSendContractTransactionDelegate Out);

    /**
     * Allows `approved_address` to withdraw from your account multiple times,
     * up to the `amount` amount.
     * @param contractAddress erc20 contract
     * @param approvedAddress address to approve
     * @param amount amount
     * @param gasLimit gas limit
     * @param gasPrice gas price
     * @param Out FCronosSendContractTransactionDelegate callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc20Approve", Keywords = "PlayCppSdk"),
              Category = "PlayCppSdk")
    void Erc20Approve(FString contractAddress, FString approvedAddress,
                      FString amount, FString gasLimit, FString gasPrice,
                      FCronosSendContractTransactionDelegate Out);

    rust::string Erc721TransferFromAction(FString contract_address,
                                          FString from_address,
                                          FString to_address, FString token_id);
    rust::string Erc721SafeTransferFromAction(FString contract_address,
                                              FString from_address,
                                              FString to_address,
                                              FString token_id);

    rust::string Erc721SafeTransferFromWithAdditionalDataAction(
        FString contract_address, FString from_address, FString to_address,
        FString token_id, TArray<uint8> additional_data);

    rust::string Erc1155SafeTransferFromAction(FString contract_address,
                                               FString from_address,
                                               FString to_address,
                                               FString token_id, FString amount,
                                               TArray<uint8> additional_data);

    rust::string Erc20ApprovalAction(FString contract_address,
                                     FString approved_address, FString amount);
    rust::string Erc721ApprovalAction(FString contract_address,
                                      FString approved_address,
                                      FString token_id);
    rust::string Erc721SetApprovalForAllAction(FString contract_address,
                                               FString approved_address,
                                               bool approved);

    rust::string Erc1155ApprovalAction(FString contract_address,
                                       FString approved_address, bool approved);

    rust::string Erc20TransferAction(FString contract_address,
                                     FString to_address, FString amount);

    rust::string Erc20TransferFromAction(FString contract_address,
                                         FString from_address,
                                         FString to_address, FString amount);
};
