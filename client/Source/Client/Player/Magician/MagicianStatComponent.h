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
	// 캐릭터 목숨 갱신
	void SetLife(int32 NewLife);
	// 현재 레벨에 따른 캐릭터 스탯 적용
	void SetMagicianStat(int32 NewLevel);
	// 캐릭터 레벨 갱신
	void SetLevel();
	// 레벨업, 피격 등으로 인한 현재 남은 Hp 적용
	void SetHp(int32 NewHp);
	// 보유 자금 갱신 함수
	void PlusMoney(int32 MoneyToAdd);
	void MinusMoney(int32 MoneyToDelete);

	// 현재 레벨에 따른 무기 스탯 적용
	void SetWeaponStat(int32 NewLevel);
	// 무기 레벨 갱신
	void SetWeaponLevelUp();
	void SetWeaponLevelDown();

	// 피격 함수 
	void OnAttacked(float DamageAmount);

	// Magician Stat Data Table 
	int32 GetLife() { return Life; }
	int32 GetLevel() { return Level; }						// 현재 Level 반환
	int32 GetMaxHp() { return MaxHp; }						// 현재 Level에 대한 최대 체력 반환
	int32 GetHp() { return Hp; }							// 현재 남은 Hp 반환
	float GetHpRatio() { return Hp / float(MaxHp); }
	bool GetLevelUpAvailable() { return LevelUpAvailable; } // LevelUp에 드는 LevelUpCost 반환
	int32 GetLevelUpCost() { return LevelUpCost; }			// LevelUp에 드는 LevelUpCost 반환
	int32 GetMoney() { return Money; }						// Money 반환

	// Magician Skill Data Table
	float GetLeftDamage() { return LeftDamage; }			// 현재 Level에서 Left Skill 데미지 반환 
	float GetLeftCoolTime() { return LeftCoolTime; }		// 현재 Level에서 Left Skill 쿨타임 반환 
	float GetRightDamage() { return RightDamage; }			// 현재 Level에서 Right Skill 데미지 반환 
	float GetRightCoolTime() { return RightCoolTime; }		// 현재 Level에서 Right Skill 쿨타임 반환 
	float GetShiftDamage() { return ShiftDamage; }			// 현재 Level에서 Shift Skill 데미지 반환 
	float GetShiftCoolTime() { return ShiftCoolTime; }		// 현재 Level에서 Shift Skill 쿨타임 반환 	
	float GetQDamage() { return QDamage; }					// 현재 Level에서 Q Skill 데미지 반환 
	float GetQCoolTime() { return QCoolTime; }				// 현재 Level에서 Q Skill 쿨타임 반환 	
	float GetEDamage() { return EDamage; }					// 현재 Level에서 E Skill 데미지 반환 
	float GetECoolTime() { return ECoolTime; }				// 현재 Level에서 E Skill 쿨타임 반환 
	float GetRDamage() { return RDamage; }					// 현재 Level에서 R Skill 데미지 반환 
	float GetRCoolTime() { return RCoolTime; }				// 현재 Level에서 R Skill 쿨타임 반환 
	
	// Magician Weapon Data Table
	int32 GetWeaponLevel() { return WeaponLevel; }
	float GetAttackCoefficient() { return AttackCoefficient; }
	bool GetWeaponLevelUpAvailable() { return WeaponLevelUpAvailable; }
	int32 GetWeaponLevelUpCost() { return WeaponLevelUpCost; }
	float GetWeaponLevelUpPercentage() { return WeaponLevelUpPercentage; }


	// 각 스킬 별 남은 쿨타임 관리 변수
	FTimerHandle TimerHandle_CooldownLeft;
	FTimerHandle TimerHandle_CooldownRight;
	FTimerHandle TimerHandle_CooldownShift;

	FTimerHandle TimerHandle_CooldownQ;
	FTimerHandle TimerHandle_CooldownE;
	FTimerHandle TimerHandle_CooldownR;

	// 각 스킬 별 사용 가능 여부 및 쿨타임 관리 함수
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
	// 캐릭터 스탯 변수
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

	// 스킬 별 데미지 및 쿨타임 변수
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




	// 캐릭터 무기 스탯 변수
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
