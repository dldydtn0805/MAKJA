// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicianRProjectile.generated.h"

UCLASS()
class CLIENT_API AMagicianRProjectile : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMagicianRProjectile();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
        class USphereComponent* CollisionComponent;

    // 발사체의 움직임을 지정해 주는 변수
    UPROPERTY(VisibleAnywhere, Category = Movement)
        class UProjectileMovementComponent* ProjectileMovementComponent;

    // 발사체의 발사 방향을 지정해주는 함수
    UFUNCTION()
        void FireDirection(const FVector& ShootDirection);

    UFUNCTION()
        void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UPROPERTY(VisibleAnywhere, Category = "Effects")
        class UParticleSystemComponent* ParticleComponent1;

    UPROPERTY(VisibleAnywhere, Category = "Effects")
        class UParticleSystemComponent* ParticleComponent2;

    float DamageAmount;
};
