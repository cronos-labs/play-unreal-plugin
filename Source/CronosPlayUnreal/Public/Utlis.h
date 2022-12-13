// Copyright 2022, Cronos Labs. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utlis.generated.h"

/**
 *
 */
UCLASS()
class CRONOSPLAYUNREAL_API UUtlis : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Convert bytes to hex
     * @param address bytes address
     * @return address in hex string
     *
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "ToHex", Keywords = "PlayCppSdk"),
              Category = "Utils")
    static FString ToHex(TArray<uint8> address);
};
