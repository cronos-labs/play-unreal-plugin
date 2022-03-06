// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DefiWalletBlueprint.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams(FWalletBroadcastDelegate, FString, TXHash, FString ,Result);


/**
 * 
 */
UCLASS()
class DEFIWALLETCORE_API UDefiWalletBlueprint : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
    
    UFUNCTION(BlueprintCallable, Category = "DefiWalletCore", meta=(DisplayName="BroadcastEthTxAsync", Keywords = "Wallet"))
            static void BroadcastEthTxAsync(FWalletBroadcastDelegate Out, TArray<uint8>  signedtx, FString rpc );
        
	
};
