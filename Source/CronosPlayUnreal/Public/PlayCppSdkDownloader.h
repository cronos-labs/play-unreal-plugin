// Copyright 2022, Cronos Labs. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "PlayCppSdkDownloader.generated.h"

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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FPlayDownloadProgress, int32,
                                               RecvSize, int32, TotalSize,
                                               float, Percent);

/// callback of get json
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayGetJsonString, FString,
                                             JsonString, FString, Result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayGetNftImageInfoFromUri,
                                             FCronosNftInfo, NftImageInfo,
                                             FString, Result);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPlayGetNftImageFromUrl,
                                             UTexture2D *, NftImage, FString,
                                             Result);

UCLASS(BlueprintType)
class CRONOSPLAYUNREAL_API UPlayCppSdkDownloader : public UObject {
    GENERATED_BODY()

  protected:
    FHttpModule *HttpModule;

  public:
    FString SrcIpfs;          // "ipfs://"
    FString DstIpfs;          // "https://ipfs.io/ipfs/"
    static FString UserAgent; // "CronosPlay-UnrealEngine-Agent"

    UPlayCppSdkDownloader();
    FString convert_ipfs_url(const FString &src);

    UPROPERTY(BlueprintAssignable)
    FPlayDownloadProgress PlayDownloadProgressCallback;
    void OnPlayDownloadProgressCallback(FHttpRequestPtr RequestPtr,
                                        int32 SendBytes, int32 RecvBytes);

    /**
     * SetupIpfsConverting
     * @param src ipfs uri "ipfs://"
     * @param dst ipfs url "https://ipfs.io/ipfs/"
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "SetupIpfsConverting",
                      Keywords = "Nft,Ipfs,Uri,Url"),
              Category = "PlayCppSdk")

    void SetupIpfsConverting(FString src, FString dst);

    /**
     * GetNftTokenUri
     * @param tokenuri  tokenuri to get json
     * @param success success of apicall
     * @param output_message  error message of apicall
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetJsonStringFromUri",
                      Keywords = "Nft,Json,Uri"),
              Category = "PlayCppSdk")

    void GetJsonStringFromUri(FString tokenuri, bool &success,
                              FString &output_message);

    UPROPERTY(BlueprintAssignable)
    FPlayGetJsonString GetJsonStringFromUriCallback;

    void OnGetJsonStringFromUriCompleteCallback(FHttpRequestPtr httprequest,
                                                FHttpResponsePtr httpresponse,
                                                bool connectedSuccessfully);

    /**
     * GetNftImageFromUri
     * @param tokenuri  tokenuri to get json
     * @param success success of apicall
     * @param output_message  error message of apicall
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetNftImageInfoFromUri",
                      Keywords = "Nft,Ipfs,Image,Uri"),
              Category = "PlayCppSdk")

    void GetNftImageInfoFromUri(FString tokenuri, bool &success,
                                FString &output_message);

    UPROPERTY(BlueprintAssignable)
    FPlayGetNftImageInfoFromUri GetNftImageInfoFromUriCallback;

    void OnGetNftImageInfoFromUriCompleteCallback(FHttpRequestPtr httprequest,
                                                  FHttpResponsePtr httpresponse,
                                                  bool connectedSuccessfully);

    /**
     * GetNftImageFromUrl
     * CAUTION: This function is dangerous. Validate imageurl and check ,
     * because it can download any data from arbitrary url. before calling this
     * api, please check the image url is valid. validity check references:
     *  https://cloud.google.com/vision/docs/detecting-safe-search
     *  https://docs.aws.amazon.com/rekognition/latest/dg/moderation.html
     *  https://docs.microsoft.com/en-us/gaming/gdk/_content/gc/policies/xr/xr018
     * @param imageurl  imageurl to get nft image
     * @param success success of apicall
     * @param output_message  error message of apicall
     */
    UFUNCTION(BlueprintCallable,
              meta = (DisplayName = "GetNftImageFromUrl",
                      Keywords = "Nft,Ipfs,Image,Texture,Url"),
              Category = "PlayCppSdk")

    void GetNftImageFromUrl(FString imageurl, bool &success,
                            FString &output_message);
    UPROPERTY(BlueprintAssignable)
    FPlayGetNftImageFromUrl GetNftImageFromUrlCallback;

    void OnGetNftImageFromUrlCompleteCallback(FHttpRequestPtr httprequest,
                                              FHttpResponsePtr httpresponse,
                                              bool connectedSuccessfully);
};
