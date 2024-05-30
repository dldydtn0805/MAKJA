// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicianAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MagicianCharacter.h"


UMagicianAnimInstance::UMagicianAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ADM(TEXT("AnimMontage'/Game/Animations/Player/Magician/MagicianDeathMontage.MagicianDeathMontage'"));
	if (ADM.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Die Montage"));
		DeathMontage = ADM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ANM(TEXT("AnimMontage'/Game/Animations/Player/Magician/MagicianAttackNormalAnimMontage.MagicianAttackNormalAnimMontage'"));
	if (ANM.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Attack Left Montage"));
		AttackLeftMontage = ANM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ARIM(TEXT("AnimMontage'/Game/Animations/Player/Magician/MagicianAttackRightAnimMontage.MagicianAttackRightAnimMontage'"));
	if (ARIM.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Attack Right Montage"));
		AttackRightMontage = ARIM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AQM(TEXT("AnimMontage'/Game/Animations/Player/Magician/MagicianAttackQAnimMontage.MagicianAttackQAnimMontage'"));
	if (AQM.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Attack Q Montage"));
		AttackQMontage = AQM.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AEM(TEXT("AnimMontage'/Game/Animations/Player/Magician/MagicianAttackEAnimMontage.MagicianAttackEAnimMontage'"));
	if (AEM.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Attack E Montage"));
		AttackEMontage = AEM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ARM(TEXT("AnimMontage'/Game/Animations/Player/Magician/MagicianAttackRAnimMontage.MagicianAttackRAnimMontage'"));
	if (ARM.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Attack R Montage"));
		AttackRMontage = ARM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> TM(TEXT("AnimMontage'/Game/Animations/Player/Magician/MagicianTeleportAnimMontage.MagicianTeleportAnimMontage'"));
	if (TM.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("Teleport Montage"));
		TeleportMontage = TM.Object;
	}	
}

void UMagicianAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds); // Super : UMyAnimInstance의 부모, Generate Body에 포함된 기능
		auto Pawn = TryGetPawnOwner();
		if (IsValid(Pawn)) {
			Speed = Pawn->GetVelocity().Size(); // 프레임마다 Speed 업데이트
			auto Character = Cast<AMagicianCharacter>(Pawn);
			if (Character) {
				IsFalling = Character->GetMovementComponent()->IsFalling();
			}
		}
}

void UMagicianAnimInstance::PlayDeathMontage()
{
	if (!Montage_IsPlaying(DeathMontage))
	{
		Montage_Play(DeathMontage, 1.f);
	}
}

void UMagicianAnimInstance::PlayAttackLeftMontage()
{
	if (!Montage_IsPlaying(AttackLeftMontage))
	{
		Montage_Play(AttackLeftMontage, 1.f);
	}
}

void UMagicianAnimInstance::PlayAttackRightMontage()
{
	if (!Montage_IsPlaying(AttackRightMontage))
	{
		Montage_Play(AttackRightMontage, 1.f);
	}
}

void UMagicianAnimInstance::PlayAttackQMontage()
{
	if (!Montage_IsPlaying(AttackQMontage))
	{
		Montage_Play(AttackQMontage, 1.f);
	}
}

void UMagicianAnimInstance::PlayAttackEMontage()
{
	if (!Montage_IsPlaying(AttackEMontage))
	{
		Montage_Play(AttackEMontage, 1.f);
	}
}

void UMagicianAnimInstance::PlayAttackRMontage()
{
	if (!Montage_IsPlaying(AttackRMontage))
	{
		Montage_Play(AttackRMontage, 1.f);
	}
}



void UMagicianAnimInstance::PlayTeleportMontage()
{
	if (!Montage_IsPlaying(TeleportMontage))
	{
		Montage_Play(TeleportMontage, 1.f);
	}
}

void UMagicianAnimInstance::AnimNotify_AttackLeftHit()
{
	UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackLeftHit"))
	OnAttackLeftHit.Broadcast();
}

void UMagicianAnimInstance::AnimNotify_AttackRightHit()
{
	UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackRightHit"));
	OnAttackRightHit.Broadcast();
}

void UMagicianAnimInstance::AnimNotify_AttackQHit()
{
	UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackQHit"))
	OnAttackQHit.Broadcast();
}

void UMagicianAnimInstance::AnimNotify_AttackEHit()
{
	UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackEHit"))
	OnAttackEHit.Broadcast();

}

void UMagicianAnimInstance::AnimNotify_AttackRHit()
{
	UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackRHit"))
	OnAttackRHit.Broadcast();

}

FName UMagicianAnimInstance::GetAttackLeftMontageName(int32 LeftSectionIndex)
{
	return FName(*FString::Printf(TEXT("AttackLeft%d"), LeftSectionIndex));
}

FName UMagicianAnimInstance::GetAttackRightMontageName(int32 RightSectionIndex)
{
	return FName(*FString::Printf(TEXT("AttackRight%d"), RightSectionIndex));
}

FName UMagicianAnimInstance::GetAttackQMontageName(int32 QSectionIndex)
{
	return FName(*FString::Printf(TEXT("AttackQ%d"), QSectionIndex));
}

void UMagicianAnimInstance::JumpToLeftSection(int32 LeftSectionIndex)
{
	FName Name = GetAttackLeftMontageName(LeftSectionIndex);
	Montage_JumpToSection(Name, AttackLeftMontage);
}

void UMagicianAnimInstance::JumpToRightSection(int32 RightSectionIndex)
{
	FName Name = GetAttackRightMontageName(RightSectionIndex);
	Montage_JumpToSection(Name, AttackRightMontage);
}

void UMagicianAnimInstance::JumpToQSection(int32 QSectionIndex)
{
	FName Name = GetAttackQMontageName(QSectionIndex);
	Montage_JumpToSection(Name, AttackQMontage);
}