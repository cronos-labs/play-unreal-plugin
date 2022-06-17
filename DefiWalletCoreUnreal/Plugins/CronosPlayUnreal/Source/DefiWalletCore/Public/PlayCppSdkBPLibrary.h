
#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayCppSdkBPLibrary.generated.h"

UENUM(BlueprintType)
enum class EQueryOption : uint8
{
  TE_ByContract UMETA(DisplayName = "ByContract"),
  TE_ByAddressAndContract UMETA(DisplayName = "ByAddressAndContract"),
  TE_ByAddress UMETA(DisplayName = "ByAddress"),
};

USTRUCT(BlueprintType)
struct FRawTokenResult
{
  GENERATED_BODY()
  /// how many tokens are owned by the address
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString Balance;
  /// the deployed contract address
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString ContractAddress;
  /// the number of decimal places
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString Decimals;
  /// the token id
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString ID;
  /// the human-readable name of the token
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString Name;
  /// the ticker for the token
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString Symbol;
  /// the token type (ERC-20, ERC-721, ERC-1155)
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString TokenType;
};

USTRUCT(BlueprintType)
struct FRawTxDetail
{
  GENERATED_BODY()

  /// Transaction hash
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString Hash;

  /// the hexadecimal address of the receiver
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString ToAddress;

  /// the hexadecimal address of the sender
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString FromAddress;

  /// the value sent in decimal (in base tokens)
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString Value;

  /// block number when it happened, 64bit integer
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString BlockNo;

  /// the time it happened
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString Timestamp;

  /// the address of the contract (if no contract, it's an empty string)
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayCppSdk")
  FString ContractAddress;
};

UCLASS()
class UPlayCppSdkBPLibrary : public UBlueprintFunctionLibrary
{
  GENERATED_UCLASS_BODY()

public:
  // cronoscan
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetTransactionHistoryBlocking",
                    Keywords = "Wallet"),
            Category = "PlayCppSdk")
  static void GetTransactionHistoryBlocking(FString address, FString apikey,
                                            TArray<FRawTxDetail> &output,
                                            bool &success,
                                            FString &output_message);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetErc20TransferHistoryBlocking",
                    Keywords = "Wallet"),
            Category = "PlayCppSdk")
  static void
  GetErc20TransferHistoryBlocking(FString address, FString ContractAddress,
                                  EQueryOption option, FString api_key,
                                  TArray<FRawTxDetail> &output, bool &success,
                                  FString &output_message);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetErc721TransferHistoryBlocking",
                    Keywords = "Wallet"),
            Category = "PlayCppSdk")
  static void
  GetErc721TransferHistoryBlocking(FString address, FString ContractAddress,
                                   EQueryOption option, FString api_key,
                                   TArray<FRawTxDetail> &output, bool &success,
                                   FString &output_message);

  // blackscout
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetTokensBlocking", Keywords = "Wallet"),
            Category = "PlayCppSdk")
  static void GetTokensBlocking(FString blockscoutBaseUrl,
                                FString account_address,
                                TArray<FRawTokenResult> &output, bool &success,
                                FString &output_message);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetTokenTransfersBlocking",
                    Keywords = "Wallet"),
            Category = "PlayCppSdk")
  static void GetTokenTransfersBlocking(FString blockscoutBaseUrl,
                                        FString address,
                                        FString contractAddress,
                                        EQueryOption option,
                                        TArray<FRawTxDetail> &output,
                                        bool &success, FString &output_message);

  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GenerateQrCode", Keywords = "Wallet"),
            Category = "PlayCppSdk")
  static UTexture2D *GenerateQrCode(FString string);
};
