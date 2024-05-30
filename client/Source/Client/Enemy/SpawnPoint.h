// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.h"
#include "SpawnPoint.generated.h"

UCLASS()
class CLIENT_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnPoint();
	void Spawn(TSubclassOf<AEnemy> Enemy, int32 EnemyID, int32 Count);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
};
