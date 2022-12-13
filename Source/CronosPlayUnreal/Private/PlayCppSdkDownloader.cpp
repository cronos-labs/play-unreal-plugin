// Copyright 2022, Cronos Labs. All Rights Reserved

#include "PlayCppSdkDownloader.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/KismetRenderingLibrary.h"
FString UPlayCppSdkDownloader::UserAgent =
    TEXT("CronosPlay-UnrealEngine-Agent");
// ipfs:// -> https://ipfs.io/ipfs/
FString UPlayCppSdkDownloader::convert_ipfs_url(const FString &src) {
    FString dst = src.Replace(*SrcIpfs, *DstIpfs);
    return dst;
}

void UPlayCppSdkDownloader::SetupIpfsConverting(FString src, FString dst) {
    SrcIpfs = src;
    DstIpfs = dst;
}

// Sets default values
UPlayCppSdkDownloader::UPlayCppSdkDownloader() {
    HttpModule = &FHttpModule::Get();
    SrcIpfs = TEXT("ipfs://");
    DstIpfs = TEXT("https://ipfs.io/ipfs/");
}

void UPlayCppSdkDownloader::OnPlayDownloadProgressCallback(
    FHttpRequestPtr RequestPtr, int32 SendBytes, int32 RecvBytes) {

    int32 TotalSize = RequestPtr->GetResponse()->GetContentLength();
    float Percent = (float)RecvBytes / TotalSize;

    PlayDownloadProgressCallback.Broadcast(RecvBytes, TotalSize, Percent);
}

void UPlayCppSdkDownloader::GetJsonStringFromUri(FString tokenuriuser,
                                                 bool &success,
                                                 FString &output_message

) {
    if (!GetJsonStringFromUriCallback.IsBound()) {
        success = false;
        output_message = "GetJsonStringFromUriCallback is not bound";
        return;
    }

    FString tokenuri = convert_ipfs_url(tokenuriuser);
    assert(HttpModule != NULL);
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httprequest =
        HttpModule->CreateRequest();
    httprequest->SetVerb(TEXT("GET"));
    httprequest->SetHeader(TEXT("User-Agent"), UserAgent);
    httprequest->SetHeader(TEXT("Content-Type"),
                           TEXT("application/x-www-form-urlencoded"));
    httprequest->SetURL(tokenuri);

    httprequest->OnRequestProgress().BindUObject(
        this, &UPlayCppSdkDownloader::OnPlayDownloadProgressCallback);

    httprequest->OnProcessRequestComplete().BindUObject(
        this, &UPlayCppSdkDownloader::OnGetJsonStringFromUriCompleteCallback);
    httprequest->ProcessRequest();
    output_message = TEXT("");
}

void UPlayCppSdkDownloader::OnGetJsonStringFromUriCompleteCallback(
    FHttpRequestPtr httprequest, FHttpResponsePtr httpresponse,
    bool connectedSuccessfully) {
    FString jsonstring;
    FString result;
    if (connectedSuccessfully) {
        FString response = httpresponse->GetContentAsString();
        jsonstring = response;
        result = TEXT("");
    } else {
        switch (httprequest->GetStatus()) {
        case EHttpRequestStatus::Failed_ConnectionError:
            result = TEXT("Connection failed.");
        default:
            result = TEXT("Request failed.");
        }
    }

    GetJsonStringFromUriCallback.Broadcast(jsonstring, result);
}

void UPlayCppSdkDownloader::GetNftImageInfoFromUri(FString tokenuriuser,
                                                   bool &success,
                                                   FString &output_message) {

    if (!GetNftImageInfoFromUriCallback.IsBound()) {
        success = false;
        output_message = "GetNftImageInfoFromUriCallback is not bound";
        return;
    }

    FString tokenuri = convert_ipfs_url(tokenuriuser);
    assert(HttpModule != NULL);
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httprequest =
        HttpModule->CreateRequest();

    httprequest->SetVerb(TEXT("GET"));
    httprequest->SetHeader(TEXT("Content-Type"),
                           TEXT("application/x-www-form-urlencoded"));
    httprequest->SetHeader(TEXT("User-Agent"), UserAgent);
    httprequest->SetURL(tokenuri);

    httprequest->OnRequestProgress().BindUObject(
        this, &UPlayCppSdkDownloader::OnPlayDownloadProgressCallback);

    httprequest->OnProcessRequestComplete().BindUObject(
        this, &UPlayCppSdkDownloader::OnGetNftImageInfoFromUriCompleteCallback);
    httprequest->ProcessRequest();
    output_message = TEXT("");
}

void UPlayCppSdkDownloader::OnGetNftImageInfoFromUriCompleteCallback(
    FHttpRequestPtr httprequest, FHttpResponsePtr httpresponse,
    bool connectedSuccessfully) {

    FString result;
    FString jsonstring;
    FCronosNftInfo nftinfo;
    if (connectedSuccessfully) {
        FString response = httpresponse->GetContentAsString();
        jsonstring = response;
        TSharedRef<TJsonReader<TCHAR>> jsonreader =
            TJsonReaderFactory<TCHAR>::Create(jsonstring);
        TSharedPtr<FJsonObject> jsonobject = MakeShareable(new FJsonObject());
        if (!FJsonSerializer::Deserialize(jsonreader, jsonobject) ||
            !jsonobject.IsValid()) {
            result = TEXT("GetNftImageInfoFromUri Failed to parse json");
        } else {
            nftinfo.JsonString = jsonstring;
            jsonobject->TryGetStringField(TEXT("name"), nftinfo.Name);
            jsonobject->TryGetStringField(TEXT("description"),
                                          nftinfo.Description);
            jsonobject->TryGetStringField(TEXT("image"), nftinfo.Image);
            nftinfo.ImageUrl = convert_ipfs_url(nftinfo.Image);
            result = TEXT("");
        }
    } else {
        switch (httprequest->GetStatus()) {
        case EHttpRequestStatus::Failed_ConnectionError:
            result = TEXT("Connection failed.");
        default:
            result = TEXT("Request failed.");
        }
    }

    GetNftImageInfoFromUriCallback.Broadcast(nftinfo, result);
}

void UPlayCppSdkDownloader::GetNftImageFromUrl(FString imageurl, bool &success,
                                               FString &output_message) {

    if (!GetNftImageFromUrlCallback.IsBound()) {
        success = false;
        output_message = "GetNftImageFromUrlCallback is not bound";
        return;
    }

    assert(HttpModule != NULL);
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> httprequest =
        HttpModule->CreateRequest();
    httprequest->SetVerb(TEXT("GET"));
    httprequest->SetHeader(TEXT("User-Agent"), UserAgent);
    httprequest->SetURL(imageurl);

    httprequest->OnRequestProgress().BindUObject(
        this, &UPlayCppSdkDownloader::OnPlayDownloadProgressCallback);

    httprequest->OnProcessRequestComplete().BindUObject(
        this, &UPlayCppSdkDownloader::OnGetNftImageFromUrlCompleteCallback);
    httprequest->ProcessRequest();
    output_message = TEXT("");
}

void UPlayCppSdkDownloader::OnGetNftImageFromUrlCompleteCallback(
    FHttpRequestPtr httprequest, FHttpResponsePtr httpresponse,
    bool connectedSuccessfully) {
    UTexture2D *nftimage = NULL;
    FString result;
    if (connectedSuccessfully) {
        // dangerous. Validate imageurl and check validity
        nftimage = UKismetRenderingLibrary::ImportBufferAsTexture2D(
            NULL, httpresponse->GetContent());
        if (NULL == nftimage) {
            result = TEXT("Failed to import image.");
        } else {
            result = TEXT("");
        }
    } else {
        switch (httprequest->GetStatus()) {
        case EHttpRequestStatus::Failed_ConnectionError:
            result = TEXT("Connection failed.");
        default:
            result = TEXT("Request failed.");
        }
    }
    GetNftImageFromUrlCallback.Broadcast(nftimage, result);
}
