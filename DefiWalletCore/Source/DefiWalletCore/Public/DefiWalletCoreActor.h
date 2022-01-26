// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefiWalletCoreActor.generated.h"

UCLASS()
class DEFIWALLETCORE_API ADefiWalletCoreActor : public AActor {
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString myCosmosRpc;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString myTendermintRpc;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString myMnemonics;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString myChainID;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString myFrom;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString myTo;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  int64 myAmount;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefiWalletCore")
  FString myAmountDenom;

  // Sets default values for this actor's properties
  ADefiWalletCoreActor();

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;
};
