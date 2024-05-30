// Fill out your copyright notice in the Description page of Project Settings.

#include "Tower.h"
#include "DrawDebugHelpers.h"
#include "../Enemy/Enemy.h"
#include "TowerProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATower::ATower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Tower ������Ʈ ���� �� ��Ʈ ������Ʈ�� ����
	Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TOWER"));
	SetRootComponent(Tower);

	// ���� ������Ʈ ��������
	TowerStat = CreateDefaultSubobject<UTowerStatComponent>(TEXT("STAT"));
}

void ATower::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("check %d"), TowerStat == nullptr)
	TowerStat->InitFromTowerId(TowerId);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATower::LevelUp()
{
	if (!TowerStat->IsMaxLevel()) {
		TowerStat->LevelUp(TowerId);
	}
}

void ATower::Sell()
{
	Destroy();
}

// ���� Ÿ�԰� ž�� Ÿ���� ���Ͽ� ���� �������� Ȯ��
bool ATower::CheckAttack(FTowerType TowerTyp, EnemyType EnemyTyp)
{
	switch (TowerTyp) {
	case FTowerType::GROUND:
		return EnemyTyp == EnemyType::GROUND || EnemyTyp == EnemyType::BOSS;
	case FTowerType::AIR:
		return EnemyTyp == EnemyType::AIR || EnemyTyp == EnemyType::BOSS;
	case FTowerType::ALL_ROUND:
		return true;
	}
	return false;
}


