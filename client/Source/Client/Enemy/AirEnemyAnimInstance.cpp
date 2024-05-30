// Fill out your copyright notice in the Description page of Project Settings.


#include "AirEnemyAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"

UAirEnemyAnimInstance::UAirEnemyAnimInstance()
    :Super()
{
    IsFloating = true;
}


void UAirEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
    auto pawn = TryGetPawnOwner();
    auto Enemy = Cast<AEnemy>(pawn);
    if (Enemy) {
        IsFloating = Enemy->GetMovementComponent()->IsFlying();
        //MaxSpeed = Enemy->Stat->GetSpeed();
    }   
}