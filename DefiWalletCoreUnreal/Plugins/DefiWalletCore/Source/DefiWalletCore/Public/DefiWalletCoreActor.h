// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "lib.rs.h"
#include "nft.rs.h"
#include "DefiWalletCoreActor.generated.h"


USTRUCT(BlueprintType)
struct FCosmosNFTDenom
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    FString ID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    FString Name;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    FString Schema;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    FString Creator;
};

USTRUCT(BlueprintType)
struct FCosmosNFTToken
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    FString ID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    FString Name;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    FString URI;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    FString Data;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    FString Owner;
};

USTRUCT(BlueprintType)
struct FCosmosNFTCollection
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    bool DenomOption;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    FCosmosNFTDenom DenomValue;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    TArray<FCosmosNFTToken> NFTs;
    
};

USTRUCT(BlueprintType)
struct FCosmosNFTIDCollection
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    FString DenomID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    TArray<FString> TokenIDs;
    
};


USTRUCT(BlueprintType)
struct FCosmosNFTOwner
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    FString Address;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DefiWalletCore")
    TArray<FCosmosNFTIDCollection> IDCollections;
    
};




UCLASS()
class DEFIWALLETCORE_API ADefiWalletCoreActor : public AActor {
    GENERATED_BODY()

    // cosmos
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "IntializeWallet", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void InitializeWallet(FString mnemonics, FString password, FString& output, bool& success, FString& message);

    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "SendAmount", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void SendAmount(int32 walletIndex, FString fromaddress, FString toaddress, int64 amount, FString amountdenom, FString& output, bool& success, FString& message);
    
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "GetAddress", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void GetAddress(int32 index, FString& output, bool& success, FString& message);
    
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "GetBalance", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void GetBalance(FString address, FString denom, FString& output, bool& success, FString& message);
    
    // supply
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "GetNFTSupply", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void GetNFTSupply(FString mygrpc, FString denomid, FString nftowner,int64& output, bool& success, FString& message);
    
    
    // owner
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "GetNFTOwner", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void GetNFTOwner(FString mygrpc, FString denomid, FString nftowner,FCosmosNFTOwner& output, bool& success, FString& message);
    
    // collection
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "GetNFTCollection", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void GetNFTCollection(FString mygrpc, FString denomid, FCosmosNFTCollection& output, bool& success, FString& message);

    // denom
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "GetNFTDenom", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void GetNFTDenom(FString mygrpc, FString denomid, FCosmosNFTDenom& output, bool& success, FString& message);
    
    
    // denom_by_name
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "GetNFTDenomByName", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void GetNFTDenomByName(FString mygrpc, FString denomname, FCosmosNFTDenom& output, bool& success, FString& message);
    
        
    // grpcurl : http://127.0.0.1:9090
    // denoms
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "GetNFTAllDenoms", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void GetNFTAllDenoms(FString mygrpc, TArray<FCosmosNFTDenom>& output, bool& success, FString& message);
    
    // nft
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "GetNFTToken", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void GetNFTToken(FString mygrpc, FString denomid, FString tokenid,FCosmosNFTToken& output, bool& success, FString& message);
   
    

    
    // ethereum
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "GetEthAddress", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void GetEthAddress(int32 index, FString& output, bool& success, FString& message);
    
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "GetEthBalance", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void GetEthBalance(FString address, FString& output, bool& success, FString& message);
    
    
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "SignEthAmount", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    TArray<uint8> SignEthAmount(int32 walletIndex, int32 chainid,
                                             FString fromaddress, FString toaddress,
                                             FString amount, FString gasLimit,
                                             FString gasPrice,
                                                     bool &success, FString &message);
    
    
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "SendEthAmount", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void SendEthAmount(int32 walletIndex, int32 chainid, FString fromaddress, FString toaddress, FString amount, FString gasLimit, FString gasPrice, FString& output, bool& success, FString& message);
 
    // login
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "SignLogin", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void SignLogin(int32 walletIndex, FString document,TArray<uint8> &  signature, bool& success, FString& message);
    
    
    UFUNCTION(BlueprintCallable,
        meta = (DisplayName = "VerifyLogin", Keywords = "Wallet"),
        Category = "DefiWalletCore")
    void VerifyLogin(FString document, TArray<uint8> signature, bool& success, FString& message);
 
    
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
    FString myCosmosRpc;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
    FString myTendermintRpc;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
    FString myChainID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
    FString myCronosRpc;


    FString defaultAddress;

    // Sets default values for this actor's properties
    ADefiWalletCoreActor();

    org::defi_wallet_core::Wallet* _coreWallet;
    
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
