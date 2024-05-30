// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicianEProjectile.h"
#include "Components/BoxComponent.h"
#include "DrawDebugHelpers.h"
#include "MagicianCharacter.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "../../Enemy/Enemy.h"

// Sets default values
AMagicianEProjectile::AMagicianEProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    // ��ü�� �ܼ� �ݸ��� ǥ������ ����մϴ�.
    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent2"));
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("ProjectileE"));

    // �ڽ� �ݸ��� ũ�� ����
    CollisionComponent->InitBoxExtent(FVector(500.0f, 500.0f, 1000.0f));
    RootComponent = CollisionComponent;

    ParticleComponent1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent1"));
    ParticleComponent1->bAutoActivate = true; // �ڵ� Ȱ��ȭ ����
    ParticleComponent1->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset1(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
    if (ParticleAsset1.Succeeded())
    {
        ParticleComponent1->SetTemplate(ParticleAsset1.Object);
    }

    ParticleComponent2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent2"));
    ParticleComponent2->bAutoActivate = false; // �ڵ� Ȱ��ȭ ����
    ParticleComponent2->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset2(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
    if (ParticleAsset2.Succeeded())
    {
        ParticleComponent2->SetTemplate(ParticleAsset2.Object);
    }
}

// Called when the game starts or when spawned
void AMagicianEProjectile::BeginPlay()
{
    Super::BeginPlay();
    SetLifeSpan(0.5f);
    UE_LOG(LogTemp, Error, TEXT("E Damage Amount : %f"), DamageAmount);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMagicianEProjectile::OnOverlapBegin);
}

// Called every frame
void AMagicianEProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void AMagicianEProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // �浹�� ����� ĳ���͸� �ǰ� ���� ����
    //UE_LOG(LogTemp, Log, TEXT("AttackNormal"));
    AEnemy* Enemy = Cast<AEnemy>(OtherActor);
    if (Enemy) {
        UE_LOG(LogTemp, Log, TEXT("Overlap with %s"), *OtherActor->GetName());
        UE_LOG(LogTemp, Log, TEXT("E Attack Damaged!"));
        UParticleSystemComponent* SpawnedParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleComponent2->Template, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());

        // ��ƼŬ �ý��� ũ�� 5�� ����
        SpawnedParticleSystem->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));
        FDamageEvent DamageEvent;
        Enemy->TakeDamage(DamageAmount, DamageEvent, GetInstigatorController(), this);

    }
}

// Enemy ���� �ؾ���
//void AMagicianEProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//    UE_LOG(LogTemp, Warning, TEXT("AttackE OverlapBegin : %d"), CollisionComponent == nullptr);
//
//    int32 DamageAmount;
//    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
//    APawn* DefaultPawn = PlayerController->GetPawn();
//    UPlayerStatComponent* PlayerStat = DefaultPawn->FindComponentByClass<UPlayerStatComponent>();
//    DamageAmount = PlayerStat->GetEDamage();
//
//    TArray<AActor*> OverlappingActors;
//    CollisionComponent->GetOverlappingActors(OverlappingActors, AEnemyPawn::StaticClass());
//
//    for (AActor* Actor : OverlappingActors)
//    {
//        if (Actor != nullptr) // ��ȿ�� �˻� �߰�
//        {
//            AEnemyPawn* Enemy = Cast<AEnemyPawn>(Actor);
//            if (Enemy)
//            {
//                // EU_LOG�� �� ���� ���
//                UE_LOG(LogTemp, Warning, TEXT("Enemy Detected: %s at Location: %s"), *Enemy->GetName(), *Enemy->GetActorLocation().ToString());
//                Enemy->TakeDamage(DamageAmount);
//            }
//        }
//    }
//
//
//
//}
//
