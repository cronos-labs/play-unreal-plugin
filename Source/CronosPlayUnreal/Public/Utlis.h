// Copyright 2022, Cronos Labs. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayCppSdkLibrary/Include/rust/cxx.h"
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

    /**
     * Convert TArray<uint8> to std::array<uint8_t, 20>
     * @param address bytes address
     * @return address in std::array<uint8_t, 20>, if address size is not 20,
     * return all 0
     *
     */
    static std::array<std::uint8_t, 20> ToArray(TArray<uint8> address);

    static void copyTArrayToVec(const TArray<uint8> &src, rust::Vec<uint8_t> &dst);
    static void copyVecToTArray(const rust::Vec<uint8_t> &src, TArray<uint8> &dst);
};
