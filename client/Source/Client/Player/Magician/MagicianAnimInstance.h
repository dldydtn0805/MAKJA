// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MagicianAnimInstance.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);

UCLASS()
class CLIENT_API UMagicianAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMagicianAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// 특정 어택 몽타주 실행 함수
	void PlayDeathMontage();
	void PlayAttackLeftMontage();
	void PlayAttackRightMontage();
	void PlayAttackQMontage();
	void PlayAttackEMontage();
	void PlayAttackRMontage();
	void PlayTeleportMontage();
	void JumpToLeftSection(int32 LeftSectionIndex);
	void JumpToRightSection(int32 RightSectionIndex);
	void JumpToQSection(int32 QSectionIndex);

	FOnAttackHit OnAttackLeftHit;
	FOnAttackHit OnAttackRightHit;
	FOnAttackHit OnAttackQHit;
	FOnAttackHit OnAttackEHit;
	FOnAttackHit OnAttackRHit;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Magician, Meta = (AllowPrivateAccess = true))
	float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Magician, Meta = (AllowPrivateAccess = true))
	bool IsFalling;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Magician, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Magician, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackLeftMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Magician, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackRightMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Magician, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackQMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Magician, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackEMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Magician, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackRMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Magician, Meta = (AllowPrivateAccess = true))
	UAnimMontage* TeleportMontage;

	UFUNCTION()
	void AnimNotify_AttackLeftHit();
	UFUNCTION()
	void AnimNotify_AttackRightHit();
	UFUNCTION()
	void AnimNotify_AttackQHit();
	UFUNCTION()
	void AnimNotify_AttackEHit();
	UFUNCTION()
	void AnimNotify_AttackRHit();

	FName GetAttackLeftMontageName(int32 LeftSectionIndex);
	FName GetAttackRightMontageName(int32 RightSectionIndex);
	FName GetAttackQMontageName(int32 QSectionIndex);
};
