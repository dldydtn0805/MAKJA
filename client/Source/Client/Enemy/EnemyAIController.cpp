// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "Enemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController() {
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/AI/BT_Enemy.BT_Enemy'"));
	if (BT.Succeeded()) {
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/AI/BB_Enemy.BB_Enemy'"));
	if (BD.Succeeded()) {
		BlackboardData = BD.Object;
	}

}
void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const FVector CoreLocation(1150.f, 300.f, 100.f);

	if (UseBlackboard(BlackboardData, Blackboard)) {
		Blackboard->SetValueAsVector(FName(TEXT("CoreCord")), CoreLocation);
		Blackboard->SetValueAsInt(FName(TEXT("Target")), 0); // Target: 0 - Nexus, 1 - Barricade, 2- Player
		if (RunBehaviorTree(BehaviorTree)) {
			//TODO
		}
	}
}

void AEnemyAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void AEnemyAIController::OnPlayerHit(AActor* Attacker) {
	if (Attacker) {
		Blackboard->SetValueAsInt(FName(TEXT("Target")), 2);
		Blackboard->SetValueAsVector(FName(TEXT("Destination")), Attacker->GetActorLocation());
		if (BrainComponent) {
			BrainComponent->RestartLogic();
		}
	}
}