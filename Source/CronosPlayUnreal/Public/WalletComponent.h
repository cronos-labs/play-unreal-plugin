// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "DefiWalletCoreActor.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/lib.rs.h"
#include "WalletComponent.generated.h"

/// Describes what coin type to use (for HD derivation or address generation)
UENUM(BlueprintType)
enum class ECoinType : uint8 {
  /// Crypto.org Chain mainnet
  CryptoOrgMainnet,
  /// Crypto.org Chain testnet
  CryptoOrgTestnet,
  /// Cronos mainnet beta
  CronosMainnet,
  /// Cosmos Hub mainnet
  CosmosHub,
  /// Ethereum
  Ethereum,
};

ENUM_RANGE_BY_FIRST_AND_LAST(ECoinType, ECoinType::CryptoOrgMainnet,
                             ECoinType::Ethereum);

// Wrapper of org::defi_wallet_core::Wallet
UCLASS(BlueprintType)
class CRONOSPLAYUNREAL_API UWallet : public UObject {
  GENERATED_BODY()
public:
  UWallet();
  /**
   * Get address with index
   * @param index wallet index which starts from 0
   * @param coin_type describes what coin type to use (for HD derivation or
   * address generation)
   * @param address get eth address
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetAddress", Keywords = "Wallet"),
            Category = "CronosPlayUnreal")
  void GetAddress(int32 index, ECoinType coin_type, FString &address,
                  FString &output_message);

  /**
   Opaque pointer to store wallet
   */
  org::defi_wallet_core::Wallet *_coreWallet;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class CRONOSPLAYUNREAL_API UWalletComponent : public UActorComponent {
  GENERATED_BODY()

public:
  // Sets default values for this component's properties
  UWalletComponent();

protected:
  // Called when the game starts
  virtual void BeginPlay() override;

public:
  // Called every frame
  virtual void
  TickComponent(float DeltaTime, ELevelTick TickType,
                FActorComponentTickFunction *ThisTickFunction) override;

  /**
   * Restore wallet with mnemonics and password.
   * @param mnemonics mnemonics to restore
   * @param password salt in mnemonics restoration
   * @param output generated address (index=0)
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "RestoreWallet", Keywords = "Wallet"),
            Category = "CronosPlayUnreal")
  UWallet *RestoreWallet(FString mnemonics, FString password,
                         FString &output_message);

  /**
   * Create a new wallet with password and wordcount.
   * @param password salt in mnemonics restoration
   * @param wordcount mnemonics word count (12, 18, 24)
   * @param output generated address (index=0)
   * @param success whether succeed or not
   * @param message error message, "" if succeed
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "InitializeNewDevelopmentOnlyWallet",
                    Keywords = "Wallet"),
            Category = "CronosPlayUnreal")
  UWallet *InitializeNewWallet(FString password, EMnemonicsWordCount wordcount,
                               FString &output_message);
};
