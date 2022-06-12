#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DefiWalletBlueprint.generated.h"
DECLARE_DYNAMIC_DELEGATE_TwoParams(FWalletBroadcastDelegate, FString, TXHash,
                                   FString, Result);

/**
 * Blueprint utility functions
 */
UCLASS()
class DEFIWALLETCORE_API UDefiWalletBlueprint
    : public UBlueprintFunctionLibrary {
  GENERATED_BODY()
public:
  /**
   * Broadcast signed eth tx
   * @param Out  event delegate which is triggered after tx is broadcasted
   * @param signedtx signed tx as bytes
   * @param rpc cronos rpc server url
   */
  UFUNCTION(BlueprintCallable, Category = "DefiWalletCore",
            meta = (DisplayName = "BroadcastEthTxAsync", Keywords = "Wallet"))
  static void BroadcastEthTxAsync(FWalletBroadcastDelegate Out,
                                  TArray<uint8> signedtx, FString rpc);
};
