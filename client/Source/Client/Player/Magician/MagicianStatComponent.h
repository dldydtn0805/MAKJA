// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MagicianStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpChanged);
DECLARE_MULTICAST_DELEGATE(FOnPlayerLevelChanged);
DECLARE_MULTICAST_DELEGATE(FOnWeaponLevelChanged);
DECLARE_MULTICAST_DELEGATE(FOnMoneyChanged);
DECLARE_MULTICAST_DELEGATE(FOnLifeChanged);
DECLARE_MULTICAST_DELEGATE(FOnNoLifeChanged);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRightCoolTimeChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnShiftCoolTimeChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQCoolTimeChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnECoolTimeChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnRCoolTimeChanged, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLIENT_API UMagicianStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMagicianStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// ĳ���� ��� ����
	void SetLife(int32 NewLife);
	// ���� ������ ���� ĳ���� ���� ����
	void SetMagicianStat(int32 NewLevel);
	// ĳ���� ���� ����
	void SetLevel();
	// ������, �ǰ� ������ ���� ���� ���� Hp ����
	void SetHp(int32 NewHp);
	// ���� �ڱ� ���� �Լ�
	void PlusMoney(int32 MoneyToAdd);
	void MinusMoney(int32 MoneyToDelete);

	// ���� ������ ���� ���� ���� ����
	void SetWeaponStat(int32 NewLevel);
	// ���� ���� ����
	void SetWeaponLevelUp();
	void SetWeaponLevelDown();

	// �ǰ� �Լ� 
	void OnAttacked(float DamageAmount);

	// Magician Stat Data Table 
	int32 GetLife() { return Life; }
	int32 GetLevel() { return Level; }						// ���� Level ��ȯ
	int32 GetMaxHp() { return MaxHp; }						// ���� Level�� ���� �ִ� ü�� ��ȯ
	int32 GetHp() { return Hp; }							// ���� ���� Hp ��ȯ
	float GetHpRatio() { return Hp / float(MaxHp); }
	bool GetLevelUpAvailable() { return LevelUpAvailable; } // LevelUp�� ��� LevelUpCost ��ȯ
	int32 GetLevelUpCost() { return LevelUpCost; }			// LevelUp�� ��� LevelUpCost ��ȯ
	int32 GetMoney() { return Money; }						// Money ��ȯ

	// Magician Skill Data Table
	float GetLeftDamage() { return LeftDamage; }			// ���� Level���� Left Skill ������ ��ȯ 
	float GetLeftCoolTime() { return LeftCoolTime; }		// ���� Level���� Left Skill ��Ÿ�� ��ȯ 
	float GetRightDamage() { return RightDamage; }			// ���� Level���� Right Skill ������ ��ȯ 
	float GetRightCoolTime() { return RightCoolTime; }		// ���� Level���� Right Skill ��Ÿ�� ��ȯ 
	float GetShiftDamage() { return ShiftDamage; }			// ���� Level���� Shift Skill ������ ��ȯ 
	float GetShiftCoolTime() { return ShiftCoolTime; }		// ���� Level���� Shift Skill ��Ÿ�� ��ȯ 	
	float GetQDamage() { return QDamage; }					// ���� Level���� Q Skill ������ ��ȯ 
	float GetQCoolTime() { return QCoolTime; }				// ���� Level���� Q Skill ��Ÿ�� ��ȯ 	
	float GetEDamage() { return EDamage; }					// ���� Level���� E Skill ������ ��ȯ 
	float GetECoolTime() { return ECoolTime; }				// ���� Level���� E Skill ��Ÿ�� ��ȯ 
	float GetRDamage() { return RDamage; }					// ���� Level���� R Skill ������ ��ȯ 
	float GetRCoolTime() { return RCoolTime; }				// ���� Level���� R Skill ��Ÿ�� ��ȯ 
	
	// Magician Weapon Data Table
	int32 GetWeaponLevel() { return WeaponLevel; }
	float GetAttackCoefficient() { return AttackCoefficient; }
	bool GetWeaponLevelUpAvailable() { return WeaponLevelUpAvailable; }
	int32 GetWeaponLevelUpCost() { return WeaponLevelUpCost; }
	float GetWeaponLevelUpPercentage() { return WeaponLevelUpPercentage; }


	// �� ��ų �� ���� ��Ÿ�� ���� ����
	FTimerHandle TimerHandle_CooldownLeft;
	FTimerHandle TimerHandle_CooldownRight;
	FTimerHandle TimerHandle_CooldownShift;

	FTimerHandle TimerHandle_CooldownQ;
	FTimerHandle TimerHandle_CooldownE;
	FTimerHandle TimerHandle_CooldownR;

	// �� ��ų �� ��� ���� ���� �� ��Ÿ�� ���� �Լ�
	bool IsSkillReady(FName SkillName);
	void ActivateSkill(FName SkillName);

	UFUNCTION(BlueprintCallable)
	void BroadcastRightCoolTime();
	UFUNCTION(BlueprintCallable)
	void BroadcastShiftCoolTime();
	UFUNCTION(BlueprintCallable)
	void BroadcastQCoolTime();
	UFUNCTION(BlueprintCallable)
	void BroadcastECoolTime();
	UFUNCTION(BlueprintCallable)
	void BroadcastRCoolTime();
private:
	// ĳ���� ���� ����
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	int32 Life;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	int32 Level;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	int32 MaxHp;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	int32 Hp;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	bool LevelUpAvailable;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	int32 LevelUpCost;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	int32 Money;

	// ��ų �� ������ �� ��Ÿ�� ����
	// Left
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	float LeftDamage;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	float LeftCoolTime;

	// Right
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	float RightDamage;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	float RightCoolTime;
	
	// Shift
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	float ShiftDamage;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	float ShiftCoolTime;
	
	// Q
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	float QDamage;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	float QCoolTime;
	
	// E
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	float EDamage;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	float ECoolTime;
	
	// R
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	float RDamage;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
	float RCoolTime;




	// ĳ���� ���� ���� ����
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
		int32 WeaponLevel;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
		float AttackCoefficient;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
		bool WeaponLevelUpAvailable;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
		int32 WeaponLevelUpCost;
	UPROPERTY(EditAnywhere, Category = PlayerStat, Meta = (AllowPrivateAccess = true))
		float WeaponLevelUpPercentage;
public:
	FOnHpChanged OnHpChanged;
	FOnPlayerLevelChanged OnPlayerLevelChanged;
	FOnWeaponLevelChanged OnWeaponLevelChanged;
	FOnRightCoolTimeChanged OnRightCoolTimeChanged;
	FOnShiftCoolTimeChanged OnShiftCoolTimeChanged;
	FOnQCoolTimeChanged OnQCoolTimeChanged;
	FOnECoolTimeChanged OnECoolTimeChanged;
	FOnRCoolTimeChanged OnRCoolTimeChanged;
	FOnMoneyChanged OnMoneyChanged;
	FOnLifeChanged OnLifeChanged;

	FOnNoLifeChanged OnNoLifeChanged;
};
