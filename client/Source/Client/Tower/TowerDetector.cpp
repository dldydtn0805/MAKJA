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

	// �ݸ��� ������Ʈ ����
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("TowerSpawnDetector")); // ��ü�� ������Ÿ�� �ݸ��� ä�� ����
	RootComponent = CollisionComponent; // ��Ʈ ������Ʈ�� ����

	// SpawnDetector ����
	SpawnDetector = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SPAWN_DETECTOR"));
	SpawnDetector->SetupAttachment(RootComponent);

	// TowerPreviewComponent ����
	TowerPreviewComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TOWER_PREVIEW"));
	TowerPreviewComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATowerDetector::BeginPlay()
{
	Super::BeginPlay();

	// CollisionComponent�� SpawnDetector�� ������ ����
	CollisionComponent->SetSphereRadius(DetectRange);
	SpawnDetector->SetWorldScale3D(FVector(DetectRange / 50.f, DetectRange / 50.f, 0.2f));

	// Overlap ���� ��������Ʈ ����
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATowerDetector::OnOverlapBegin);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ATowerDetector::OnOverlapEnd);

	// ������ ž�� �� �ʱ�ȭ
	InitDetectedStatus();
	// ������ ž�� ���� ���� �޽� ���� �� ������ ����
	SpawnDetector->SetStaticMesh(DetectedTowerNum || DetectedNexus ? DetectMesh : NotDetectMesh);
}

// ������ ž�� �� �ʱ�ȭ
void ATowerDetector::InitDetectedStatus()
{
	// ATower Ÿ�� ���͵鸸 Ȯ��
	TArray<AActor*> OverlappedTowers;
	CollisionComponent->GetOverlappingActors(OverlappedTowers, ATower::StaticClass());
	DetectedTowerNum = OverlappedTowers.Num();

	TArray<AActor*> OverlappedNexus;
	CollisionComponent->GetOverlappingActors(OverlappedNexus, ANexus::StaticClass());
	DetectedNexus = OverlappedNexus.Num() > 0;
}

// ���� �������� ž�� ���� ��
void ATowerDetector::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ���� ���°� �ƹ��͵� ������ �ȵ� ���¶�� ���� ���¿� �´� �޽��� ����
	if (DetectedTowerNum == 0 && !DetectedNexus)
	{
		SpawnDetector->SetStaticMesh(DetectMesh);
	}

	// �������� ������Ʈ�� �ؼ������� ���� Ȯ��
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

// �������� ���� �� ž�� ���� ��
void ATowerDetector::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// �������� ������ ������Ʈ�� �ؼ������� ���� Ȯ��
	ANexus* Nexus = Cast<ANexus>(OtherActor);
	if (Nexus) {
		DetectedNexus = false;
		UE_LOG(LogTemp, Log, TEXT("Detected Nexus : %d"), DetectedNexus);
	}
	else {
		DetectedTowerNum--;
		UE_LOG(LogTemp, Log, TEXT("Detected Tower : %d"), GetDetectedTowerNum());
	}
	
	// ���� ���°� �ƹ��͵� ������ �ȵ� ���¶�� ���� ���¿� �´� �޽��� ����
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

// ������ ž �ϳ��� ��ȯ
ATower* ATowerDetector::GetOverlappedTowerOnce()
{
	TArray<AActor*> OverlappedActors;
	// ATower Ÿ���� ���͵鸸 �˻��մϴ�.
	CollisionComponent->GetOverlappingActors(OverlappedActors, ATower::StaticClass());

	for (auto& Actor : OverlappedActors) {
		ATower* Tower = Cast<ATower>(Actor);
		if (Tower != nullptr) {
			return Tower;
		}
	}

	return nullptr;
}