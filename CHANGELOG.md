# Changelog


## [v0.0.2-alpha] - 2022-07-18
- Update to play-cpp-sdk v0.0.2-alpha
- fix incorrect tokenid in Erc721Uri, Erc1155Uri
- change code-style
- add tokenuri download and texture generation
- change folder structure for unrealengine marketplace
- remove play-cpp-sdk binary, add downloading in Makefile, CI Build

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
