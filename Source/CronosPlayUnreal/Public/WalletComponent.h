// Copyright 2022, Cronos Labs. All Rights Reserved

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "DefiWalletCoreActor.h"
#include "Error.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/lib.rs.h"
#include "UObject/Class.h"
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
   * @param error error struct
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetAddress", Keywords = "Wallet"),
            Category = "CronosPlayUnreal")
  void GetAddress(int32 index, ECoinType coin_type, FString &address,
                  FError &error);

  /**
   * Get backup mnemonic phrase.
   * @param mnemonic_phrase backup mnemonics
   * @param error error struct
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetBackupMnemonicPhrase",
                    Keywords = "Wallet"),
            Category = "CronosPlayUnreal")
  void GetBackupMnemonicPhrase(FString &mnemonic_phrase, FError &error);
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
   * @param error error struct
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "RestoreWallet", Keywords = "Wallet"),
            Category = "CronosPlayUnreal")
  UWallet *RestoreWallet(FString mnemonics, FString password, FError &error);

  /**
   * Create a new wallet with password and wordcount.
   * @param password salt in mnemonics restoration
   * @param wordcount mnemonics word count (12, 18, 24)
   * @param error error struct
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "InitializeNewWallet", Keywords = "Wallet"),
            Category = "CronosPlayUnreal")
  UWallet *InitializeNewWallet(FString password, EMnemonicsWordCount wordcount,
                               FError &error);

  /**
   * Generate mnemonics.
   * @param password salt in mnemonics restoration
   * @param wordcount mnemonics word count (12, 18, 24)
   * @param mnemonic_phrase generated mnemonics
   * @param error error struct
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GenerateMnemonics", Keywords = "Wallet"),
            Category = "CronosPlayUnreal")
  void GenerateMnemonics(FString password, EMnemonicsWordCount wordcount,
                         FString &mnemonic_phrase, FError &error);
};
