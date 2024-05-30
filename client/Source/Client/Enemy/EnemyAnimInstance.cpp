// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"


UEnemyAnimInstance::UEnemyAnimInstance()
{

	Speed = 0.0f;
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	auto pawn = TryGetPawnOwner();
	if (IsValid(pawn)) {
		Speed = pawn->GetVelocity().Size();
	}
}

void UEnemyAnimInstance::PlayAttackMontage() {
	Montage_Play(AttackMontage, 1.f);
}

void UEnemyAnimInstance::PlayHitMontage()
{
	Montage_Play(HitMontage, 1.f);
}

void UEnemyAnimInstance::PlayDeathMontage()
{
	Montage_Play(DeathMontage, 1.f);
}

void UEnemyAnimInstance::JumpToSection(int32 MontageType, int32 SectionIndex)
{
	switch (MontageType) {
	case(0):
	{
		FName Name = FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
		Montage_JumpToSection(Name, AttackMontage);
		break;
	}
	case(1):
	{
		FName Name = FName(*FString::Printf(TEXT("Hit%d"), SectionIndex));
		Montage_JumpToSection(Name, HitMontage);
		break;
	}
	case(2):
	{
		FName Name = FName(*FString::Printf(TEXT("Death%d"), SectionIndex));
		Montage_JumpToSection(Name, DeathMontage);
		break;
	}
	}
}

void UEnemyAnimInstance::AnimNotify_AttackHit() {
	OnEnemyAttackHit.Broadcast();
}