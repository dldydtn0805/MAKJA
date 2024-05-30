// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerProjectile.generated.h"

UCLASS()
class CLIENT_API ATowerProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATowerProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 발사체가 충돌을 감지하면 호출
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 발사체를 지정한 발사 방향으로 발사
	void FireInDirection(const FVector& ShootDir);

protected:
	// 발사체 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float ProjectileSpeed;

	// 색
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	FColor ProjectileColor;

	// 색
	FVector ColorVector;

	// 구체 콜리전 컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	class USphereComponent* CollisionComponent;

	// 프로젝타일 무브먼트 컴포넌트
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	// 프로젝타일 파티클 컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	class UParticleSystemComponent* ProjectileParticleComponent;

	// 발사 소리가 담길 오디오 컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	class UAudioComponent* ShootSoundComponent;

	// 발사 될 때 나타나는 파티클
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	class UParticleSystem* MuzzleParticle;

	// 누군가 피격시 나타날 파티클
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	class UParticleSystem* DamageParticle;

	// 발사 효과음
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	class USoundWave* ShootSoundWave;

	// 소리 배율
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	float AudioVolumeMultiplier = 1.0f;

public:
	// 발사체의 공격력
	UPROPERTY()
	int32 Attack;
};
