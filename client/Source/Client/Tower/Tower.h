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
	// ���� Ÿ�԰� ž�� Ÿ���� ���Ͽ� ���� �������� Ȯ��
	bool CheckAttack(FTowerType TowerTyp, EnemyType EnemyTyp);

public:
	virtual void LevelUp(); // ���� ��

	virtual void Sell(); // �Ǹ�

	// ž�� TowerStatComponent�� ��ȯ
	UTowerStatComponent* GetTowerStatComponent() { return  TowerStat; }

	// Ÿ�� ���̵� Getter �޼���
	FString GetTowerId() { return  TowerId; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString TowerId; // Ÿ���� ���̵�

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FTowerType Type; // Ÿ���� Ÿ��

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* Tower; // Ÿ�� ����ƽ �޽� ������Ʈ

	// ž�� �ǹ��� �������� �� �߻� �������� ��ġ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	// ������ų ������Ÿ�� Ŭ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	TSubclassOf<class ATowerProjectile> ProjectileClass;

	// Ÿ���� ����
	UTowerStatComponent* TowerStat;
};
