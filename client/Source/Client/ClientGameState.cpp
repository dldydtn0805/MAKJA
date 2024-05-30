// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientGameState.h"
#include "ClientGameMode.h"
#include "Kismet/GameplayStatics.h"

AClientGameState::AClientGameState() {
	TotalWaves = 20;
	CurrentWave = 0;
	// 디버깅용
	TotalEnemy = 0;
}




int32 AClientGameState::GetCurrentWave()
{
	return CurrentWave;
}

void AClientGameState::StartNewWave() {
	if (CurrentWave < TotalWaves) {
		CurrentWave++;
		// 디버그용
		UE_LOG(LogTemp, Warning, TEXT("%i wave start"), CurrentWave);
		//DecreaseEnemy();
	}
}

void AClientGameState::DecreaseEnemy()
{
	if (TotalEnemy > 0) {
		TotalEnemy--;
	}
	else {
		AClientGameMode* GM = Cast<AClientGameMode>(UGameplayStatics::GetGameMode(this));
		if (GM) {
			GM->EndWave();
		}
	}
}
