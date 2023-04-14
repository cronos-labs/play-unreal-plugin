// Copyright 2022, Cronos Labs. All Rights Reserved

#include "DynamicContractObject.h"

UDynamicContractObject::UDynamicContractObject() {
    defiWallet = NULL;
    _coreContract = NULL;
}

void UDynamicContractObject::BeginDestroy() {
    Super::BeginDestroy();

    UE_LOG(LogTemp, Log, TEXT("UDynamicContractObject  BeginDestroy"));
    DestroyContract();

    assert(NULL == _coreContract);
}

void UDynamicContractObject::DestroyContract() {

    UE_LOG(LogTemp, Log, TEXT("UDynamicContractObject  DestroyContract"));

    if (_coreContract != NULL) {
        UE_LOG(LogTemp, Log,
               TEXT("UDynamicContractObject  Removed CoreContract"));
        // restored back
        rust::cxxbridge1::Box<org::defi_wallet_core::EthContract> tmpcontract =
            rust::cxxbridge1::Box<org::defi_wallet_core::EthContract>::from_raw(
                _coreContract);
        _coreContract = NULL;
    }
}

FString UDynamicContractObject::DynamicContractReadJson(
    FString filepath, FString keyname, bool &success, FString &output_message) {

    FString ret = TEXT("");
    try {
        std::string myfilepath = TCHAR_TO_UTF8(*filepath);
        std::string mykeyname = TCHAR_TO_UTF8(*keyname);
        rust::String abijson = read_json(myfilepath, mykeyname);
        ret = UTF8_TO_TCHAR(abijson.c_str());
        success = true;
    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("PlayCppSdk DynamicContractReadJson Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }

    return ret;
}

void UDynamicContractObject::EncodeDynamicContract(FString functionName,
                                                   FString functionArgs,
                                                   TArray<uint8> &output,
                                                   bool &success,
                                                   FString &output_message) {
    try {

        if (NULL == _coreContract) {
            success = false;
            output_message = TEXT("Invalid Contract");
            return;
        }

        std::string myfunctionname = TCHAR_TO_UTF8(*functionName);
        std::string myfunctionargs = TCHAR_TO_UTF8(*functionArgs);

        rust::Vec<uint8_t> data =
            _coreContract->encode(myfunctionname, myfunctionargs);

        output.SetNum(data.size());
        for (int i = 0; i < data.size(); i++) {
            output[i] = data[i];
        }

        success = true;

    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("PlayCppSdk EncodeDynamicContract Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}

void UDynamicContractObject::CallDynamicContract(
    FString functionName, FString functionArgs,
    FCallDynamicContractDelegate Out) {

    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out,

                                                        functionName,
                                                        functionArgs]() {
        FString jsonoutput;
        FString result;
        try {

            if (NULL == _coreContract) {
                result = TEXT("Invalid Contract");

            } else {
                std::string myfunctionname = TCHAR_TO_UTF8(*functionName);
                std::string myfunctionargs = TCHAR_TO_UTF8(*functionArgs);
                std::string response =
                    _coreContract->call(myfunctionname, myfunctionargs).c_str();
                jsonoutput = UTF8_TO_TCHAR(response.c_str());
            }
        } catch (const std::exception &e) {
            result = FString::Printf(
                TEXT("CronosPlayUnreal CallDynamicContract Error: %s"),
                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, jsonoutput, result]() {
            Out.ExecuteIfBound(jsonoutput, result);
        });
    });
}

void UDynamicContractObject::SendDynamicContract(
    FString functionName, FString functionArgs,
    FDynamicContractSendDelegate Out) {
    AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [this, Out, functionName,
                                                        functionArgs]() {
        FString result;
        FCronosTransactionReceiptRaw txresult;

        try {
            if (NULL == _coreContract) {
                result = TEXT("Invalid Contract");
            } else {
                std::string myfunctionname = TCHAR_TO_UTF8(*functionName);
                std::string myfunctionargs = TCHAR_TO_UTF8(*functionArgs);

                assert(_coreContract != NULL);
                CronosTransactionReceiptRaw receipt =
                    _coreContract->send(myfunctionname, myfunctionargs);

                convertCronosTXReceipt(receipt, txresult);
            }
        } catch (const std::exception &e) {
            result = FString::Printf(
                TEXT("CronosPlayUnreal SendDynamicContract Error: %s"),
                UTF8_TO_TCHAR(e.what()));
        }

        AsyncTask(ENamedThreads::GameThread, [Out, txresult, result]() {
            Out.ExecuteIfBound(txresult, result);
        });
    });
}

void UDynamicContractObject::NewSigningEthContract(FString contractaddress,
                                                   FString abijson,
                                                   int32 walletindex,
                                                   bool &success,
                                                   FString &output_message) {
    try {
        if (NULL != _coreContract) {
            success = false;
            output_message = TEXT("Contract Already Exists");
            return;
        }

        if (NULL == defiWallet) {
            success = false;
            output_message = TEXT("Wallet Not Linked");
            return;
        }

        assert(defiWallet != NULL);
        if (NULL == defiWallet->getCoreWallet()) {
            success = false;
            output_message = TEXT("Wallet Not Initialized");
            return;
        }

        char hdpath[100];
        snprintf(hdpath, sizeof(hdpath), "m/44'/%d'/0'/0/%d", EthCoinType,
                 walletindex);
        rust::cxxbridge1::Box<PrivateKey> privatekey =
            defiWallet->getCoreWallet()->get_key(hdpath);

        assert(defiWallet != NULL);
        std::string mycronosrpc = TCHAR_TO_UTF8(*defiWallet->myCronosRpc);
        int32 chainid = defiWallet->myCronosChainID;

        std::string mycontract = TCHAR_TO_UTF8(*contractaddress);
        std::string myjson = TCHAR_TO_UTF8(*abijson);

        rust::cxxbridge1::Box<EthContract> tmpContract =
            new_signing_eth_contract(mycronosrpc, mycontract, myjson,
                                     *privatekey, chainid);
        // ownership transferred
        _coreContract = tmpContract.into_raw();

        success = true;
    } catch (const std::exception &e) {
        success = false;
        output_message = FString::Printf(
            TEXT("CronosPlayUnreal NewSigningEthContract Error: %s"),
            UTF8_TO_TCHAR(e.what()));
    }
}

void UDynamicContractObject::NewEthContract(FString contractaddress,
                                            FString abijson, bool &success,
                                            FString &output_message) {
    try {
        if (NULL != _coreContract) {
            success = false;
            output_message = TEXT("Contract Already Exists");
            return;
        }

        if (NULL == defiWallet) {
            success = false;
            output_message = TEXT("Wallet Not Linked");
            return;
        }

        assert(defiWallet != NULL);
        std::string mycronosrpc = TCHAR_TO_UTF8(*defiWallet->myCronosRpc);
        std::string mycontract = TCHAR_TO_UTF8(*contractaddress);
        std::string myjson = TCHAR_TO_UTF8(*abijson);

        rust::cxxbridge1::Box<EthContract> tmpContract =
            new_eth_contract(mycronosrpc, mycontract, myjson);
        // ownership transferred
        _coreContract = tmpContract.into_raw();

        success = true;
    } catch (const std::exception &e) {
        success = false;
        output_message =
            FString::Printf(TEXT("CronosPlayUnreal NewEthContract Error: %s"),
                            UTF8_TO_TCHAR(e.what()));
    }
}