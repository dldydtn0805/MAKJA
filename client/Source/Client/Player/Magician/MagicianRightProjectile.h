// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicianRightProjectile.generated.h"

UCLASS()
class CLIENT_API AMagicianRightProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMagicianRightProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	class USphereComponent* CollisionComponent;

	// ������Ÿ�� �����Ʈ ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	// ������Ÿ���� �ӵ��� �߻� �������� �ʱ�ȭ��Ű�� �Լ��Դϴ�.
	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	class UParticleSystemComponent* ParticleComponent1;

	UPROPERTY(VisibleAnywhere, Category = "Effects")
	class UParticleSystemComponent* ParticleComponent2;

	UPROPERTY(VisibleAnywhere)
	class UMagicianStatComponent* MagicianStat;

	float DamageAmount;
};
