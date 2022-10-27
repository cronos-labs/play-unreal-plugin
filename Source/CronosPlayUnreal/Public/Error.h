// Copyright 2022, Cronos Labs. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Error.generated.h"

/// Error Category
UENUM(BlueprintType)
enum class EErrorCategory : uint8 {
  WalletComponent,
  None,
};

/**
 * Error struct
 */
USTRUCT(BlueprintType)
struct FError {
  GENERATED_BODY()
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
  EErrorCategory category;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
  FString message;
};
