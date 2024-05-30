// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TowerStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLIENT_API UTowerStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTowerStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// 타워 아이디와 레벨에 따른 스텟값 얻기
	void GetStatFromTowerIdAndLevel(FString TowerId, int32 NewLevel);

	// Setter
	int32 GetLevel() { return Level; }
	int32 GetAttack() { return Attack; }
	float GetAttackRange() { return AttackRange; }
	float GetAttackSpeed() { return AttackSpeed; }
	int32 GetCost() { return Cost; }
	int32 GetStable() { return Stable; }

public:
	UFUNCTION()
	void InitFromTowerId(FString TowerId); // 타워 스텟 초기화

	UFUNCTION()
	void LevelUp(FString TowerId); // 타워 레벨 업

	UFUNCTION()
	bool IsMaxLevel(); // 만렙 여부

	UFUNCTION()
	int32 SellCost(); // 판매 비용

private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level; // 타워의 레벨

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Attack; // 공격력

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackRange; // 공격 범위

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackSpeed; // 공격 속도	

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Cost; // 비용	

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Stable; // 안정치	

	// 소비한 총 비용
	int32 TotalCost = 0; 

	// 판매 시 반환될 비용 퍼센트
	int32 SellPercentage = 70; 
};
