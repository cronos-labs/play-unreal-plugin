/**
 * cronos token, NFT
 *
 */
#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlayCppSdkBPLibrary.generated.h"

/**
 Cronos NFT Info
 */
USTRUCT(BlueprintType)
struct FCronosNftInfo {
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
  FString JsonString;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
  FString Name;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
  FString Description;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
  FString Image;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CronosPlayUnreal")
  FString ImageUrl;
};

/// callback of get json
DECLARE_DYNAMIC_DELEGATE_TwoParams(FGetJsonStringFromUri, FString, JsonString,
                                   FString, Result);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FGetNftImageInfoFromUri, FCronosNftInfo,
                                   NftImageInfo, FString, Result);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FGetNftImageFromUrl, UTexture2D *, NftImage,
                                   FString, Result);

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
   * crono-scan api, get ercu721 transaction history
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
                                   TArray<FRawTxDetail> &output, bool &success,
                                   FString &output_message);

  /**
   * given the BlockScout REST API base url and the account address
   * (hexadecimal), it will return the list of all owned tokens (ref:
   * https://cronos.org/explorer/testnet3/api-docs)
   * @param blockscoutBaseUrl the base url of the BlockScout API
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
                                TArray<FRawTokenResult> &output, bool &success,
                                FString &output_message);

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
  * @param blockscoutBaseUrl the base url of the BlockScout API
  * @param address  the account address to query
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
  static void GetTokenTransfersBlocking(FString blockscoutBaseUrl,
                                        FString address,
                                        FString contractAddress,
                                        EQueryOption option,
                                        TArray<FRawTxDetail> &output,
                                        bool &success, FString &output_message);

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
   * GetNftTokenUri
   * @param tokenuri  tokenuri to get json
   * @param Out callback, fetched json string
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetJsonStringFromUri",
                    Keywords = "Nft,Json,Uri"),
            Category = "PlayCppSdk")

  static void GetJsonStringFromUri(FGetJsonStringFromUri Out, FString tokenuri);

  /**
   * GetNftImageFromUri
   * @param tokenuri  tokenuri to get json
   * @param Out callback, fetched nft image url
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetNftImageUrlFromUri",
                    Keywords = "Nft,Ipfs,Image,Uri"),
            Category = "PlayCppSdk")

  static void GetNftImageInfoFromUri(FGetNftImageInfoFromUri Out,
                                     FString tokenuri);

  /**
   * GetNftImageFromUrl
   * CAUTION: This function is dangerous. Validate imageurl and check , because
   * it can download any data from arbitrary url. before calling this api,
   * please check the image url is valid. validity check references:
   *  https://cloud.google.com/vision/docs/detecting-safe-search
   *  https://docs.aws.amazon.com/rekognition/latest/dg/moderation.html
   *  https://docs.microsoft.com/en-us/gaming/gdk/_content/gc/policies/xr/xr018
   * @param imageurl  imageurl to get nft image
   * @param Out callback, fetched nft image
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "GetNftImageFromUrl",
                    Keywords = "Nft,Ipfs,Image,Texture,Url"),
            Category = "PlayCppSdk")

  static void GetNftImageFromUrl(FGetNftImageFromUrl Out, FString imageurl);

  /**
   * SetupIpfsConverting
   * @param src ipfs uri "ipfs://"
   * @param dst ipfs url "https://ipfs.io/ipfs/"
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SetupIpfsConverting",
                    Keywords = "Nft,Ipfs,Uri,Url"),
            Category = "PlayCppSdk")

  static void SetupIpfsConverting(FString src, FString dst);

  /**
   * SetupUserAgent for http access
   * @param useragent "CronosPlay-UnrealEngine-Agent"
   */
  UFUNCTION(BlueprintCallable,
            meta = (DisplayName = "SetupIpfsConverting",
                    Keywords = "Nft,Ipfs,UserAgent,Http"),
            Category = "PlayCppSdk")

  static void SetupUserAgent(FString useragent);
};
