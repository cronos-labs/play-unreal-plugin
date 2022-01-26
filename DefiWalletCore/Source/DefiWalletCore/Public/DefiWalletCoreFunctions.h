// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DefiWalletCoreFunctions.generated.h"
/**
 *
 */
UCLASS()
class DEFIWALLETCORE_API UDefiWalletCoreFunctions
    : public UBlueprintFunctionLibrary {
  GENERATED_BODY()

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetDefaultAddress",
                    Keywords = "Wallet Address"),
            Category = "DefiWalletCore")
  static FString GetDefaultAddress(FString mnemonics);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SendAmount", Keywords = "Wallet Address"),
            Category = "DefiWalletCore")
  static FString SendAmount(FString mnemonics, FString chainid, FString from,
                            FString to, int64 amount, FString amountdenom,
                            FString cosmosrpc, FString tendermintrpc);
};
