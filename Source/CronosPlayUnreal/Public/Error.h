// Fill out your copyright notice in the Description page of Project Settings.

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
 *
 */
USTRUCT(BlueprintType)
struct FError {
  GENERATED_BODY()
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
  EErrorCategory category;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
  FString message;
};
