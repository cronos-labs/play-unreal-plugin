// Copyright 2022, Cronos Labs. All Rights Reserved

/**
 * basic wallet
 * cosmos NFT
 *
 */
#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/contract.rs.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/lib.rs.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/nft.rs.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/ethereum.rs.h"
#include "DynamicContractObject.h"
#include "DefiWalletCoreActor.generated.h"

/*
TODO:
for erc20,erc721,erc1155
add opaque pointer for each api, and don't recreate in very call
add like apis
createErc20();
createERc721();
createErc1155();
destroyErc20();
destroyErc721();
destroyErc1155();
*/

// callback
// eth
DECLARE_DYNAMIC_DELEGATE_TwoParams(FSendEthTransferDelegate,
                                   FCronosTransactionReceiptRaw, TxResult,
                                   FString, Result);

/// callback of tx broadcast
DECLARE_DYNAMIC_DELEGATE_TwoParams(FWalletBroadcastDelegate, FString, TXHash,
                                   FString, Result);

// erc 1155
DECLARE_DYNAMIC_DELEGATE_TwoParams(FErc1155TransferFromDelegate,
                                   FCronosTransactionReceiptRaw, TxResult,
                                   FString, Result);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FErc1155ApproveDelegate,
                                   FCronosTransactionReceiptRaw, TxResult,
                                   FString, Result);

// erc 721
DECLARE_DYNAMIC_DELEGATE_TwoParams(FErc721TransferFromDelegate,
                                   FCronosTransactionReceiptRaw, TxResult,
                                   FString, Result);
DECLARE_DYNAMIC_DELEGATE_TwoParams(FErc721ApproveDelegate,
                                   FCronosTransactionReceiptRaw, TxResult,
                                   FString, Result);

// erc 20
DECLARE_DYNAMIC_DELEGATE_TwoParams(FErc20TransferFromDelegate,
                                   FCronosTransactionReceiptRaw, TxResult,
                                   FString, Result);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FErc20ApproveDelegate,
                                   FCronosTransactionReceiptRaw, TxResult,
                                   FString, Result);

/**
 Cosmos NFT Denom
 */
USTRUCT(BlueprintType)
struct FCosmosNFTDenom {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString Schema;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString Creator;
};

/**
 * Cosmos NFT Token
 */
USTRUCT(BlueprintType)
struct FCosmosNFTToken {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString URI;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString Data;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString Owner;
};

/**
 * Cosmos NFT Collection
 */
USTRUCT(BlueprintType)
struct FCosmosNFTCollection {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    bool DenomOption;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FCosmosNFTDenom DenomValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    TArray<FCosmosNFTToken> NFTs;
};

/**
 * Cosmos ID Collection
 */
USTRUCT(BlueprintType)
struct FCosmosNFTIDCollection {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString DenomID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    TArray<FString> TokenIDs;
};

/// wallet connect session state
UENUM(BlueprintType)
enum class EMnemonicsWordCount : uint8 {
    Twelve UMETA(DisplayName = "12 mnemonics"),
    Eighteen UMETA(DisplayName = "18 mnemonics"),
    TwentyFour UMETA(DisplayName = "24 mnemonics"),
};

/**
 * Cosmos NFT Owner
 */
USTRUCT(BlueprintType)
struct FCosmosNFTOwner {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString Address;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    TArray<FCosmosNFTIDCollection> IDCollections;
};

UCLASS()
class CRONOSPLAYUNREAL_API ADefiWalletCoreActor : public AActor {
    GENERATED_BODY()

  private:
    /**
     Opaque pointer to store wallet
     */
    org::defi_wallet_core::Wallet *_coreWallet;

  public:
    org::defi_wallet_core::Wallet *getCoreWallet();

    /**
     * Restore wallet with mnemonics and password (Only for testing &
     * development purpose).
     * @param mnemonics mnemonics to restore
     * @param password salt in mnemonics restoration
     * @param output generated address (index=0)
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "RestoreWallet", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void RestoreWallet(FString mnemonics, FString password, FString &output,
                       bool &success, FString &output_message);

    /**
     * Restore wallet with mnemonics and password
     * and save to secure storage
     * @param mnemonics mnemonics to restore
     * @param password salt in mnemonics restoration
     * @param servicename service name for secure storage
     * @param username username for secure storage
     * @param output generated address (index=0)
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "RestoreWalletSaveToSecureStorage",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void RestoreWalletSaveToSecureStorage(FString mnemonics, FString password,
                                          FString servicename, FString username,
                                          FString &output, bool &success,
                                          FString &output_message);

    /**
     * Restore wallet from secure storage
     * @param servicename service name for secure storage
     * @param username username for secure storage
     * @param output generated address (index=0)
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "RestoreWalletLoadFromSecureStorage",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void RestoreWalletLoadFromSecureStorage(FString servicename,
                                            FString username, FString &output,
                                            bool &success,
                                            FString &output_message);

    /**
     * Create a new wallet with password and wordcount (Only for testing &
     * development purpose).
     * @param password salt in mnemonics restoration
     * @param wordcount mnemonics word count (12, 18, 24)
     * @param output generated address (index=0)
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "InitializeNewWallet", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void InitializeNewWallet(FString password, EMnemonicsWordCount wordcount,
                             FString &output, bool &success,
                             FString &output_message);

    /**
     * Get backup mnemonic phrase (Only for testing &
     * development purpose).
     * @param output backup mnemonics
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetBackupMnemonicPhrase",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GetBackupMnemonicPhrase(FString &output, bool &success,
                                 FString &output_message);

    /**
     * Generate mnemonics (Only for testing &
     * development purpose).
     * @param password salt in mnemonics restoration
     * @param wordcount mnemonics word count (12, 18, 24)
     * @param output generated mnemonics
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GenerateMnemonics", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GenerateMnemonics(FString password, EMnemonicsWordCount wordcount,
                           FString &output, bool &success,
                           FString &output_message);

    /**
     * Cosmos send amount.
     * @param walletIndex wallet index which starts from 0
     * @param fromaddress sender address
     * @param toaddress receiver address
     * @param amount amount to send
     * @param amountdenom   amount denom to send
     * @param output transaction hash
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "SendAmount", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void SendAmount(int32 walletIndex, FString fromaddress, FString toaddress,
                    int64 amount, FString amountdenom, FString &output,
                    bool &success, FString &output_message);

    /**
     * Cosmos get address with specified index.
     * @param index  wallet index which starts from 0
     * @param output address string
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetAddress", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GetAddress(int32 index, FString &output, bool &success,
                    FString &output_message);

    /**
     * Cosmos get balance.
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetBalance", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GetBalance(FString address, FString denom, FString &output,
                    bool &success, FString &output_message);

    /**
     * Cosmos get nft supply.
     * @param denomid denom id
     * @param nftowner nft owner
     * @param output nft supply
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetNFTSupply", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GetNFTSupply(FString denomid, FString nftowner, int64 &output,
                      bool &success, FString &output_message);

    /**
     * Cosmos get nft owner.
     * @param denomid denom id
     * @param nftowner nft owner
     * @param output cosmos nft owner
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetNFTOwner", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GetNFTOwner(FString denomid, FString nftowner, FCosmosNFTOwner &output,
                     bool &success, FString &output_message);

    /**
     * Cosmos get nft collection.
     * @param denomid denom id
     * @param output cosmos nft collection
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetNFTCollection", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GetNFTCollection(FString denomid, FCosmosNFTCollection &output,
                          bool &success, FString &output_message);

    /**
     * Cosmos get nft denom.
     * @param denomid denom id
     * @param output cosmos nft denom
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetNFTDenom", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GetNFTDenom(FString denomid, FCosmosNFTDenom &output, bool &success,
                     FString &output_message);

    /**
     * Cosmos get nft denom by name
     * @param denomname denom name
     * @param output cosmos nft denom
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetNFTDenomByName", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GetNFTDenomByName(FString denomname, FCosmosNFTDenom &output,
                           bool &success, FString &output_message);

    /**
     * Get all nft denoms
     * @param output cosmos nft denom
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetNFTAllDenoms", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GetNFTAllDenoms(TArray<FCosmosNFTDenom> &output, bool &success,
                         FString &output_message);

    /**
     * Get nft token
     * @param denomid denom id
     * @param tokenid token id
     * @param cosmos nft token
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetNFTToken", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GetNFTToken(FString denomid, FString tokenid, FCosmosNFTToken &output,
                     bool &success, FString &output_message);

    /**
     * Get eth address with index
     * @param index wallet index which starts from 0
     * @param output get eth address
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetEthAddress", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GetEthAddress(int32 index, FString &output, bool &success,
                       FString &output_message);

    /**
     * Get eth balance
     * @param address eth address
     * @param output get balance
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetEthBalance", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void GetEthBalance(FString address, FString &output, bool &success,
                       FString &output_message);

    /**
     * Broadcast signed eth tx
     * @param Out  event delegate which is triggered after tx is broadcasted
     * @param signedtx signed tx as bytes
     * @param rpc cronos rpc server url
     */
    UFUNCTION(BlueprintCallable, Category = "CronosPlayUnreal",
              meta = (DisplayName = "BroadcastEthTxAsync", Keywords = "Wallet"))
    static void BroadcastEthTxAsync(FWalletBroadcastDelegate Out,
                                    TArray<uint8> signedtx, FString rpc);

    /**
     * Sign eth amount
     * @param walletIndex wallet index which starts from 0
     * @param fromaddress sender address
     * @param toaddress receiver address
     * @param amount amount in eth decimal, eg. 0.1 means 0.1 eth
     * @param gasLimit gas limit, fee= gasLimit * gasPrice
     * @param gasPrice gas price in wei, eg. 1wei= 1/(10^18)eth
     * 1wei=1/(10^9)gwei
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     * @return signed transaction as bytes
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "SignEthAmount", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    TArray<uint8> SignEthAmount(int32 walletIndex, FString fromaddress,
                                FString toaddress, FString amount,
                                FString gasLimit, FString gasPrice,
                                TArray<uint8> txdata, bool &success,
                                FString &output_message);

    /**
     * Send eth amount
     * @param walletIndex wallet index which starts from 0
     * @param fromaddress sender address
     * @param toaddress receiver address
     * @param amountInEthDecimal amount in eth decimal, eg. 0.1 means 0.1 eth
     * @param gasLimit gas limit, fee= gasLimit * gasPrice
     * @param gasPriceInWei gas price in wei, eg. 1wei= 1/(10^18)eth
     * 1wei=1/(10^9)gwei
     * @param Out SendEthAmount callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "SendEthAmount", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void SendEthAmount(int32 walletIndex, FString fromaddress,
                       FString toaddress, FString amountInEthDecimal,
                       FString gasLimit, FString gasPriceInWei,
                       TArray<uint8> txdata, FSendEthTransferDelegate Out);

    /**
     * Sign eth login
     * @param walletIndex wallet index which starts from 0
     * @param document document to sign
     * @param signature get signature
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "SignLogin", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void SignLogin(int32 walletIndex, FString document,
                   TArray<uint8> &signature, bool &success,
                   FString &output_message);

    /**
     * Verify eth login
     * @param document: document to verify
     * @param signature signature to verify
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "VerifyLogin", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void VerifyLogin(FString document, TArray<uint8> signature, bool &success,
                     FString &output_message);

    /**
     * Get erc-20 balance
     * @param contractAddress erc20 contract address
     * @param accountAddress account address to fetch balance
     * @param balance get balance of account address
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc20Balance", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc20Balance(FString contractAddress, FString accountAddress,
                      FString &balance, bool &success, FString &output_message);

    /**
     * Get erc-721 balance, minted token total count of this address
     * @param contractAddress erc721 contract address
     * @param accountAddress account address to fetch balance
     * @param balance to get balance of this address
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721Balance", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721Balance(FString contractAddress, FString accountAddress,
                       FString &balance, bool &success,
                       FString &output_message);

    /**
     * Get erc-1155 balance
     * @param contractAddress erc1155 contract address
     * @param accountAddress account address to fetch balance
     * @param tokenID toiken id to fetch balance
     * @param balance to get balance
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc1155Balance", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc1155Balance(FString contractAddress, FString accountAddress,
                        FString tokenID, FString &balance, bool &success,
                        FString &output_message);

    /**
     * Get erc-1155 balance of batch
     * @param contractAddress erc1155 contract address
     * @param accountAddresses account addresses to fetch balance
     * @param tokenIDs toiken ids to fetch balance
     * @param balanceofbatch balances
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc1155BalanceOfBatch",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc1155BalanceOfBatch(FString contractAddress,
                               TArray<FString> accountAddresses,
                               TArray<FString> tokenIDs,
                               TArray<FString> &balanceofbatch, bool &success,
                               FString &output_message);

    /**
     * Get erc-20 name
     * @param contractAddress erc20 contract address
     * @param name get name
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc20Name", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc20Name(FString contractAddress, FString &name, bool &success,
                   FString &output_message);

    /**
     * Get erc-20 symbol
     * @param contractAddress erc20 contract address
     * @param symbol get symbol
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc20Symbol", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc20Symbol(FString contractAddress, FString &symbol, bool &success,
                     FString &output_message);

    /**
     * Get erc-20 decimals
     * @param contractAddress erc20 contract address
     * @param decimals get decimals
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc20Decimals", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc20Decimals(FString contractAddress, int32 &decimals, bool &success,
                       FString &output_message);

    /**
     * Get erc-20 total supply
     * @param contractAddress erc20 contract address
     * @param totalSupply get total supply
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc20TotalSupply", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc20TotalSupply(FString contractAddress, FString &totalSupply,
                          bool &success, FString &output_message);

    /**
     * Get erc-721 name
     * @param contractAddress erc721 contract address
     * @param name get name
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721Name", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721Name(FString contractAddress, FString &name, bool &success,
                    FString &output_message);

    /**
     * Get erc-721 symbol
     * @param contractAddress contract address
     * @param symbol get symbol
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721Symbol", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721Symbol(FString contractAddress, FString &symbol, bool &success,
                      FString &output_message);

    /**
     * Get erc-721 uri
     * @param contractAddress erc721 contract address
     * @param tokenID token id
     * @param uri  get uri
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721Uri", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721Uri(FString contractAddress, FString tokenID, FString &uri,
                   bool &success, FString &output_message);

    /**
     * Get erc-721 Approved
     * @param contractAddress erc721 contract address
     * @param tokenID token id
     * @param result approved
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721GetApproved", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721GetApproved(FString contractAddress, FString tokenID,
                           FString &result, bool &success,
                           FString &output_message);

    /**
     * Get erc-721 IsApprovedForAll
     * @param contractAddress erc721 contract address
     * @param erc721owner owner address
     * @param erc721approvedaddress  approved address
     * @param result is approved for all
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721IsApprovedForAll",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721IsApprovedForAll(FString contractAddress, FString erc721owner,
                                FString erc721approvedaddress, bool &result,
                                bool &success, FString &output_message);

    /**
     * Get erc-721 owner
     * @param contractAddress erc 721 contract address
     * @param tokenID token id
     * @param ercowner get owner
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721Owner", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721Owner(FString contractAddress, FString tokenID,
                     FString &ercowner, bool &success, FString &output_message);

    /**
     * Get erc-721 total suppy
     * @param contractAddress erc 721 contract address
     * @param totalsupply total suppy
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721TotalSupply", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721TotalSupply(FString contractAddress, FString &totalsupply,
                           bool &success, FString &output_message);

    /**
     *  Returns a token ID at a given index of all the tokens stored by the
     * contract. Use along with totalSupply to enumerate all tokens.
     * @param contractAddress erc 721 contract address
     * @param erc721index which index
     * @param token a token ID at a given index
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721TokenByIndex", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721TokenByIndex(FString contractAddress, FString erc721index,
                            FString &token, bool &success,
                            FString &output_message);

    /**
     * Returns a token ID owned by owner at a given index of its token list. Use
     * along with balanceOf to enumerate all of owner's tokens.
     * @param contractAddress erc 721 contract address
     * @param erc721owner owner
     * @param erc721index which index
     * @param token a token ID at a given index
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721TokenOwnerByIndex",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721TokenOwnerByIndex(FString contractAddress, FString erc721owner,
                                 FString erc721index, FString &token,
                                 bool &success, FString &output_message);

    /**
     * Get erc-1155 uri
     * @param contractAddress erc1155 contract address
     * @param tokenID token ID
     * @param uri  get uri
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc1155Uri", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc1155Uri(FString contractAddress, FString tokenID, FString &uri,
                    bool &success, FString &output_message);

    /**
     * Get erc-1155 IsApprovedForAll
     * @param contractAddress erc1155 contract address
     * @param erc1155owner owner address
     * @param erc1155approvedaddress  approved address
     * @param result is approved for all
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc1155IsApprovedForAll",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc1155IsApprovedForAll(FString contractAddress, FString erc1155owner,
                                 FString erc1155approvedaddress, bool &result,
                                 bool &success, FString &output_message);

    /**
     * erc20 Moves `amount` tokens from the caller’s account to `to_address`.
     * @param contractAddress erc20 contract
     * @param walletindex wallet index which starts from 0
     * @param toAddress to address
     * @param amount amount
     * @param Out Erc20Transfer callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc20Transfer", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc20Transfer(FString contractAddress, int32 walletindex,
                       FString toAddress, FString amount,
                       FErc20TransferFromDelegate Out);

    /**
     * erc20 Moves `amount` tokens from `from_address` to `to_address` using the
     * allowance mechanism.
     * @param contractAddress erc20 contract
     * @param walletindex wallet index which starts from 0
     * @param fromAddress from address to move
     * @param toAddress to address
     * @param amount amount
     * @param Out Erc20TransferFrom callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc20TransferFrom", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc20TransferFrom(FString contractAddress, int32 walletindex,
                           FString fromAddress, FString toAddress,
                           FString amount, FErc20TransferFromDelegate Out);

    /**
     * erc20 Allows `approved_address` to withdraw from your account multiple
     * times, up to the `amount` amount.
     * @param contractAddress erc20 contract
     * @param walletindex wallet index which starts from 0
     * @param approvedAddress address to approve
     * @param amount amount
     * @param Out Erc20Approve callback
     *
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc20Approve", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc20Approve(FString contractAddress, int32 walletindex,
                      FString approvedAddress, FString amount,
                      FErc20ApproveDelegate Out);

    /**
     * Returns the amount of tokens in existence
     * @param contractAddress erc20 contract
     * @param erc20owner erc20 owner
     * @param erc20spender erc20 spender
     * @param result allowance
     * @param success whether succeed or not
     * @param output_message error message, "" if succeed
     */

    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc20Allowance", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc20Allowance(FString contractAddress, FString erc20owner,
                        FString erc20spender, FString &result, bool &success,
                        FString &output_message);

    /**
     * erc721 Moves `amount` tokens from `from_address` to `to_address` using
     * the allowance mechanism.
     * @param contractAddress erc721 contract
     * @param walletindex wallet index which starts from 0
     * @param fromAddress from address to move
     * @param toAddress to address
     * @param tokenid token id
     * @param Out Erc721TransferFrom callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721TransferFrom", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721TransferFrom(FString contractAddress, int32 walletindex,
                            FString fromAddress, FString toAddress,
                            FString tokenid, FErc721TransferFromDelegate Out);

    /**
     * Safely transfers `token_id` token from `from_address` to `to_address`.
     * @param contractAddress erc721 contract
     * @param walletindex wallet index which starts from 0
     * @param fromAddress from address to move
     * @param toAddress to address
     * @param tokenid token id
     * @param Out Erc721SafeTransferFrom callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721SafeTransferFrom",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721SafeTransferFrom(FString contractAddress, int32 walletindex,
                                FString fromAddress, FString toAddress,
                                FString tokenid,
                                FErc721TransferFromDelegate Out);

    /**
     * Safely transfers `token_id` token from `from_address` to `to_address`
     * with `additional_data`.
     * @param contractAddress erc721 contract
     * @param walletindex wallet index which starts from 0
     * @param fromAddress from address to move
     * @param toAddress to address
     * @param tokenid token id
     * @param Out Erc721SafeTransferFromWithData callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721SafeTransferFromWithData",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721SafeTransferFromWithData(FString contractAddress,
                                        int32 walletindex, FString fromAddress,
                                        FString toAddress, FString tokenid,
                                        TArray<uint8> additionaldata,
                                        FErc721TransferFromDelegate Out);

    /**
     * erc721 Allows `approved_address` to withdraw from your account multiple
     * times, up to the `amount` amount.
     * @param contractAddress erc721 contract
     * @param walletindex wallet index which starts from 0
     * @param approvedAddress  address to approve
     * @param tokenid token id
     * @param Out Erc721Approve callback
     *
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc721Approve", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc721Approve(FString contractAddress, int32 walletindex,
                       FString approvedAddress, FString tokenid,
                       FErc721ApproveDelegate Out);

    /**
     * erc1155 Moves `amount` tokens from `from_address` to `to_address` using
     * the allowance mechanism.
     * @param contractAddress erc1155 contract
     * @param walletindex wallet index which starts from 0
     * @param fromAddress from address to move
     * @param toAddress to address
     * @param tokenid token id
     * @param amount amount
     * @param additionaldata additional data
     * @param Out Erc1155SafeTransferFrom callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc1155SafeTransferFrom",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc1155SafeTransferFrom(FString contractAddress, int32 walletindex,
                                 FString fromAddress, FString toAddress,
                                 FString tokenid, FString amount,
                                 TArray<uint8> additionaldata,
                                 FErc1155TransferFromDelegate Out);

    /**
     * Batched version of safeTransferFrom.
     * @param contractAddress erc1155 contract
     * @param walletindex wallet index which starts from 0
     * @param fromAddress from address to move
     * @param toAddress to address
     * @param tokenids token ids
     * @param amounts amounts
     * @param additionaldata additional data
     * @param Out Erc1155SafeBatchTransferFrom callback
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc1155SafeBatchTransferFrom",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc1155SafeBatchTransferFrom(
        FString contractAddress, int32 walletindex, FString fromAddress,
        FString toAddress, TArray<FString> tokenids, TArray<FString> amounts,
        TArray<uint8> additionaldata, FErc1155TransferFromDelegate Out);

    /**
     * erc1155 Allows `approved_address` to withdraw from your account multiple
     * times, up to the `amount` amount.
     * @param contractAddress erc1155 contract
     * @param walletindex wallet index which starts from 0
     * @param approvedAddress address to approve
     * @param approved approved or not
     * @param Out Erc1155Approve callback
     *
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "Erc1155Approve", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void Erc1155Approve(FString contractAddress, int32 walletindex,
                        FString approvedAddress, bool approved,
                        FErc1155ApproveDelegate Out);

    /**
     * create dynamic contract object for call
     * @param contractaddress function name to encode
     * @param abijson abi json string (not file path, actual json)
     * @param success  success or not
     * @param output_message result message
     *
     */

    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "CreateDynamicContract",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    UDynamicContractObject *CreateDynamicContract(FString contractaddress,
                                                  FString abijson,
                                                  bool &success,
                                                  FString &output_message);

    /**
     * initialize dynamic contract for send
     * @param contractaddress function name to encode
     * @param abijson abi json string (not file path, actual json)
     * @param walletindex which wallet to use (starts from 0)
     * @param success  success or not
     * @param output_message result message
     *
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "CreateDynamicSigningContract",
                      Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    UDynamicContractObject *
    CreateDynamicSigningContract(FString contractaddress, FString abijson,
                                 int32 walletindex, bool &success,
                                 FString &output_message);

    /**
     * Grpc address
     * for example: http://127.0.0.1:1316
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString myGrpc;

    /**
     * Cosmos rpc address
     * for example: http://127.0.0.1:1317
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString myCosmosRpc;

    /**
     * Tendermint rpc address
     * for example: http://127.0.0.1:26657
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString myTendermintRpc;

    /**
     * Cosmos chain-id
     * for example: testnet-test-1
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString myChainID;

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

    ADefiWalletCoreActor();

    void CreateWallet(FString mneomnics, FString password);

    /**
     * destroy wallet pointer
     */

    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "DestroyWallet", Keywords = "Wallet"),
              Category = "CronosPlayUnreal")
    void DestroyWallet();

  protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void Destroyed() override;

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
};
