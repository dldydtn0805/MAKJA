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

	// ��Ÿ��
	float Cooltime = 1.f / TowerStat->GetAttackSpeed();
	UE_LOG(LogTemp, Error, TEXT("%d %d"), TowerStat->GetAttackSpeed(), TowerStat->GetAttackRange())

	// ��Ÿ���� ���� �� ���� ������ �� �ֵ��� Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASingleTower::CheckEnemy, Cooltime, true, 0.f);
}

void ASingleTower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ���� �ִ��� üũ
void ASingleTower::CheckEnemy()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false);
	FVector CurrentPos = GetActorLocation();
	FVector Center = CurrentPos + FVector(0.f, 0.f, 500.f);

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
			// ���� �ش��ϴ� Ŭ������ ĳ���� �õ�
			AEnemy* Enemy = Cast<AEnemy>(OverlapResult.GetActor());
			if (Enemy && Enemy->Stat->GetHp() > 0 && CheckAttack(Type, Enemy->Stat->GetType())) { // ���� �����ư� �ش� ž�� ���� �� �ֺz ���̸� ���� ����
				Fire(Enemy);
				return;
			}
		}
	}
}

// Ÿ���� ���� �߻�ü�� �߻�
void ASingleTower::Fire(APawn* Target)
{
	// �߻�ü Ŭ������ ������ �߻� ����
	if (ProjectileClass) {
		// �߻� ���� ��ġ
		FVector MuzzleLocation = GetActorLocation() + MuzzleOffset;

		// �ѱ� ��ġ�� �߻�ü�� ����
		FTransform SpawnTransform = FTransform(MuzzleLocation);
		ATowerProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATowerProjectile>(ProjectileClass, SpawnTransform, this);
		if (Projectile)
		{
			// �߻�ü�� ž�� ���ݷ��� ����ش�.
			int32 TowerAttack = TowerStat->GetAttack();
			int32 ToweStable = TowerStat->GetStable();
			Projectile->Attack = FMath::RandRange(
				TowerAttack * ToweStable / 100 + (TowerAttack * ToweStable % 100 > 0),
				TowerAttack
			);

			// �ʿ� �߻�ü�� ����
			Projectile->FinishSpawning(SpawnTransform);

			// �߻� ��ġ ��� �� �߻�
			FVector ShootDir = Target->GetActorLocation() - MuzzleLocation;
			ShootDir.Normalize();
			Projectile->FireInDirection(ShootDir);
		}
	}
}

// ���� ��
void ASingleTower::LevelUp()
{
	Super::LevelUp();

	// ���ο� ��Ÿ��
	float NewCooltime = 1.f / TowerStat->GetAttackSpeed();
	// ���� ��Ÿ��
	float BeforeCooltime = GetWorld()->GetTimerManager().GetTimerRate(TimerHandle);

	// ��Ÿ�� ������ �ʿ��ϸ� ����
	if (BeforeCooltime != NewCooltime) {
		float RemainTime = GetWorldTimerManager().GetTimerElapsed(TimerHandle);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ASingleTower::CheckEnemy, NewCooltime, true, (RemainTime > 0 ? RemainTime : 0.f));
	}
}
