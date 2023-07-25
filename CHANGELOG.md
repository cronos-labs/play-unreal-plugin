# Changelog

## [Unreleased]

## [v0.0.14-alpha] - 2023-7-26
- only one build for mac
- fix Array bug in abi json

## [v0.0.13-alpha] - 2023-6-15
- Support apple arm64
- Support Unreal Engine 5.2.0
- Update play-cpp-sdk headerfiles 

## [v0.0.12-alpha] - 2023-5-04
- Support sending transactions using Metamask and Crypto.com Defi Wallet
- Use play-cpp-sdk v0.0.19-alpha
- Replace rust::cxxbridge1::Error with std::exception
- Merge CI workflows
- Fix varies crashes issues

## [v0.0.11-alpha] - 2023-3-07
- Add SecureStorageOption for Wallets for desktop, mobile
- Add merge group event on ci actions
- Fix Cpp Linter
- Add StateInit and StateRestored for Walletconnect
- Fix WalletConnectSessionInfo is not correctly constructed
- Fix Function RestoreClient is not aysnc
- Add codeowners file
- Fix Outdated Cronos Play Application Form link
- Fix Linux/Mac Makefile to cache mac/linux build


## [v0.0.10-alpha] - 2023-1-09
- Add DynamicContractObject: which is created from DefiWalletActor
- Add DynamicContractObject::Send : for modifying contract
- Add DynamicContractObject::Call : for query contract
- Add DynamicContractObject::Encode : for wallet connect
- Support Unreal Engine 5.1.0
- play-cpp-sdk v0.0.14-alpha
- Save session to a sessioninfo.json file under project dir
- Restore session info sessioninfo.json file
- Consolidate Engine Version, Platform Name, and Plugin Version Name into user agent
- Add support of walletconnect and erc-721, erc-1155, erc-20
- Add IOS support
- Add chain_id to function: SignEip155Transaction
- Rename
  - `FInitializeWalletConnectBlockingDelegate` to `FInitializeWalletConnectDelegate`
  - Rename `EnsureSessionBlocking` to `EnsureSession`
  - Add GetCryptoWalletUrl
  - Turn `SignPersonalBlocking` into `async SignPersonal`
  - Rename `SignEip155TransactionBlocking` to `SignEip155Transaction`
- Add ToHex function


## [v0.0.9-alpha] - 2022-11-01
!!! Please notice: This version introduces some breaking changes !!!
- Add `chain_id` for function `InitializeWalletConnect`
- Rename the following Functions
  - `InitializeWallet` -> `RestoreWallet`
  - `InitializeNewDevelopmentOnlyWallet` -> `InitializeNewWallet`
  - `DevelopmentOnlyGenerateMnemonics` -> `GenerateMnemonics`
  - `SendEthAmountAsync` -> `SendEthAmount`
  - `SignLegacyTransactionBlocking` -> `SignEip155TransactionBlocking`
- Rename the following Variables
  - `FWalletConnectTxLegacy` ->`FWalletConnectTxEip155`
  - `FWalletSignTXLegayResult` -> `FWalletSignTXEip155Result`
  - `FWalletconnectSignLegacyTransactionDelegate` -> `FWalletconnectSignEip155TransactionDelegate`
- Setup Default User Agent in `DefiWalletCoreActor`
- Move `BroadcastEthTxAsync` to `DefiWalletCoreActor`
- Fix DocsURL
- Use play-cpp-sdk [v0.0.9-alpha](https://github.com/cronos-labs/play-cpp-sdk/releases/tag/v0.0.9-alpha)

## [v0.0.8-alpha] - 2022-10-07
- Fix documentations

## [v0.0.7-alpha] - 2022-09-16
- Remove Blueprints Assets
- Support Android (arm64-v8a, armeabi-v7a, and x86_64)
- add erc20,721,1155 async api (transfer, approve)
- add sendEthAmountAsync

## [v0.0.6-alpha] - 2022-08-17
- Use QRCODE in play-cpp-sdk
- Support Linux

## [v0.0.4-alpha] - 2022-08-11
- Follow epic game marketplace guideline
- Add get-backup-mnemonics, generate-mnemonics


## [v0.0.3-alpha] - 2022-08-03
- Fix usermessage not being set in SignPersonalBlocking
- Support UE5
- Add install-play-cpp-sdk.bat
- Use play-cpp-sdk v0.0.3-alpha
- Add blueprint asset to plugin
- Set http agent for http downloading, cronos rpc apis ( CronosPlay-UnrealEngine-Agent )

## [v0.0.2-alpha] - 2022-07-18
- Update to play-cpp-sdk v0.0.2-alpha
- Fix incorrect tokenid in Erc721Uri, Erc1155Uri
- Change code-style
- Add tokenuri download and texture generation
- Change folder structure for unrealengine marketplace
- Remove play-cpp-sdk binary, add downloading in Makefile, CI Build

## [v0.0.1-alpha] - 2022-06-21
### Added
- Add function BroadcastEthTxAsync to broadcast eth-tx
- Add function DefiWalletCore Apis
  - Add function `InitializeWallet` to create wallet using menmonics
  - Add function `GetWallet` to get wallet info
  - Add function `GetWalletBalance` to get wallet balance
  - Add function `GetNFTOwner` to get nft owner
  - Add function `GetNFTCollecion` to get nft collection
  - Add function `GetNFTDenom` to get nft denom
  - Add function `GetNFTDenomByName` to get nft denom by name
  - Add function `GetNFTAllDenoms` to get all nft denoms
  - Add function `GetNFTToken` to get nft token
  - Add function `GetEthAddress` to get eth address
  - Add function `GetEthBalance` to get eth balance
  - Add function `SignEthAmount` to sign eth amount
  - Add function `SendEthAmount` to send eth amount
  - Add function `SignLogin` to sign login
  - Add function `VerifyLogin` to verify login
  - Add function `Erc20Balance` to get erc20 balance
  - Add function `Erc721Balance` to get erc721 balance
  - Add function `Erc1155Balance` to get erc1155 balance
  - Add function `Erc20Name` to get erc20 name
  - Add function `Erc20Symbol` to get erc20 symbol
  - Add function `Erc20Decimals` to get erc20 decimals
  - Add function `Erc20TotalSupply` to get erc20 total supply
  - Add function `Erc721Name` to get erc721 name
  - Add function `Erc721Symbol` to get erc721 symbol
  - Add function `Erc721Uri` to get erc721 uri
  - Add function `Erc721Owner` to get erc721 owner
  - Add function `Erc1155Uri` to get erc1155 uri
  - Add function `Erc1155
- Add Blockscout apis
  - Add function `GetTokensBlocking` to get tokens
  - Add function `GetTokenTransfersBlocking` to get token transfers
- Add Cronoscan apis
  - Add function `GetTransactionHistoryBlocking` to get transaction history
  - Add function `GetErc20TransferHistoryBlocking` to get erc20 transfer history
  - Add function `GetErc721TransferHistoryBlocking` to get erc721 transfer history
- WalledtConnect apis
  - Add function `InitializeWalletConnect` to initialize wallet-connect client
  - Add function `EnsureSessionBlocking` to ensure session
  - Add function `SetupCallback` to setup callback
  - Add function `GetConnectionString` to get qr-code string
  - Add function `SaveClient` to save session information as string
  - Add function `RestoreClient` to restore session information from string
  - Add function `SignPersonalBlocking` to sign general message
  - Add function `SignLegacyTransactionBlocking` to sign legacy tx
