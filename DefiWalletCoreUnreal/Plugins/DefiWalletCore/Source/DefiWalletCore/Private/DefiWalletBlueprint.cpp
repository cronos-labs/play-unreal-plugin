#include "DefiWalletBlueprint.h"

#include <iostream>
#include <sstream>

#include "TxBuilder.h"
#include "cxx.h"
#include "lib.rs.h"
using namespace std;
using namespace org::defi_wallet_core;
string to_hex(const unsigned char *data, int len);

void UDefiWalletBlueprint::BroadcastEthTxAsync(FWalletBroadcastDelegate Out,
                                               TArray<uint8> usersignedtx,
                                               FString rpc) {
  AsyncTask(ENamedThreads::AnyHiPriThreadNormalTask, [Out, usersignedtx,
                                                      rpc]() {
    rust::Vec<::std::uint8_t> signedtx;
    copy(usersignedtx.begin(), usersignedtx.end(),
         std::back_inserter(signedtx));
    assert(signedtx.size() == usersignedtx.size());

    string mycronosrpc = TCHAR_TO_UTF8(*rpc);

    FString txhashtext;
    FString result;
    try {
      ::org::defi_wallet_core::CronosTransactionReceiptRaw receipt =
          broadcast_eth_signed_raw_tx(signedtx, mycronosrpc);

      std::string txhash = to_hex(receipt.transaction_hash.data(),
                                  receipt.transaction_hash.size());

      txhashtext = UTF8_TO_TCHAR(txhash.c_str());
    } catch (const rust::cxxbridge1::Error &e) {
      result =
          FString::Printf(TEXT("DefiWalletCore BroadcastEthTxAsync Error: %s"),
                          UTF8_TO_TCHAR(e.what()));
    }

    AsyncTask(ENamedThreads::GameThread, [Out, txhashtext, result]() {
      Out.ExecuteIfBound(txhashtext, result);
    });
  });
}
