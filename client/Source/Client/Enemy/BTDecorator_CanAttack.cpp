// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanAttack.h"
#include "../Player/Magician/MagicianCharacter.h"
#include "EnemyAIController.h"
#include "EnemyStatComponent.h"
#include "Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return false;

	FVector Dest = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("Destination"));
	
	auto Stat = Cast<UEnemyStatComponent>(Cast<AEnemy>(CurrentPawn)->Stat);
	if (Stat == nullptr)
		return false;
	
	float Range = Stat->GetAttackRange();
	
	float Distance = FVector::Distance(CurrentPawn->GetActorLocation(), Dest);

	return bResult && Distance <= Range;
}
