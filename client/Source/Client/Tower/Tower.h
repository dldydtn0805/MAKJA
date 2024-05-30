// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerStatComponent.h"
#include "../ClientGameInstance.h"
#include "Tower.generated.h"

UENUM()
enum class FTowerType {
	GROUND,
	AIR,
	ALL_ROUND
};


UCLASS()
class CLIENT_API ATower : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 적의 타입과 탑의 타입을 비교하여 공격 가능한지 확인
	bool CheckAttack(FTowerType TowerTyp, EnemyType EnemyTyp);

public:
	virtual void LevelUp(); // 레벨 업

	virtual void Sell(); // 판매

	// 탑의 TowerStatComponent를 반환
	UTowerStatComponent* GetTowerStatComponent() { return  TowerStat; }

	// 타워 아이디 Getter 메서드
	FString GetTowerId() { return  TowerId; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString TowerId; // 타워의 아이디

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTowerType Type; // 타워의 타입

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* Tower; // 타워 스태틱 메시 컴포넌트

	// 탑의 피벗을 기준으로 한 발사 시작점의 위치
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// 스폰시킬 프로젝타일 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class ATowerProjectile> ProjectileClass;

	// 타워의 스텟
	UTowerStatComponent* TowerStat;
};
