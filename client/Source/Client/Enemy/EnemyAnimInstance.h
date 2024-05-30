// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy.h"
#include "EnemyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEnemyAttackHit);

/**
 * 
 */
UCLASS()
class CLIENT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds);

	void PlayAttackMontage();
	void PlayHitMontage();
	void PlayDeathMontage();

	void JumpToSection(int32 MontageType, int32 SectionIndex);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;

protected:
	UFUNCTION()
	void AnimNotify_AttackHit();

public:
	FOnEnemyAttackHit OnEnemyAttackHit;
};
