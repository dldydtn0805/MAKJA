// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleTower.h"
#include "DrawDebugHelpers.h"
#include "../Enemy/Enemy.h"
#include "TowerProjectile.h"
#include "../ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASingleTower::ASingleTower()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ASingleTower::BeginPlay()
{
	Super::BeginPlay();

	// 쿨타임
	float Cooltime = 1.f / TowerStat->GetAttackSpeed();
	UE_LOG(LogTemp, Error, TEXT("%d %d"), TowerStat->GetAttackSpeed(), TowerStat->GetAttackRange())

	// 쿨타임이 지날 때 마다 공격할 수 있도록 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASingleTower::CheckEnemy, Cooltime, true, 0.f);
}

void ASingleTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 적이 있는지 체크
void ASingleTower::CheckEnemy()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false);
	FVector CurrentPos = GetActorLocation();
	FVector Center = CurrentPos + FVector(0.f, 0.f, 500.f);

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
			// 적에 해당하는 클래스로 캐스팅 시도
			AEnemy* Enemy = Cast<AEnemy>(OverlapResult.GetActor());
			if (Enemy && Enemy->Stat->GetHp() > 0 && CheckAttack(Type, Enemy->Stat->GetType())) { // 적이 감지됐고 해당 탑이 때릴 수 있틑 적이면 공격 개시
				Fire(Enemy);
				return;
			}
		}
	}
}

// 타겟을 향해 발사체를 발사
void ASingleTower::Fire(APawn* Target)
{
	// 발사체 클래스가 있으면 발사 진행
	if (ProjectileClass) {
		// 발사 시작 위치
		FVector MuzzleLocation = GetActorLocation() + MuzzleOffset;

		// 총구 위치에 발사체를 스폰
		FTransform SpawnTransform = FTransform(MuzzleLocation);
		ATowerProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATowerProjectile>(ProjectileClass, SpawnTransform, this);
		if (Projectile)
		{
			// 발사체에 탑의 공격력을 담아준다.
			int32 TowerAttack = TowerStat->GetAttack();
			int32 ToweStable = TowerStat->GetStable();
			Projectile->Attack = FMath::RandRange(
				TowerAttack * ToweStable / 100 + (TowerAttack * ToweStable % 100 > 0),
				TowerAttack
			);

			// 맵에 발사체를 스폰
			Projectile->FinishSpawning(SpawnTransform);

			// 발사 위치 계산 후 발사
			FVector ShootDir = Target->GetActorLocation() - MuzzleLocation;
			ShootDir.Normalize();
			Projectile->FireInDirection(ShootDir);
		}
	}
}

// 레벨 업
void ASingleTower::LevelUp()
{
	Super::LevelUp();

	// 새로운 쿨타임
	float NewCooltime = 1.f / TowerStat->GetAttackSpeed();
	// 이전 쿨타임
	float BeforeCooltime = GetWorld()->GetTimerManager().GetTimerRate(TimerHandle);

	// 쿨타임 갱신이 필요하면 갱신
	if (BeforeCooltime != NewCooltime) {
		float RemainTime = GetWorldTimerManager().GetTimerElapsed(TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASingleTower::CheckEnemy, NewCooltime, true, (RemainTime > 0 ? RemainTime : 0.f));
	}
}
