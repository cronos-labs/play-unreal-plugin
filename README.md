# Cronos Play Unreal SDK
Cronos Play Unreal SDK is a plugin for Unreal Engine that allows interacting with blockchain
networks in a self-custodial way.

## Pre-requisites
Unreal Engine 4.27 and 5.0 are supported, i.e. one of them should be installed via the Epic Games Launcher.

## Building
Building is supported via the [Unreal Build System](https://docs.unrealengine.com/4.27/en-US/ProductionPipelines/UnrealBuildSystem/) (C#):

1. Run copydll.sh or copydll.bat (important)
2. Click on the `.project` file
3. The project will be built automatically

### Documentation Generation
Currently, this is supported on Windows via [KantanDocGen Plugin](https://github.com/kamrann/KantanDocGenPlugin):

1. Run getdoc.sh inside [Git shell](https://gitforwindows.org)
2. Enable KantanDocGenPlugin in plugins 
3. Open the plugin dialog: File -> KantanDocGen 
4. Navigate in this dialog: Class Search -> Native Module -> Click + -> (project name)
5. Click on "Generate Docs"

### Windows
Make sure you use Visual Studio 2019. 

###  macOS
Make sure you use Xcode 13.2.1.
If you are building the C++ SDK bindings and the corresponding binaries from Rust for Unreal Engine 5,
then make sure to set a deployment target for the [Cronos Play C++ SDK](https://github.com/crypto-com/play-cpp-sdk):

```bash
export MACOSX_DEPLOYMENT_TARGET=10.15
cargo build ..
```

You can check the deployment target using the `otool`:
```bash
otool -l <resulting binary>.dylib
```

The `LC_BUILD_VERSION/minos` should be 10.15.


