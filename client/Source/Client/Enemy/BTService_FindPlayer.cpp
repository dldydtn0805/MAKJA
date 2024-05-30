// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindPlayer.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


UBTService_FindPlayer::UBTService_FindPlayer() {
	NodeName = TEXT("Find Player");
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("Destination")), PlayerCharacter->GetActorLocation());
	}
	else {
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName(TEXT("Target")), 0);
	}
	
}
