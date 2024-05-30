// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ClientGameInstance.h"
#include "Enemy/SpawnPoint.h"
#include "Sound/SoundWave.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "ClientGameMode.generated.h"
/**
 * 
 */
UCLASS()
class CLIENT_API AClientGameMode : public AGameMode
{
	GENERATED_BODY()
	AClientGameMode();

public:
	class UClientGameInstance* ClientGameInstance;
	virtual void BeginPlay() override;
	void StartWave();
	void EndWave();
	void GameOver();
	void GameClear();
	void DecreaseEnemy();

private:
	FWaveData* WD;
	class AClientGameState* GS;

public:
	UPROPERTY(VisibleAnywhere)
	int32 TotalWave;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentWave;

	UPROPERTY(VisibleAnywhere)
	int32 TotalEnemy;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentEnemy;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn Points", meta = (AllowPrivateAccess = "true"))
	TArray<TSoftObjectPtr<ASpawnPoint>> SpawnPoints;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundWave* BackgroundSoundWave;
	UPROPERTY()
	UAudioComponent* BackgroundMusicComponent;
};
