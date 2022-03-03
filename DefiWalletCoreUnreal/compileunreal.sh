export PATH="/Users/Shared/Epic Games/UE_4.27/Engine/Binaries/ThirdParty/Mono/Mac/bin":$PATH
export DOTNETPATH="/Users/Shared/Epic Games/UE_4.27/Engine/Binaries/DotNET"
export UE_MONO_DIR="/Users/Shared/Epic Games/UE_4.27/Engine/Binaries/ThirdParty/Mono/Mac"

"mono" "$DOTNETPATH/UnrealBuildTool.exe"  DefiWalletCoreUnreal Mac Development -Project=$PWD/DefiWalletCoreUnreal.uproject 
