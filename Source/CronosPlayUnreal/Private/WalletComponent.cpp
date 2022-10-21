// Fill out your copyright notice in the Description page of Project Settings.

#include "WalletComponent.h"

// Sets default values for this class's properties
UWallet::UWallet() {}

// Sets default values for this component's properties
UWalletComponent::UWalletComponent() {
  // Set this component to be initialized when the game starts, and to be ticked
  // every frame.  You can turn these features off to improve performance if you
  // don't need them.
  PrimaryComponentTick.bCanEverTick = false;

  // ...
}

// Called when the game starts
void UWalletComponent::BeginPlay() {
  Super::BeginPlay();

  // ...
}

// Called every frame
void UWalletComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction *ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // ...
}

UWallet *UWalletComponent::InitializeWallet(FString mnemonics, FString password,
                                            FString &output_message) {
  UWallet *wallet = NewObject<UWallet>();
  try {
    wallet->_coreWallet =
        restore_wallet(TCHAR_TO_UTF8(*mnemonics), TCHAR_TO_UTF8(*password))
            .into_raw();
  } catch (const rust::cxxbridge1::Error &e) {
    output_message = FString::Printf(TEXT("CronosPlayUnreal Error: %s"),
                                     UTF8_TO_TCHAR(e.what()));
  }
  return wallet;
}

void UWallet::GetAddress(int32 index, ECoinType coin_type, FString &address,
                         FString &output_message) {
  try {
    switch (coin_type) {
    case ECoinType::CryptoOrgMainnet:
      address = UTF8_TO_TCHAR(
          _coreWallet
              ->get_address(org::defi_wallet_core::CoinType::CronosMainnet,
                            index)
              .c_str());
      break;
    case ECoinType::CryptoOrgTestnet:
      address = UTF8_TO_TCHAR(
          _coreWallet
              ->get_address(org::defi_wallet_core::CoinType::CryptoOrgTestnet,
                            index)
              .c_str());
      break;
    case ECoinType::CronosMainnet:
      address = UTF8_TO_TCHAR(
          _coreWallet
              ->get_address(org::defi_wallet_core::CoinType::CronosMainnet,
                            index)
              .c_str());
      break;
    case ECoinType::CosmosHub:
      address = UTF8_TO_TCHAR(
          _coreWallet
              ->get_address(org::defi_wallet_core::CoinType::CosmosHub, index)
              .c_str());
      break;
    case ECoinType::Ethereum:
      address = UTF8_TO_TCHAR(
          _coreWallet
              ->get_address(org::defi_wallet_core::CoinType::Ethereum, index)
              .c_str());
      break;
    default:
      break;
    }

  } catch (const rust::cxxbridge1::Error &e) {
    output_message = FString::Printf(TEXT("CronosPlayUnreal Error: %s"),
                                     UTF8_TO_TCHAR(e.what()));
  }
}
