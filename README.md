# Cronos Play Unreal SDK Demo
This repository contains a sample project that uses the Cronos Play Unreal SDK plugin.
For more information, please see the [Cronos Play Unreal SDK plugin repository](https://github.com/crypto-com/play-unreal-plugin).

## Pre-requisites
Windows: Visual Studo 2019 or later
Mac: XCODE 13.2.1
Android: NDK 21.4.7075529
Unreal Engine: 4.27 or 5.x

## Pre-built Release
Download the release zip file in [Release](https://github.com/cronos-labs/play-unreal-demo/releases) page.

### `CronosPlayDemo.zip`
- Assets: `./Content/CronosPlayUnreal/Blueprint`
- `CronosPlayUnreal` Plugin: `./Plugins/play-unreal-plugin`

### `CronosPlayAssetPack.zip`
- Assets: Blueprints, widgets etc

## Setup Manually
If you prefer configuring the project manually:
1. Clone this project
2. Under the project root and run `make` to setup `CronosPlayUnreal` plugin
3. open `CronosPlayUnrealDemo.uproject`
4. For android, you need to setup the key for signing, please check [here](https://docs.unrealengine.com/4.27/en-US/SharingAndReleasing/Mobile/Android/DistributionSigning/).

### More information for Cronos Play
If you are a game developer, please visit [Cronos Play](https://cronos.org/play) or fill this [Contact Form](https://airtable.com/shrFiQnLrcpeBp2lS) for more information.
