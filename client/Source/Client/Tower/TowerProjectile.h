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
	// �߻�ü�� �浹�� �����ϸ� ȣ��
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// �߻�ü�� ������ �߻� �������� �߻�
	void FireInDirection(const FVector& ShootDir);

protected:
	// �߻�ü �ӵ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	float ProjectileSpeed;

	// ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	FColor ProjectileColor;

	// ��
	FVector ColorVector;

	// ��ü �ݸ��� ������Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	class USphereComponent* CollisionComponent;

	// ������Ÿ�� �����Ʈ ������Ʈ
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	// ������Ÿ�� ��ƼŬ ������Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	class UParticleSystemComponent* ProjectileParticleComponent;

	// �߻� �Ҹ��� ��� ����� ������Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Movement)
	class UAudioComponent* ShootSoundComponent;

	// �߻� �� �� ��Ÿ���� ��ƼŬ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	class UParticleSystem* MuzzleParticle;

	// ������ �ǰݽ� ��Ÿ�� ��ƼŬ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Projectile)
	class UParticleSystem* DamageParticle;

	// �߻� ȿ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	class USoundWave* ShootSoundWave;

	// �Ҹ� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	float AudioVolumeMultiplier = 1.0f;

public:
	// �߻�ü�� ���ݷ�
	UPROPERTY()
	int32 Attack;
};
