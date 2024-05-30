// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "SingleTower.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API ASingleTower : public ATower
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASingleTower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void CheckEnemy(); // ���� �ִ��� üũ

	UFUNCTION()
	void Fire(APawn* Target); // ����

public:
	virtual void LevelUp() override; // ���� ��

private:
	FTimerHandle TimerHandle;
};
