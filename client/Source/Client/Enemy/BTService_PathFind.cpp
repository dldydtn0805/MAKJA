// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PathFind.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

#include "../Tower/Barricade/Barricade.h"
#include "../Nexus/Nexus.h"
#include "Kismet/KismetSystemLibrary.h"

UBTService_PathFind::UBTService_PathFind() {
	NodeName = TEXT("Path Find");
	Interval = 1.0f;
}

void UBTService_PathFind::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return;
	UWorld* World = CurrentPawn->GetWorld();
	FVector MyLocation = CurrentPawn->GetActorLocation();

	if (World == nullptr)
		return;

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	FVector dest_cord = CoreLocation;

	if (NavSys) {
		UNavigationPath* NavPath = NavSys->FindPathToLocationSynchronously(World, MyLocation, CoreLocation, nullptr, nullptr);
		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			for (int32 i = 0; i < NavPath->PathPoints.Num() - 1; i++)
			{
				// 라인 트레이스로 중간에 ATower_Barricade / ANexus가 있는지 확인
				FHitResult HitResult;
				FCollisionQueryParams CollisionParams;

				bool bHit = World->LineTraceSingleByChannel(
					HitResult,
					NavPath->PathPoints[i],
					NavPath->PathPoints[i + 1],
					ECollisionChannel::ECC_GameTraceChannel11,
					CollisionParams
				);

				if (bHit)
				{
					UE_LOG(LogTemp, Warning, TEXT("Barricade hit at: %s"), *HitResult.ImpactPoint.ToString());
					ABarricade * BC = Cast<ABarricade>(HitResult.GetActor());
					if (BC)
					{
						dest_cord = HitResult.ImpactPoint;
						break; // 첫 번째 타워를 만난 후 for문 탈출
					}
					else {
						ANexus* Core = Cast<ANexus>(HitResult.GetActor());
						if (Core) {
							dest_cord = HitResult.ImpactPoint;
							break;
						}
					}
				}
			}
			OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("Destination")), dest_cord);
			OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName(TEXT("Target")), 1);
			return;
		}
		
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("Destination")), dest_cord);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(FName(TEXT("Target")), 0);
}