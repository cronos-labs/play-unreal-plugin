// Copyright 2022, Cronos Labs. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/contract.rs.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/lib.rs.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/nft.rs.h"
#include "PlayCppSdkLibrary/Include/defi-wallet-core-cpp/src/ethereum.rs.h"
#include "DynamicContractObject.generated.h"

class ADefiWalletCoreActor; // NOLINT
/**
 Cronos Transaction Receipt Raw
 */
USTRUCT(BlueprintType)
struct FCronosTransactionReceiptRaw {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    TArray<uint8> TransationHash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    TArray<uint8> BlockHash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString BlockNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString CumulativeGasUsed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString GasUsed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString ContractAddress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    TArray<FString> Logs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    TArray<uint8> Root;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    TArray<uint8> LogsBloom;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString TransactionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    FString EffectiveGasPrice;
};

// dynamic contract send
DECLARE_DYNAMIC_DELEGATE_TwoParams(FDynamicContractSendDelegate,
                                   FCronosTransactionReceiptRaw, TxResult,
                                   FString, Result);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FCallDynamicContractDelegate, FString,
                                   JsonResult, FString, Result);

/**
 * Dynamic Contract Object for Cronos Play Unreal
 */
UCLASS(Blueprintable, BlueprintType)
class CRONOSPLAYUNREAL_API UDynamicContractObject : public UObject {
    GENERATED_BODY()

  private:
    org::defi_wallet_core::EthContract *_coreContract;

  public:
    /**
     * read json file and get json value from the key
     * @param filepath the json file path
     * @param keyname the key name to get value
     * @param success  success or not
     * @param output_message result message
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "DynamicContractReadJson",
                      Keywords = "Json,Rpc,DynamicContract,Http"),
              Category = "CronosPlayUnreal")
    static FString DynamicContractReadJson(FString filepath, FString keyname,
                                           bool &success,
                                           FString &output_message);

    /**
     * initialize dynamic contract for send
     * @param contractaddress function name to encode
     * @param abijson abi json string (not file path, actual json)
     * @param walletindex which wallet to use (starts from 0)
     * @param success  success or not
     * @param output_message result message
     *
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "NewSigningEthContract",
                      Keywords = "DynamicContract,Wallet"),
              Category = "CronosPlayUnreal")
    void NewSigningEthContract(FString contractaddress, FString abijson,
                               int32 walletindex, bool &success,
                               FString &output_message);

    /**
     * initialize dynamic contract for call
     * @param contractaddress function name to encode
     * @param abijson abi json string (not file path, actual json)
     * @param success  success or not
     * @param output_message result message
     *
     */

    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "NewEthContract",
                      Keywords = "DynamicContract"),
              Category = "CronosPlayUnreal")
    void NewEthContract(FString contractaddress, FString abijson, bool &success,
                        FString &output_message);

    /**
     * encode dynamic contract
     * @param functionName function name to encode
     * @param functionArgs wallet index which starts from 0
         \code
     * simple json example: Address, String
     * solidity function signature: safeMint(address,string)
     * [{"Address":{"data":"0x00"}},{"Str":{"data":"my"}}]
     * for all datatypes :
     [{"Address":{"data":"0x0000000000000000000000000000000000000000"}},{"FixedBytes":{"data":[1,2]}},{"Bytes":{"data":[1,2]}},{"Int":{"data":"1"}},{"Uint":{"data":"1"}},{"Bool":{"data":true}},{"Str":{"data":"test"}},{"FixedArray":{"data":[{"Int":{"data":"1"}}]}},{"Array":{"data":[{"Int":{"data":"1"}}]}},{"Tuple":{"data":[{"Int":{"data":"1"}}]}}]
         \endcode

     * @param output encoded bytes output
     * @param success  success or not
     * @param Out CallDynamicContract callback
     *
     */

    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "EncodeDynamicContract",
                      Keywords = "Json,Rpc,DynamicContract,Http"),
              Category = "PlayCppSdk")
    void EncodeDynamicContract(FString functionName, FString functionArgs,
                               TArray<uint8> &output, bool &success,
                               FString &output_message);

    /**
     * call dynamic contract (non state changing)
     * @param functionName function name to call
     * @param functionArgs wallet index which starts from 0
     * @param Out CallDynamicContract callback
     *
     */

    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "CallDynamicContract",
                      Keywords = "Json,Rpc,DynamicContract,Http"),
              Category = "PlayCppSdk")
    void CallDynamicContract(FString functionName, FString functionArgs,
                             FCallDynamicContractDelegate Out);

    /**
     * sign and send dynamic contract (state changing)
     * @param functionName function name to send
     * @param functionArgs wallet index which starts from 0
     * @param Out SendDynamicContract callback
     *
     */

    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "SendDynamicContract",
                      Keywords = "Json,Rpc,DynamicContract,Http"),
              Category = "CronosPlayUnreal")
    void SendDynamicContract(FString functionName, FString functionArgs,
                             FDynamicContractSendDelegate Out);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
    ADefiWalletCoreActor *defiWallet;

    /**
     * destroy contract pointer
     *
     */

    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "DestroyContract",
                      Keywords = "Json,Rpc,DynamicContract,Http"),
              Category = "PlayCppSdk")
    void DestroyContract();

    UDynamicContractObject();
    virtual void BeginDestroy() override;
};
