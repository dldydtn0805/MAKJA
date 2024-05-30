// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_PathFind.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UBTService_PathFind : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_PathFind();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	FVector CoreLocation = FVector(1150.f, 300.f, 100.f);
};
