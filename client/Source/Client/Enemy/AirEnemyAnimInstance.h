// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAnimInstance.h"
#include "AirEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UAirEnemyAnimInstance : public UEnemyAnimInstance
{
	GENERATED_BODY()

public:
    UAirEnemyAnimInstance();

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
    float MaxSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
    bool IsFloating;
};
