// Copyright Epic Games, Inc. All Rights Reserved.


#include "ClientGameModeBase.h"
#include "./Player/Magician/MagicianCharacter.h"

AClientGameModeBase::AClientGameModeBase()
{
	DefaultPawnClass = AMagicianCharacter::StaticClass();

	static ConstructorHelpers::FClassFinder<ACharacter> BP_Char(TEXT("Blueprint'/Game/BluePrint/Player/Magician/BP_MagicianCharacter.BP_MagicianCharacter_C'")); // _C를 붙인다

	if (BP_Char.Succeeded())
	{
		DefaultPawnClass = BP_Char.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("Blueprint'/Game/BluePrint/Player/Controller/BP_ClientPlayerController.BP_ClientPlayerController_C'"));

	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}