// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../EnumTypes.h"
#include "Components/ActorComponent.h"
#include "EnemyStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHpChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CLIENT_API UEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	void SetEnemyStat(int32 EnemyID);
	void SetHp(int32 NewHp);
	void OnAttacked(float DamageAmount);
	
	FString GetName() { return EnemyName; }
	FString GetKrName() { return EnemyKrName; }
	int32 GetLevel() { return Level; }
	int32 GetHp() { return Hp; }
	int32 GetMaxHp() { return MaxHp; }
	float GetHpRatio() { return Hp / float(MaxHp); }
	EnemyType GetType() { return type; }
	float GetSpeed() { return Speed; }
	float GetAttackDmg() { return AttackDmg; }
	float GetAttackRange() { return AttackRange; } 
	float GetAttackSpeed() { return AttackSpeed;  }


private:	
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Hp;
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 MaxHp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true, DisplayName = "Enemy EN Name"))
	FString EnemyName; // Monster name(EN)

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true, DisplayName = "Enemy KR Name"))
	FString EnemyKrName; // Monster name(KR)

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	EnemyType type; // Monster type

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AR_Regist; // AD Resist Ratio in float

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float MR_Regist; // Magic Resist Ratio in float

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float Speed; // Speed in float

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackDmg; // DMG in float

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackRange; // Attack range in float

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	float AttackSpeed; // Attack speed in float

public:
	FOnHpChanged OnHpChanged;
};
