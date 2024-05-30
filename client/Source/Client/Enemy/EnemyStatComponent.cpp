// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatComponent.h"
#include "../ClientGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UEnemyStatComponent::UEnemyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UEnemyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetEnemyStat(1);
}

void UEnemyStatComponent::SetEnemyStat(int32 EnemyID) {
	UClientGameInstance* CGI = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	AClientGameMode* GM = Cast<AClientGameMode>(GetWorld()->GetAuthGameMode());
	int32 WaveNumber = 1;
	if (GM) {
		WaveNumber = GM->CurrentWave;
	}
	if (CGI) {
		auto StatData = CGI->GetEnemyStatData(EnemyID);
		if (StatData) {
			Level = WaveNumber;
			SetHp((StatData->HpRatio) * Level * 100);
			MaxHp = (StatData->HpRatio) * Level * 100;
			
			EnemyName = StatData->EnemyEnName;
			EnemyKrName = StatData->EnemyKrName;
			type = StatData->type;
			AR_Regist = StatData->ARRatio * Level;
			MR_Regist = StatData->MRRatio * Level;
			Speed = StatData->Speed;

			AttackDmg = StatData->DmgRatio * Level;
			AttackRange = StatData->AttackRange;
			AttackSpeed = StatData->AttackSpeed;
		}
	}
}
void UEnemyStatComponent::SetHp(int32 NewHp)
{
	Hp = NewHp;
	if (Hp < 0)
		Hp = 0;
	OnHpChanged.Broadcast();
}

void UEnemyStatComponent::OnAttacked(float DamageAmount)
{
	int32 NewHp = Hp - DamageAmount;
	SetHp(NewHp);
}

