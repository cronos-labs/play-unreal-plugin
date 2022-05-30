# defi-wallet-core unreal engine plugin
## compile guide
compile is supported unreal build system (c#)
1. run copydll.sh or copydll.bat (important)
2. click DefiWalletCoreUnreal.project in file finder or explorer
3. it will be built automatically

## for documentation
1. use windows pc
2. run gitshell
3. run getdoc.sh
4. enable KantanDocGenPlugin in plugins 
5. File -> KantanDocGen 
6. Class Search -> Native Module -> Clcik + -> DefiWalletCore
7. Click Generate Docs

##  mac
- xcode 13.2.1 
- for rust frameworks to support ue5
```
export MACOSX_DEPLOYMENT_TARGET=10.15
cargo build ..
```
- check deployment target
```
otool -l libdefi_wallet_core_cpp.dylib > out
vi out
check LC_BUILD_VERSION/minos is 10.15
```

## windows
visual studo 2019 


