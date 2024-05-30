// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyStatComponent.h"
#include "Enemy.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyHit, AActor*);

UCLASS()
class CLIENT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Attack();
	void AttackCheck();

	void Hit();

	void Die();

	FOnAttackEnd OnAttackEnd;
	FOnEnemyHit OnEnemyHit;

	UFUNCTION()
	void OnEnemyMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY()
	class UEnemyAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	bool IsBeingHit = false;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	bool IsDying = false;

	UPROPERTY()
	int32 AttackIdx = 0;

	UPROPERTY()
	int32 HitIdx = 0;

	UPROPERTY()
	int32 DeathIdx = 0;


public:
	UPROPERTY(VisibleAnywhere)
	UEnemyStatComponent* Stat;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* EnemyUI;
};
