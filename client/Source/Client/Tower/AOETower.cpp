// Fill out your copyright notice in the Description page of Project Settings.

#include "AOETower.h"
#include "DrawDebugHelpers.h"
#include "../Enemy/Enemy.h"
#include "TowerProjectile.h"
#include "../ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"

// Sets default values
AAOETower::AAOETower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAOETower::BeginPlay()
{
	Super::BeginPlay();

	// ��Ÿ��
	float Cooltime = 1.f / TowerStat->GetAttackSpeed();

	// ��Ÿ���� ���� �� ���� ������ �� �ֵ��� Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAOETower::CheckEnemy, Cooltime, true, 0.f);

	/*if (SpawnSoundWave) {
		SpawnSoundComponent = UGameplayStatics::SpawnSound2D(this, SpawnSoundWave);
		if (SpawnSoundComponent) {
			SpawnSoundComponent->SetVolumeMultiplier(AudioVolumeMultiplier);
			SpawnSoundComponent->SetPitchMultiplier(AudioPitchMultiplier);
		}
	}*/
}

void AAOETower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ���� �ִ��� üũ
void AAOETower::CheckEnemy()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false);
	FVector CurrentPos = GetActorLocation();
	FVector Center = CurrentPos + FVector(0.f, 0.f, 500.f);

	bool CatchEnemy = false; // �� ���� ����

	// �ʿ��� ���� ��������
	float AttackRange = TowerStat->GetAttackRange();
	float Cooltime = 1.f / TowerStat->GetAttackSpeed();

	// ž�� ���� ������ ���� ������Ʈ�� �ִ��� Ȯ��
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeCapsule(AttackRange, AttackRange + 800.f),
		QueryParams
	);

	if (bResult) { // �����Ȱ� ������
		// �����Ȱ͵��� Ȯ��
		for (auto& OverlapResult : OverlapResults)
		{
			AEnemy* Enemy = Cast<AEnemy>(OverlapResult.GetActor());
			if (Enemy && Enemy->Stat->GetHp() > 0 && CheckAttack(Type, Enemy->Stat->GetType())) { // ���� �����ư� �ش� ž�� ���� �� �ֺz ���̸� ���� ����
				CatchEnemy = true;
				break;
			}
		}
	}

	// ���� �����ƴ��� ���� ���� ���°� �ƴϸ� ������ �ǽ�
	if (CatchEnemy && !isAttack) {
		StartAttack();
	}

	// ���� �������� �ʾҴµ� ���� ���� ���¸� ������ ����
	else if (!CatchEnemy && isAttack) {
		StopAttack();
	}

	// ���� ���̶�� ������ �Ѵ�.
	else if (isAttack) {
		DoAttack(OverlapResults);
	}
}

void AAOETower::StartAttack()
{
	UE_LOG(LogTemp, Error, TEXT("Attack Start!"));

	// ���� ����
	float AttackRange = TowerStat->GetAttackRange();

	// ���� ����Ʈ On
	// ���ÿ� ��ƼŬ ������Ʈ ����
	if (SpawnedAttackParticle == nullptr) {
		UParticleSystemComponent* SpawnedParticle = SpawnAttackParticle(AttackRange);
		SpawnedAttackParticle = SpawnedParticle;
		
		// ȿ���� ���
		/*if (SpawnSoundComponent && !SpawnSoundComponent->IsPlaying()) {
			SpawnSoundComponent->Play();
		}*/
	}

	isAttack = true;
}

void AAOETower::DoAttack(TArray<FOverlapResult> OverlapResults)
{
	UE_LOG(LogTemp, Error, TEXT("Attack!"));
	
	int32 TowerAttack = TowerStat->GetAttack();
	int32 ToweStable = TowerStat->GetStable();

	// ���� ����� Ȯ���ؼ� ������ �������� �ش�.
	for (auto& OverlapResult : OverlapResults)
	{
		AEnemy* Enemy = Cast<AEnemy>(OverlapResult.GetActor());
		if (Enemy && Enemy->Stat->GetHp() > 0 && CheckAttack(Type, Enemy->Stat->GetType())) {
			int32 Damage = FMath::RandRange(
				TowerAttack * ToweStable / 100 + (TowerAttack * ToweStable % 100 > 0),
				TowerAttack
			);
			FDamageEvent DamageEvent;
			Enemy->TakeDamage(Damage, DamageEvent, GetInstigatorController(), this);
		}

	}
}

void AAOETower::StopAttack()
{
	UE_LOG(LogTemp, Error, TEXT("Attack Stop!"));
	// ���� ����Ʈ�� Ȱ��ȭ�Ǿ� �ִٸ� �������ش�.
	DestroyAttackParticle();

	// ȿ������ ������̶�� ��� ����
	/*if (SpawnSoundComponent && SpawnSoundComponent->IsPlaying()) {
		SpawnSoundComponent->Stop();
	}*/

	isAttack = false;
}

UParticleSystemComponent* AAOETower::SpawnAttackParticle(float AttackRange)
{
	FVector ScaleVec = FVector(AttackRange / ScalePerRange.X, AttackRange / ScalePerRange.Y, AttackRange / ScalePerRange.Z);
	UParticleSystemComponent* SpawnedParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackParticle, GetActorLocation() + MuzzleOffset, FRotator::ZeroRotator, ScaleVec);
	return SpawnedParticle;
}

void AAOETower::DestroyAttackParticle()
{
	if (SpawnedAttackParticle)
	{
		UE_LOG(LogTemp, Error, TEXT("%d"), SpawnedAttackParticle == nullptr)
		SpawnedAttackParticle->DestroyComponent();
		UE_LOG(LogTemp, Error, TEXT("%d"), SpawnedAttackParticle == nullptr)
		SpawnedAttackParticle = nullptr;
		UE_LOG(LogTemp, Error, TEXT("%d"), SpawnedAttackParticle == nullptr)
	}
}

// ���� ��
void AAOETower::LevelUp()
{
	Super::LevelUp();

	// ���ο� ��Ÿ��
	float NewCooltime = 1.f / TowerStat->GetAttackSpeed();

	// ���� ��Ÿ��
	float BeforeCooltime = GetWorld()->GetTimerManager().GetTimerRate(TimerHandle);

	// ��Ÿ�� ������ �ʿ��ϸ� ����
	if (BeforeCooltime != NewCooltime) {
		float RemainTime = GetWorldTimerManager().GetTimerElapsed(TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAOETower::CheckEnemy, NewCooltime, true, (RemainTime > 0 ? RemainTime : 0.f));
	}

	// ���� Ȱ��ȭ�� ��ƼŬ�� ������ ���ο� ���� ������ ���缭 ��ƼŬ�� ��ȯ �� ���� ��ƼŬ�� �����Ѵ�.
	if (SpawnedAttackParticle) {
		DestroyAttackParticle();

		float NewAttackRange = TowerStat->GetAttackRange();
		UParticleSystemComponent* NewSpawnedParticle = SpawnAttackParticle(NewAttackRange);
		SpawnedAttackParticle = NewSpawnedParticle;
	}
}

void AAOETower::Sell()
{
	// Ȱ��ȭ�� ��ƼŬ�� ������ ���� �Ѵ�.
	DestroyAttackParticle();

	// ȿ������ ������̶�� ��� ����
	/*if (SpawnSoundComponent->IsPlaying()) {
		SpawnSoundComponent->Stop();
	}*/

	Super::Sell();
}
