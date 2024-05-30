// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicianStatComponent.h"
#include "../../ClientGameInstance.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
// Sets default values for this component's properties
UMagicianStatComponent::UMagicianStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;


	// 변수 초기화(초기 세팅)
	Life = 3;
	Level = 1;
	WeaponLevel = 1;
	Money = 5000;

	/*Hp = 100;
	LevelUpAvailable = true;
	LevelUpCost = 100;

	LeftDamage = 20.0f;
	LeftCoolTime = 0.0f;
	RightDamage = 10.0f;
	RightCoolTime = 2.0f;
	ShiftDamage = 0.0f;
	ShiftCoolTime = 0.5f;
	QDamage = 40.0f;
	QCoolTime = 3.0f;
	EDamage = 30.0f;
	ECoolTime = 5.0f;
	RDamage = 100.0f;
	RCoolTime = 10.0f;*/
}


// Called when the game starts
void UMagicianStatComponent::BeginPlay()
{
	Super::BeginPlay();
	SetLife(Life);
	SetMagicianStat(Level);
	SetWeaponStat(WeaponLevel);
	PlusMoney(0);
	// ...

}

void UMagicianStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
}


// Called every frame
void UMagicianStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}


// 현재 레벨에 따른 마법사 캐릭터의 스탯 및 스킬 갱신
void UMagicianStatComponent::SetMagicianStat(int32 NewLevel)
{
	UE_LOG(LogTemp, Log, TEXT("Change Money"))
	UE_LOG(LogTemp, Log, TEXT("Change 23131"))
	auto ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (ClientGameInstance)
	{
		auto MagicianStatData = ClientGameInstance->GetCharacterStatData(NewLevel);
		auto MagicianSkillData = ClientGameInstance->GetCharacterSkillData(NewLevel);
		if (MagicianStatData && MagicianSkillData)
		{
			// 변수 초기화(초기 세팅)
			Level = MagicianStatData->Level;
			SetHp(MagicianStatData->MaxHp);
			MaxHp = MagicianStatData->MaxHp;
			LevelUpAvailable = MagicianStatData->LevelUpAvailable;
			LevelUpCost = MagicianStatData->LevelUpCost;

			LeftDamage = MagicianSkillData->LeftDamage;
			LeftCoolTime = MagicianSkillData->LeftCoolTime;
			RightDamage = MagicianSkillData->RightDamage;
			RightCoolTime = MagicianSkillData->RightCoolTime;
			ShiftDamage = MagicianSkillData->ShiftDamage;
			ShiftCoolTime = MagicianSkillData->ShiftCoolTime;
			QDamage = MagicianSkillData->QDamage;
			QCoolTime = MagicianSkillData->QCoolTime;
			EDamage = MagicianSkillData->EDamage;
			ECoolTime = MagicianSkillData->ECoolTime;
			RDamage = MagicianSkillData->RDamage;
			RCoolTime = MagicianSkillData->RCoolTime;

			OnPlayerLevelChanged.Broadcast();
			UE_LOG(LogTemp, Warning, TEXT("Set Magician Stat Data!"));
			UE_LOG(LogTemp, Warning, TEXT("MaxHp : %d"), MaxHp);
			UE_LOG(LogTemp, Warning, TEXT("Hp : %d"), Hp);


			UE_LOG(LogTemp, Warning, TEXT("LeftDamage : %f"), LeftDamage);
			UE_LOG(LogTemp, Warning, TEXT("RightDamage : %f"), RightDamage);
			UE_LOG(LogTemp, Warning, TEXT("QDamage : %f"), QDamage);
			UE_LOG(LogTemp, Warning, TEXT("EDamage : %f"), EDamage);
			UE_LOG(LogTemp, Warning, TEXT("RDamage : %f"), RDamage);
			OnHpChanged.Broadcast();
		}
	}
}
void UMagicianStatComponent::SetLife(int32 NewLife)
{	
	Life = NewLife;
	OnLifeChanged.Broadcast();

	UE_LOG(LogTemp, Log, TEXT("NewLife : %d"), NewLife);
	if (NewLife > 0)
	{		
		UE_LOG(LogTemp, Warning, TEXT("Game Restart : Life Left %d"), NewLife);
		//OnNoLifeChanged.Broadcast(); // 디버그용으로 임시임
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Game End : No Life Left"));
		OnNoLifeChanged.Broadcast();
	}
}
void UMagicianStatComponent::SetLevel()
{
	if (LevelUpAvailable)
	{
		if (Money - LevelUpCost >= 0)
		{
			SetMagicianStat(Level + 1);
			MinusMoney(-LevelUpCost);
			UE_LOG(LogTemp, Error, TEXT("MoneyLeft : %d"), Money);
		}
	}
}


void UMagicianStatComponent::SetHp(int32 NewHp)
{
	if (NewHp < 0)
	{
		Hp = 0;
		UE_LOG(LogTemp, Error, TEXT("Hp Now : 0"));
		// SetLife(Life - 1);
	}
	else
	{
		Hp = NewHp;
		UE_LOG(LogTemp, Error, TEXT("Hp Now : %d"), Hp);

	}
	OnHpChanged.Broadcast();
}

void UMagicianStatComponent::PlusMoney(int32 MoneyToAdd)
{
	Money += MoneyToAdd;
	OnMoneyChanged.Broadcast();
}

void UMagicianStatComponent::MinusMoney(int32 MoneyToDelete)
{
	if (Money + MoneyToDelete >= 0)
	{
		Money += MoneyToDelete;
		OnMoneyChanged.Broadcast();
	}
}

void UMagicianStatComponent::SetWeaponStat(int32 NewLevel)
{
	auto ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (ClientGameInstance)
	{

		auto MagicianWeaponData = ClientGameInstance->GetCharacterWeaponData(NewLevel);

		if (MagicianWeaponData)
		{
			// 변수 초기화(초기 세팅)
			WeaponLevel = MagicianWeaponData->WeaponLevel;
			AttackCoefficient = MagicianWeaponData->AttackCoefficient;
			WeaponLevelUpAvailable = MagicianWeaponData->WeaponLevelUpAvailable;
			WeaponLevelUpCost = MagicianWeaponData->WeaponLevelUpCost;
			WeaponLevelUpPercentage = MagicianWeaponData->WeaponLevelUpPercentage;
			OnWeaponLevelChanged.Broadcast();

			// OnPlayerLevelChanged.Broadcast();
			UE_LOG(LogTemp, Warning, TEXT("Set Magician Weapon Data!"));
			UE_LOG(LogTemp, Warning, TEXT("AttackCoefficient : %f"), AttackCoefficient);

		}
	}
}

void UMagicianStatComponent::SetWeaponLevelUp()
{
	if (WeaponLevelUpAvailable)
	{
		if (Money - WeaponLevelUpCost >= 0)
		{
			SetWeaponStat(WeaponLevel + 1);
		}
	}
}
void UMagicianStatComponent::SetWeaponLevelDown()
{
	if (WeaponLevel - 1 > 0)
	{
		SetWeaponStat(WeaponLevel - 1);
	}
}

void UMagicianStatComponent::OnAttacked(float DamageAmount)
{
	UE_LOG(LogTemp, Error, TEXT("OnAttacked : %f"), DamageAmount);
	int32 NewHp = int32(Hp - DamageAmount);
	SetHp(NewHp);
}

bool UMagicianStatComponent::IsSkillReady(FName SkillName)
{
	FTimerHandle* TimerHandle = nullptr;

	if (SkillName == "Left" || SkillName == "Right" || SkillName == "Shift" || SkillName == "Q" || SkillName == "E" || SkillName == "R")
	{
		if (SkillName == "Left") TimerHandle = &TimerHandle_CooldownLeft;
		else if (SkillName == "Right") TimerHandle = &TimerHandle_CooldownRight;
		else if (SkillName == "Shift") TimerHandle = &TimerHandle_CooldownShift;
		else if (SkillName == "Q") TimerHandle = &TimerHandle_CooldownQ;
		else if (SkillName == "E") TimerHandle = &TimerHandle_CooldownE;
		else if (SkillName == "R") TimerHandle = &TimerHandle_CooldownR;
	}
	if (TimerHandle != nullptr)
	{
		FTimerManager& TimerManager = GetWorld()->GetTimerManager();
		// 타이머가 활성화되어 있는지 확인
			// 스킬 실행 이후 쿨타임이 돌고 있을 경우 false, 쿨타임이 종료되었을 경우(스킬 재사용 가능) true
		bool bIsTimerActive = TimerManager.IsTimerActive(*TimerHandle);

		// 타이머가 완료될 때까지 남은 시간
		// 다음 스킬 사용 가능까지 남은 시간
		// 스킬 사용 가능인 경우 - 1
		float TimeRemaining = TimerManager.GetTimerRemaining(*TimerHandle);

		// 타이머의 전체 기간
		// 해당 스킬을 사용한 이후 시간
		// 스킬 사용 가능인 경우 -1 
		float TimeElapsed = TimerManager.GetTimerElapsed(*TimerHandle);

		UE_LOG(LogTemp, Error, TEXT("Timer is active: %s"), bIsTimerActive ? TEXT("Yes") : TEXT("No"));
		UE_LOG(LogTemp, Error, TEXT("Time remaining: %f"), TimeRemaining);
		UE_LOG(LogTemp, Error, TEXT("Time elapsed: %f"), TimeElapsed);

		return TimerHandle && !GetWorld()->GetTimerManager().IsTimerActive(*TimerHandle);
	}

	return false;
}

void UMagicianStatComponent::ActivateSkill(FName SkillName)
{

	FTimerHandle* TimerHandle = nullptr;
	float CoolTime = 0.0f;

	if (SkillName == "Left" || SkillName == "Right" || SkillName == "Shift" || SkillName == "Q" || SkillName == "E" || SkillName == "R")
	{
		if (SkillName == "Left")
		{
			TimerHandle = &TimerHandle_CooldownLeft;
			CoolTime = LeftCoolTime;

		}
		else if (SkillName == "Right")
		{
			TimerHandle = &TimerHandle_CooldownRight;
			CoolTime = RightCoolTime;

		}
		else if (SkillName == "Shift")
		{
			TimerHandle = &TimerHandle_CooldownShift;
			CoolTime = ShiftCoolTime;

		}
		else if (SkillName == "Q")
		{
			TimerHandle = &TimerHandle_CooldownQ;
			CoolTime = QCoolTime;

		}
		else if (SkillName == "E")
		{
			TimerHandle = &TimerHandle_CooldownE;
			CoolTime = ECoolTime;

		}
		else if (SkillName == "R")
		{
			TimerHandle = &TimerHandle_CooldownR;
			CoolTime = RCoolTime;
		}
	}
	if (TimerHandle && !GetWorld()->GetTimerManager().IsTimerActive(*TimerHandle))
	{
		if (SkillName == "Right")
		{
			OnRightCoolTimeChanged.Broadcast(CoolTime);
			GetWorld()->GetTimerManager().SetTimer(*TimerHandle, this, &UMagicianStatComponent::BroadcastRightCoolTime, CoolTime, false);
		}

		else if (SkillName == "Shift")
		{
			OnShiftCoolTimeChanged.Broadcast(CoolTime);
			GetWorld()->GetTimerManager().SetTimer(*TimerHandle, this, &UMagicianStatComponent::BroadcastShiftCoolTime, CoolTime, false);
		}
		else if (SkillName == "Q")
		{
			OnQCoolTimeChanged.Broadcast(CoolTime);
			GetWorld()->GetTimerManager().SetTimer(*TimerHandle, this, &UMagicianStatComponent::BroadcastQCoolTime, CoolTime, false);
		}
		else if (SkillName == "E")
		{
			OnECoolTimeChanged.Broadcast(CoolTime);
			GetWorld()->GetTimerManager().SetTimer(*TimerHandle, this, &UMagicianStatComponent::BroadcastECoolTime, CoolTime, false);
		}
		else if (SkillName == "R")
		{
			OnRCoolTimeChanged.Broadcast(CoolTime);
			GetWorld()->GetTimerManager().SetTimer(*TimerHandle, this, &UMagicianStatComponent::BroadcastRCoolTime, CoolTime, false);
		}
	}
}

void UMagicianStatComponent::BroadcastRightCoolTime()
{
	OnRightCoolTimeChanged.Broadcast(0.0f);
}
void UMagicianStatComponent::BroadcastShiftCoolTime()
{
	OnShiftCoolTimeChanged.Broadcast(0.0f);
}
void UMagicianStatComponent::BroadcastQCoolTime()
{
	OnQCoolTimeChanged.Broadcast(0.0f);
}
void UMagicianStatComponent::BroadcastECoolTime()
{
	OnECoolTimeChanged.Broadcast(0.0f);
}

void UMagicianStatComponent::BroadcastRCoolTime()
{
	OnRCoolTimeChanged.Broadcast(0.0f);
}

