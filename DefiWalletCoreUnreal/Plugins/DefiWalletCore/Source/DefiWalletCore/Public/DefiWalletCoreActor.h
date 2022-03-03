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
    void SendAmount(FString fromaddress, FString toaddress, int64 amount, FString amountdenom, FString& output, bool& success, FString& message);

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
    FString myCosmosRpc;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
    FString myTendermintRpc;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
    FString myChainID;

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
