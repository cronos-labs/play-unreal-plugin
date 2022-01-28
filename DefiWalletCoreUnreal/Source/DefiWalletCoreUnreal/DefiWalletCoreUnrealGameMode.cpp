// Copyright Epic Games, Inc. All Rights Reserved.

#include "DefiWalletCoreUnrealGameMode.h"
#include "DefiWalletCoreUnrealCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADefiWalletCoreUnrealGameMode::ADefiWalletCoreUnrealGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
