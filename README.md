# Cronos Play Unreal SDK
Cronos Play Unreal SDK is a plugin for Unreal Engine that allows interacting with blockchain
networks in a self-custodial way.

## Security Warning

No security audits of this project have ever been performed yet.

The project is still in development and is *alpha* quality. 

USE AT YOUR OWN RISK!

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
4. Navigate in this dialog: Class Search -> Native Module -> Click + -> **CronosPlayUnreal**
5. Click on "Generate Docs"
    - location: **CronosPlaySdk/Saved/KantanDocGen/**


### Windows
Make sure you use Visual Studio 2019 or later.(2022) 
it's already coded, if any compiling errors occur, add these line to top of c++ file
```c++
#pragma warning(disable:4583)
#pragma warning(disable:4582)
```


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


### How to compile
1. install unreal engine 4.27
2. git clone this repository
3. open CronosPlaySdk/CronosPlaySdk.uproject 

### How to install
1. git clone this repository, and run `cd CronosPlaySdk` & copydll.sh or copydll.bat to copy dynamic libraries for ue4 editor
2. mkdir `Plugins` folder in your project
3. copy ./CronosPlaySdk/Plugins/CronosPlayUnreal `yourproject/Plugins/` , it's automatically detected in the project
4. also copydll.sh or copydll.bat to the project folder, and run it to copy dynamic libraries for ue4 editor


### How to start coding
1. click `add blueprint` in ue4 menu
2. inherit DefiWalletCoreActor or PlayCppSdkActor     
3. drag & drop your class to the scene
4. category is **CronosPlayUnreal**
5. apis in `..Blueprint.h` is stateless, can be called any place in the blueprint

### More information for Cronos Play
If you are a game developer, please visit [Cronos Play](https://cronos.org/play) or fill this [Contact Form](https://airtable.com/shrFiQnLrcpeBp2lS) for more information.
