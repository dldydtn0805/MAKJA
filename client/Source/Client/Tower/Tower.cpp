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

	// Tower 오브젝트 생성 후 루트 컴포넌트로 지정
	Tower = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TOWER"));
	SetRootComponent(Tower);

	// 스텟 컴포넌트 가져오기
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

// 적의 타입과 탑의 타입을 비교하여 공격 가능한지 확인
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


