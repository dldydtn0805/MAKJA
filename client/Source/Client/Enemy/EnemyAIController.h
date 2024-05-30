// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	void OnPlayerHit(AActor* Atacker);

private:
	UPROPERTY()
	class UBehaviorTree* BehaviorTree;
	UPROPERTY()
	class UBlackboardData* BlackboardData;
};
