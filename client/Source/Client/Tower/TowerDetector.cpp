// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDetector.h"
#include "Components/SphereComponent.h"
#include "../Nexus/Nexus.h"
#include "Tower.h"

// Sets default values
ATowerDetector::ATowerDetector()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 콜리전 컴포넌트 설정
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("TowerSpawnDetector")); // 구체에 프로젝타일 콜리전 채널 설정
	RootComponent = CollisionComponent; // 루트 컴포넌트로 지정

	// SpawnDetector 설정
	SpawnDetector = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SPAWN_DETECTOR"));
	SpawnDetector->SetupAttachment(RootComponent);

	// TowerPreviewComponent 설정
	TowerPreviewComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TOWER_PREVIEW"));
	TowerPreviewComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATowerDetector::BeginPlay()
{
	Super::BeginPlay();

	// CollisionComponent와 SpawnDetector의 사이즈 조절
	CollisionComponent->SetSphereRadius(DetectRange);
	SpawnDetector->SetWorldScale3D(FVector(DetectRange / 50.f, DetectRange / 50.f, 0.2f));

	// Overlap 관련 델리게이트 설정
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATowerDetector::OnOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATowerDetector::OnOverlapEnd);

	// 감지된 탑의 수 초기화
	InitDetectedStatus();
	// 감지된 탑의 수를 토대로 메시 지정 및 스케일 조정
	SpawnDetector->SetStaticMesh(DetectedTowerNum || DetectedNexus ? DetectMesh : NotDetectMesh);
}

// 감지된 탑의 수 초기화
void ATowerDetector::InitDetectedStatus()
{
	// ATower 타입 액터들만 확인
	TArray<AActor*> OverlappedTowers;
	CollisionComponent->GetOverlappingActors(OverlappedTowers, ATower::StaticClass());
	DetectedTowerNum = OverlappedTowers.Num();

	TArray<AActor*> OverlappedNexus;
	CollisionComponent->GetOverlappingActors(OverlappedNexus, ANexus::StaticClass());
	DetectedNexus = OverlappedNexus.Num() > 0;
}

// 새로 오버랩된 탑이 있을 때
void ATowerDetector::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 이전 상태가 아무것도 감지가 안된 상태라면 감지 상태에 맞는 메쉬로 변경
	if (DetectedTowerNum == 0 && !DetectedNexus)
	{
		SpawnDetector->SetStaticMesh(DetectMesh);
	}

	// 오버랩된 오브젝트가 넥서스인지 먼저 확인
	ANexus* Nexus = Cast<ANexus>(OtherActor);
	if (Nexus) {
		DetectedNexus = true;
		UE_LOG(LogTemp, Log, TEXT("Detected Nexus : %d"), DetectedNexus);
	}
	else {
		DetectedTowerNum++;
		UE_LOG(LogTemp, Log, TEXT("Detected Tower : %d"), GetDetectedTowerNum());
	}
}

// 오버랩이 해제 된 탑이 있을 때
void ATowerDetector::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 오버랩이 해제된 오브젝트가 넥서스인지 먼저 확인
	ANexus* Nexus = Cast<ANexus>(OtherActor);
	if (Nexus) {
		DetectedNexus = false;
		UE_LOG(LogTemp, Log, TEXT("Detected Nexus : %d"), DetectedNexus);
	}
	else {
		DetectedTowerNum--;
		UE_LOG(LogTemp, Log, TEXT("Detected Tower : %d"), GetDetectedTowerNum());
	}
	
	// 이후 상태가 아무것도 감지가 안된 상태라면 감지 상태에 맞는 메쉬로 변경
	if (DetectedTowerNum == 0 && !DetectedNexus)
	{
		SpawnDetector->SetStaticMesh(NotDetectMesh);
	}
}

// Called every frame
void ATowerDetector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 감지된 탑 하나를 반환
ATower* ATowerDetector::GetOverlappedTowerOnce()
{
	TArray<AActor*> OverlappedActors;
	// ATower 타입의 액터들만 검색합니다.
	CollisionComponent->GetOverlappingActors(OverlappedActors, ATower::StaticClass());

	for (auto& Actor : OverlappedActors) {
		ATower* Tower = Cast<ATower>(Actor);
		if (Tower != nullptr) {
			return Tower;
		}
	}

	return nullptr;
}