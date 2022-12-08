// Copyright 2022, Cronos Labs. All Rights Reserved

/**
 * cronos token, NFT
 *
 */
#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayCppSdkBPLibrary.generated.h"

/// Token Query Options
UENUM(BlueprintType)
enum class EQueryOption : uint8 {
    TE_ByContract UMETA(DisplayName = "ByContract"),
    TE_ByAddressAndContract UMETA(DisplayName = "ByAddressAndContract"),
    TE_ByAddress UMETA(DisplayName = "ByAddress"),
};

/// Token ownership result detail from BlockScout API
USTRUCT(BlueprintType)
struct FRawTokenResult {
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

DECLARE_DYNAMIC_DELEGATE_TwoParams(FCallDynamicContractDelegate, FString,
                                   JsonResult, FString, Result);

/// Tx Detailed Information
USTRUCT(BlueprintType)
struct FRawTxDetail {
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
class UPlayCppSdkBPLibrary : public UBlueprintFunctionLibrary {
    GENERATED_UCLASS_BODY()

  public:
    /**
     * crono-scan api, get transaction history
     * @param address the address to query
     * @param apikey the api key
     * @param output the output of the query
     * @param success success of the query
     * @param output_message error message of the query
     *
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetTransactionHistoryBlocking",
                      Keywords = "Nft,Token,TransactionHistory,CronoScan"),
              Category = "PlayCppSdk")
    static void GetTransactionHistoryBlocking(FString address, FString apikey,
                                              TArray<FRawTxDetail> &output,
                                              bool &success,
                                              FString &output_message);
    /**
     * crono-scan api, get erc20 transaction history
     * @param address the address to query
     * @param ContractAddress the contract address to query
     * @param option the query option
     * @param api_key the api key
     * @param output the output of the query
     * @param success success of the query
     * @param output_message error message of the query
     *
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetErc20TransferHistoryBlocking",
                      Keywords = "Nft,Token,Erc20,TransferHistory,CronoScan"),
              Category = "PlayCppSdk")
    static void
    GetErc20TransferHistoryBlocking(FString address, FString ContractAddress,
                                    EQueryOption option, FString api_key,
                                    TArray<FRawTxDetail> &output, bool &success,
                                    FString &output_message);

    /**
     * crono-scan api, get erc721 transaction history
     *  returns the ERC721 transfers of a given address of a given contract.
     * (address can be empty if option is ByContract)
     * default option is by address
     * The API key can be obtained from https://cronoscan.com
     * @param address the address to query
     * @param ContractAddress the contract address to query
     * @param option the query option
     * @param api_key the api key
     * @param output the output of the query
     * @param success success of the query
     * @param output_message error message of the query
     *
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetErc721TransferHistoryBlocking",
                      Keywords = "Nft,Token,Erc721,TransferHistory,CronoScan"),
              Category = "PlayCppSdk")
    static void
    GetErc721TransferHistoryBlocking(FString address, FString ContractAddress,
                                     EQueryOption option, FString api_key,
                                     TArray<FRawTxDetail> &output,
                                     bool &success, FString &output_message);

    /**
     * given the BlockScout REST API base url and the account address
     * (hexadecimal), it will return the list of all owned tokens (ref:
     * https://cronos.org/explorer/testnet3/api-docs)
     * @param blockscoutBaseUrl the base url of the BlockScout API (e.g.
     * https://cronos.org/explorer/api)
     * @param account_address the account address to query
     * @param output the output of the query
     * @param success   success of the query
     * @param output_message error message of the query
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetTokensBlocking",
                      Keywords = "Nft,Token,Url,Blockscout"),
              Category = "PlayCppSdk")
    static void GetTokensBlocking(FString blockscoutBaseUrl,
                                  FString account_address,
                                  TArray<FRawTokenResult> &output,
                                  bool &success, FString &output_message);

    /**
     * given the BlockScout REST API base url and the account address
    (hexadecimal; required)
    * and optional contract address (hexadecimal; optional -- it can be empty if
    the option is ByAddress),
    * it will return all the token transfers (ERC20, ERC721... in the newer
    BlockScout
    * releases, also ERC1155)
    * (ref: https://cronos.org/explorer/testnet3/api-docs)
    * NOTE: QueryOption::ByContract is not supported by BlockScout
    * @param blockscoutBaseUrl the base url of the BlockScout API (e.g.
    * https://cronos.org/explorer/api)
    * @param address the account address to query
    * @param contractAddress the contract address to query
    * @param option the query option
    * @param output the output of the query
    * @param success success of the query
    * @param output_message error message of the query
    */

    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetTokenTransfersBlocking",
                      Keywords = "Nft,Token,Transfers,Blockscout"),
              Category = "PlayCppSdk")
    static void
    GetTokenTransfersBlocking(FString blockscoutBaseUrl, FString address,
                              FString contractAddress, EQueryOption option,
                              TArray<FRawTxDetail> &output, bool &success,
                              FString &output_message);

    /**
     * Generate QRCode from string
     * @param string the string to encode
     * @return the QRCode as a texture
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GenerateQrCode",
                      Keywords = "Wallet,QR,Qrcode,Texture"),
              Category = "PlayCppSdk")
    static UTexture2D *GenerateQrCode(FString string);

    /**
     * SetupUserAgent for http access
     * @param useragent "CronosPlay-UnrealEngine-Agent"
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "SetupUserAgent",
                      Keywords = "Json,Rpc,UserAgent,Http"),
              Category = "PlayCppSdk")
    static void SetupUserAgent(FString UserAgent);

    /**
     * EncodeDynamicContract for http access
     * @param rpcserver    cronos rpc server endpoint
     * @param contactAddress    contract address
     * @param abiJson    contract abi json
     * @param functionName    function name
     * @param functionArgs    function args in json
     * @return encoded bytes
     */

    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "EncodeDynamicContract",
                      Keywords = "Json,Rpc,DynamicContract,Http"),
              Category = "PlayCppSdk")
    static void EncodeDynamicContract(FString rpcserver, FString contactAddress,
                                      FString abiJson, FString functionName,
                                      FString functionArgs,
                                      TArray<uint8> &output, bool &success,
                                      FString &output_message);

    /**
     * call dynamic contract (non modifying)
     *
     * @param rpcserver    cronos rpc server endpoint
     * @param contactAddress    contract address
     * @param abiJson    contract abi json
     * @param functionName    function name
     * @param functionArgs    function args in json
     * @return output in json
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "CallDynamicContract",
                      Keywords = "Json,Rpc,DynamicContract,Http"),
              Category = "PlayCppSdk")
    static void CallDynamicContract(FString rpcserver, FString contactAddress,
                                    FString abiJson,

                                    FString functionName, FString functionArgs,
                                    FCallDynamicContractDelegate Out);

    /**
     * read json from file , choose by keyname
     *
     * @param filepath    json file path
     * @param keyname    key name
     * @return output in json
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "DynamicContractReadJson",
                      Keywords = "Json,Rpc,DynamicContract,Http"),
              Category = "PlayCppSdk")
    static FString DynamicContractReadJson(FString filepath, FString keyname,
                                           bool &success,
                                           FString &output_message);
};
