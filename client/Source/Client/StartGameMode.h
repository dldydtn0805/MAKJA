// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "StartGameMode.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API AStartGameMode : public AGameModeBase
{
	GENERATED_BODY()
	AStartGameMode();

public:
	virtual void BeginPlay() override;

private:
	TSubclassOf<UUserWidget> StartLevelWidgetClass;
	class UStartLevelWidget* StartLevelWidget;
public:
	TSubclassOf<UUserWidget> GameExitWidgetClass;
	class UGameExitConfirmWidget* GameExitConfirmWidget;
public:
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundWave* BackgroundSoundWave;
	UPROPERTY()
	UAudioComponent* BackgroundMusicComponent;
};
