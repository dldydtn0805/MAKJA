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

	// 쿨타임
	float Cooltime = 1.f / TowerStat->GetAttackSpeed();

	// 쿨타임이 지날 때 마다 공격할 수 있도록 타이머 설정
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

// 적이 있는지 체크
void AAOETower::CheckEnemy()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false);
	FVector CurrentPos = GetActorLocation();
	FVector Center = CurrentPos + FVector(0.f, 0.f, 500.f);

	bool CatchEnemy = false; // 적 감지 여부

	// 필요한 스텟 가져오기
	float AttackRange = TowerStat->GetAttackRange();
	float Cooltime = 1.f / TowerStat->GetAttackSpeed();

	// 탑의 공격 범위에 들어온 오브젝트가 있는지 확인
	bool bResult = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel4,
		FCollisionShape::MakeCapsule(AttackRange, AttackRange + 800.f),
		QueryParams
	);

	if (bResult) { // 감지된게 있으면
		// 감지된것들을 확인
		for (auto& OverlapResult : OverlapResults)
		{
			AEnemy* Enemy = Cast<AEnemy>(OverlapResult.GetActor());
			if (Enemy && Enemy->Stat->GetHp() > 0 && CheckAttack(Type, Enemy->Stat->GetType())) { // 적이 감지됐고 해당 탑이 때릴 수 있틑 적이면 공격 개시
				CatchEnemy = true;
				break;
			}
		}
	}

	// 적이 감지됐는제 공격 중인 상태가 아니면 공격을 실시
	if (CatchEnemy && !isAttack) {
		StartAttack();
	}

	// 적이 감지되지 않았는데 공격 중인 상태면 공격을 중지
	else if (!CatchEnemy && isAttack) {
		StopAttack();
	}

	// 공격 중이라면 공격을 한다.
	else if (isAttack) {
		DoAttack(OverlapResults);
	}
}

void AAOETower::StartAttack()
{
	UE_LOG(LogTemp, Error, TEXT("Attack Start!"));

	// 공격 범위
	float AttackRange = TowerStat->GetAttackRange();

	// 공격 이펙트 On
	// 동시에 파티클 컴포넌트 저장
	if (SpawnedAttackParticle == nullptr) {
		UParticleSystemComponent* SpawnedParticle = SpawnAttackParticle(AttackRange);
		SpawnedAttackParticle = SpawnedParticle;
		
		// 효과음 재생
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

	// 감지 목록을 확인해서 적에게 데미지를 준다.
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
	// 공격 이펙트가 활성화되어 있다면 제거해준다.
	DestroyAttackParticle();

	// 효과음이 재생중이라면 재생 중지
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

// 레벨 업
void AAOETower::LevelUp()
{
	Super::LevelUp();

	// 새로운 쿨타임
	float NewCooltime = 1.f / TowerStat->GetAttackSpeed();

	// 이전 쿨타임
	float BeforeCooltime = GetWorld()->GetTimerManager().GetTimerRate(TimerHandle);

	// 쿨타임 갱신이 필요하면 갱신
	if (BeforeCooltime != NewCooltime) {
		float RemainTime = GetWorldTimerManager().GetTimerElapsed(TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AAOETower::CheckEnemy, NewCooltime, true, (RemainTime > 0 ? RemainTime : 0.f));
	}

	// 현재 활성화된 파티클이 있으면 새로운 공격 범위에 맞춰서 파티클을 소환 후 기존 파티클은 제거한다.
	if (SpawnedAttackParticle) {
		DestroyAttackParticle();

		float NewAttackRange = TowerStat->GetAttackRange();
		UParticleSystemComponent* NewSpawnedParticle = SpawnAttackParticle(NewAttackRange);
		SpawnedAttackParticle = NewSpawnedParticle;
	}
}

void AAOETower::Sell()
{
	// 활성화된 파티클이 있으면 제거 한다.
	DestroyAttackParticle();

	// 효과음이 재생중이라면 재생 중지
	/*if (SpawnSoundComponent->IsPlaying()) {
		SpawnSoundComponent->Stop();
	}*/

	Super::Sell();
}
