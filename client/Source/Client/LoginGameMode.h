// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "LoginGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API ALoginGameMode : public AGameModeBase
{
	GENERATED_BODY()
	ALoginGameMode();

public:
	virtual void BeginPlay() override;

public:
	TSubclassOf<UUserWidget> LoginWidgetClass;
	TSubclassOf<UUserWidget> SignUpWidgetClass;
	TSubclassOf<UUserWidget> GameExitWidgetClass;

	class ULoginWidget* LoginWidget;
	class USignUpWidget* SignUpWidget;
	class UGameExitConfirmWidget* GameExitConfirmWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundWave* BackgroundSoundWave;
	UPROPERTY()
	UAudioComponent* BackgroundMusicComponent;
	


};
