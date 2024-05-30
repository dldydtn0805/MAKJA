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
	// Ÿ�� ���̵�� ������ ���� ���ݰ� ���
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
	void InitFromTowerId(FString TowerId); // Ÿ�� ���� �ʱ�ȭ

	UFUNCTION()
	void LevelUp(FString TowerId); // Ÿ�� ���� ��

	UFUNCTION()
	bool IsMaxLevel(); // ���� ����

	UFUNCTION()
	int32 SellCost(); // �Ǹ� ���

private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level; // Ÿ���� ����

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Attack; // ���ݷ�

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackRange; // ���� ����

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackSpeed; // ���� �ӵ�	

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Cost; // ���	

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Stable; // ����ġ	

	// �Һ��� �� ���
	int32 TotalCost = 0; 

	// �Ǹ� �� ��ȯ�� ��� �ۼ�Ʈ
	int32 SellPercentage = 70; 
};
