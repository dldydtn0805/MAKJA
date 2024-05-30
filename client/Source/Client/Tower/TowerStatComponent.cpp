// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerStatComponent.h"
#include "../ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTowerStatComponent::UTowerStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTowerStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

// 레벨에 따른 스텟값 얻기
void UTowerStatComponent::GetStatFromTowerIdAndLevel(FString TowerId, int32 NewLevel)
{
	auto ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (ClientGameInstance) {
		auto TowerStatData = ClientGameInstance->GetTowerStatData(TowerId, NewLevel);
		if (TowerStatData) {
			Level = TowerStatData->Level;
			Attack = TowerStatData->Attack;
			AttackRange = TowerStatData->AttackRange;
			AttackSpeed = TowerStatData->AttackSpeed;
			Cost = (IsMaxLevel() ? 0 : ClientGameInstance->GetTowerStatData(TowerId, NewLevel+1)->Cost);
			Stable = TowerStatData->Stable;
		}
	}
}

void UTowerStatComponent::InitFromTowerId(FString TowerId)
{
	Level = 1;

	auto ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	TotalCost += ClientGameInstance->GetTowerStatData(TowerId, 1)->Cost;

	GetStatFromTowerIdAndLevel(TowerId, Level);
}

// 타워 레벨 업
void UTowerStatComponent::LevelUp(FString TowerId)
{
	TotalCost += Cost;
	GetStatFromTowerIdAndLevel(TowerId, ++Level);
}

// 만렙 여부
bool UTowerStatComponent::IsMaxLevel()
{
	return Level == 10;
}

int32 UTowerStatComponent::SellCost()
{
	return TotalCost * SellPercentage / 100;
}

