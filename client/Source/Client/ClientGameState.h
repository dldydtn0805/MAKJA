// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ClientGameState.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API AClientGameState : public AGameState
{
	GENERATED_BODY()
	AClientGameState();

public:
	int32 GetCurrentWave();
	void StartNewWave();
	void DecreaseEnemy();

private:
	UPROPERTY(VisibleAnywhere)
	int32 TotalWaves;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentWave;

	UPROPERTY(VisibleAnywhere)
	int32 TotalEnemy;
};
