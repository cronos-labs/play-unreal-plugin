# Cronos Play for Unreal Engine
Cronos Play for Unreal Engine is a plugin for Unreal Engine that allows interacting with
blockchain networks in a self-custodial way.

![Icon128.png](./Resources/Icon128.png)

## Security Warning

No security audits of this project have ever been performed yet.

The project is still in development and is *alpha* quality.

USE AT YOUR OWN RISK!

## Pre-requisites
- Unreal Engine 4.27 and 5.0 are supported, i.e. one of them should be installed via the Epic
  Games Launcher.
- Windows: Visual Studio 2019 or newer
- MacOS: Xcode 13.2.1

## Building

### Step 1: Download the release zip file in [Release](https://github.com/cronos-labs/play-unreal-plugin/releases) page
Skip this step if you prefer building from source.

### Step 2:  Install [play-cpp-sdk](https://github.com/cronos-labs/play-cpp-sdk)
Skip this step if you prefer building from the release zip file.

- Step 2a: Clone this project
- Step 2b: run `make` (mac) or run `install-play-cpp-sdk.bat` (windows) to setup [play-cpp-sdk](https://github.com/cronos-labs/play-cpp-sdk).

### Step 3: Run RunUAT.bat[or RunUAT.sh] script to build the plugin
``` bash
Engine\Build\BatchFiles\RunUAT.bat[or RunUAT.sh] BuildPlugin -Plugin=[Path to .uplugin file, must be outside engine directory] -Package=[Output directory] -Rocket
```

### Windows
Make sure you use Visual Studio 2019 or newer.

###  macOS
Make sure you use Xcode 13.2.1 (Xcode 13.3 or newer has unused variables errors when compiling
the unreal source codes, until the issue is fixed by Epic Games).

## More information about Cronos Play
If you are a game developer, please visit [Cronos Play](https://cronos.org/play) or fill this
[Contact Form](https://airtable.com/shrFiQnLrcpeBp2lS) for more information.
