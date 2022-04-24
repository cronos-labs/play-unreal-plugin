// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "walletcore/include/defi-wallet-core-cpp/src/contract.rs.h"
#include "walletcore/include/defi-wallet-core-cpp/src/lib.rs.h"
#include "walletcore/include/defi-wallet-core-cpp/src/nft.rs.h"
#include "DefiWalletCoreActor.generated.h"

/**
 Cosmos NFT Denom
 */
USTRUCT(BlueprintType)
struct FCosmosNFTDenom {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString ID;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString Schema;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString Creator;
};

/**
 * Cosmos NFT Token
 */
USTRUCT(BlueprintType)
struct FCosmosNFTToken {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString ID;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString URI;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString Data;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString Owner;
};

/**
 * Cosmos NFT Collection
 */
USTRUCT(BlueprintType)
struct FCosmosNFTCollection {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  bool DenomOption;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FCosmosNFTDenom DenomValue;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  TArray<FCosmosNFTToken> NFTs;
};

/**
 * Cosmos ID Collection
 */
USTRUCT(BlueprintType)
struct FCosmosNFTIDCollection {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString DenomID;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  TArray<FString> TokenIDs;
};

/**
 * Cosmos NFT Owner
 */
USTRUCT(BlueprintType)
struct FCosmosNFTOwner {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString Address;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  TArray<FCosmosNFTIDCollection> IDCollections;
};

UCLASS()
class DEFIWALLETCORE_API ADefiWalletCoreActor : public AActor {
  GENERATED_BODY()
public:
  /**
   * Restore wallet with mnemnonics.
   * @param mnemonics mnemonics. to restore
   * @param password  salt in mnemonics restoration
   * @param output generated address (index=0)
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "IntializeWallet", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void InitializeWallet(FString mnemonics, FString password, FString &output,
                        bool &success, FString &message);

  /**
   * Cosmos send amount.
   * @param walletIndex   address index which starts from 0
   * @param fromaddress  sender address
   * @param toaddress  receiver address
   * @param amount amount to send
   * @param amountdenom   amount denom to send
   * @param output transaction hash
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SendAmount", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void SendAmount(int32 walletIndex, FString fromaddress, FString toaddress,
                  int64 amount, FString amountdenom, FString &output,
                  bool &success, FString &message);

  /**
   * Cosmos get address with specified index.
   * @param index  wallet index which starts from 0
   * @param output address string
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetAddress", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void GetAddress(int32 index, FString &output, bool &success,
                  FString &message);

  /**
   *    Cosmos get balance.
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetBalance", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void GetBalance(FString address, FString denom, FString &output,
                  bool &success, FString &message);

  /**
   * Cosmos get nft supply.
   * @param mygrpc grpc url http://127.0.0.1:9090
   * @param denomid denom id
   * @param nftowner nft owner
   * @param output nft supply
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetNFTSupply", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void GetNFTSupply(FString mygrpc, FString denomid, FString nftowner,
                    int64 &output, bool &success, FString &message);

  /**
   * Cosmos get nft owner.
   * @param mygrpc grpc url http://127.0.0.1:9090
   * @param denomid denom id
   * @param nftowner nft owner
   * @param output cosmos nft owner
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetNFTOwner", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void GetNFTOwner(FString mygrpc, FString denomid, FString nftowner,
                   FCosmosNFTOwner &output, bool &success, FString &message);

  /**
   * Cos get nft collection.
   * @param mygrpc grpc url http://127.0.0.1:9090
   * @param denomid denom id
   * @param output cosmos nft collection
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetNFTCollection", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void GetNFTCollection(FString mygrpc, FString denomid,
                        FCosmosNFTCollection &output, bool &success,
                        FString &message);

  /**
   * Cosmos get nft denom.
   * @param mygrpc grpc url http://127.0.0.1:9090
   * @param denomid denom id
   * @param output cosmos nft denom
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetNFTDenom", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void GetNFTDenom(FString mygrpc, FString denomid, FCosmosNFTDenom &output,
                   bool &success, FString &message);

  /**
   * Cosmos get nft denom by name
   * @param mygrpc grpc url http://127.0.0.1:9090
   * @param denomname denom name
   * @param output cosmos nft denom
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetNFTDenomByName", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void GetNFTDenomByName(FString mygrpc, FString denomname,
                         FCosmosNFTDenom &output, bool &success,
                         FString &message);

  /**
   * Get all nft denoms
   * @param mygrpc grpc url http://127.0.0.1:9090
   * @param output cosmos nft denom
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetNFTAllDenoms", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void GetNFTAllDenoms(FString mygrpc, TArray<FCosmosNFTDenom> &output,
                       bool &success, FString &message);

  /**
   * Get nft token
   * @param mygrpc grpc url
   * @param denomid denom id
   * @param tokenid token id
   * @param cosmos nft token
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetNFTToken", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void GetNFTToken(FString mygrpc, FString denomid, FString tokenid,
                   FCosmosNFTToken &output, bool &success, FString &message);

  /**
   * Get eth address with index
   * @param index wallet index which starts from 0
   * @param output get eth address
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetEthAddress", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void GetEthAddress(int32 index, FString &output, bool &success,
                     FString &message);

  /**
   * Get eth balance
   * @param address eth address
   * @param output get balance
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetEthBalance", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void GetEthBalance(FString address, FString &output, bool &success,
                     FString &message);

  /**
   * Sign eth amount
   * @param walletIndex wallet index which starts from 0
   * @param fromaddress sender address
   * @param toaddress receiver address
   * @param amount amount to send in base format
   * @param gasLimit gas limit
   * @param gasPrice gas price, fee= gasLiimit * gasPrice
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   * @return signed transaction as bytes
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SignEthAmount", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  TArray<uint8> SignEthAmount(int32 walletIndex, FString fromaddress,
                              FString toaddress, FString amount,
                              FString gasLimit, FString gasPrice, bool &success,
                              FString &message);

  /**
   * Send eth amount
   * @param walletIndex wallet index which starts from 0
   * @param fromaddress sender address
   * @param toaddress receiver address
   * @param amount amount to send in base format
   * @param gasLimit gas limit
   * @param gasPrice gas price, fee= gasLiimit * gasPrice
   * @param output transaction hash
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SendEthAmount", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void SendEthAmount(int32 walletIndex, FString fromaddress, FString toaddress,
                     FString amount, FString gasLimit, FString gasPrice,
                     FString &output, bool &success, FString &message);

  /**
   * Sign eth login
   * @param walletIndex which wallet to use
   * @param document document to sign
   * @param signature get signature
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SignLogin", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void SignLogin(int32 walletIndex, FString document, TArray<uint8> &signature,
                 bool &success, FString &message);

  /**
   * Verify eth login
   * @param document: document to verify
   * @param signature signature to verify
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "VerifyLogin", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void VerifyLogin(FString document, TArray<uint8> signature, bool &success,
                   FString &message);

  /**
   * Get erc-20 balance
   * @param contractAddress erc20 contract address
   * @param accountAddress account address to fetch balance
   * @param balance get balance of account address
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc20Balance", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void Erc20Balance(FString contractAddress, FString accountAddress,
                    FString &balance, bool &success, FString &message);

  /**
   * Get erc-721 balance, minted token total count of this address
   * @param contractAddress erc721 contract address
   * @param accountAddress account address to fetch balance
   * @param balance to get balance of this address
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc721Balance", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void Erc721Balance(FString contractAddress, FString accountAddress,
                     FString &balance, bool &success, FString &message);

  /**
   * Get erc-1155 balance
   * @param contractAddress erc1155 contract address
   * @param accountAddress account address to fetch balance
   * @param tokenID toiken id to fetch balance
   * @param balance to get balance
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc1155Balance", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void Erc1155Balance(FString contractAddress, FString accountAddress,
                      FString tokenID, FString &balance, bool &success,
                      FString &message);

  /**
   * Get erc-20 name
   * @param contractAddress erc20 contract address
   * @param name get name
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc20Name", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void Erc20Name(FString contractAddress, FString &name, bool &success,
                 FString &message);

  /**
   * Get erc-20 symbol
   * @param contractAddress erc20 contract address
   * @param symbol get symbol
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc20Symbol", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void Erc20Symbol(FString contractAddress, FString &symbol, bool &success,
                   FString &message);

  /**
   * Get erc-20 decimals\
   * @param contractAddress erc20 contract address
   * @param decimals  get decimals
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc20Decimals", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void Erc20Decimals(FString contractAddress, int32 &decimals, bool &success,
                     FString &message);

  /**
   * Get erc-20 total suppy
   * @param contractAddress erc20 contract address
   * @param totalSuppy get total supply
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc20TotalSupply", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void Erc20TotalSupply(FString contractAddress, FString &totalSupply,
                        bool &success, FString &message);

  /**
   * Get erc-721 name
   * @param contractAddress erc721 contract address
   * @param name get name
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc721Name", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void Erc721Name(FString contractAddress, FString &name, bool &success,
                  FString &message);

  /**
   * Get erc-721 symbol
   * @param contractAddress contract address
   * @param symbol get symbol
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc721Symbol", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void Erc721Symbol(FString contractAddress, FString &symbol, bool &success,
                    FString &message);

  /**
   * Get erc-721 uri
   * @param contractAddress erc721 contract address
   * @param tokenID token id
   * @param uri  get uri
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc721Uri", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void Erc721Uri(FString contractAddress, FString tokenID, FString &uri,
                 bool &success, FString &message);

  /**
   * Get erc-721 owner
   * @param contractAddress erc 721 contract address
   * @param tokenID token id
   * @param ercowner get owner
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc721Owner", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void Erc721Owner(FString contractAddress, FString tokenID, FString &ercowner,
                   bool &success, FString &message);

  /**
   * Get erc-1155 uri
   * @param contractAddress erc 1155 contract address
   * @param tokenID token ID
   * @param uri  get uri
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "Erc1155Uri", Keywords = "Wallet"),
            Category = "DefiWalletCore")
  void Erc1155Uri(FString contractAddress, FString tokenID, FString &uri,
                  bool &success, FString &message);

  /**
   * Cosmos rpc address
   * for example: http://127.0.0.1:1317
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString myCosmosRpc;

  /**
   * Tendermint rpc address
   * for example: http://127.0.0.1:26657
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString myTendermintRpc;

  /**
   * Cosmos chain-id
   * for example: testnet-test-1
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString myChainID;

  /**
   *  Cronos rpc address
   *  for example:  http://127.0.0.1:8545
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString myCronosRpc;

  /**
   * Cronos chain-id
   * for example: 777
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  int32 myCronosChainID;

  ADefiWalletCoreActor();

  /**
   Opaque pointer to store wallet
   */
  org::defi_wallet_core::Wallet *_coreWallet;

  void CreateWallet(FString mneomnics, FString password);
  void DestroyWallet();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
  virtual void Destroyed() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;
};
