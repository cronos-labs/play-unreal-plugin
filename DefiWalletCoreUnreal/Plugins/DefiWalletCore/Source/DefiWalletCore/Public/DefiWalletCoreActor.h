// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "lib.rs.h"
#include "DefiWalletCoreActor.generated.h"




UCLASS()
class DEFIWALLETCORE_API ADefiWalletCoreActor : public AActor {
    GENERATED_BODY()

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
