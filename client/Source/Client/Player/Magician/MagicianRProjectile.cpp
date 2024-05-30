// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicianRProjectile.h"
#include "MagicianCharacter.h"
#include "../../Enemy/Enemy.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMagicianRProjectile::AMagicianRProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 1.0f;

    // ��ü�� �ܼ� �ݸ��� ǥ������ ����մϴ�.
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("ProjectileR"));

    // ��ü�� �ݸ��� �ݰ��� �����մϴ�.
    CollisionComponent->InitSphereRadius(1000.0f);
    // ��Ʈ ������Ʈ�� �ݸ��� ������Ʈ�� �ǵ��� �����մϴ�.
    RootComponent = CollisionComponent;

    // �� ������Ʈ�� ����Ͽ� �� ������Ÿ���� �����Ʈ�� ������ŵ�ϴ�.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 1000.0f;
    ProjectileMovementComponent->MaxSpeed = 1500.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;

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
void AMagicianRProjectile::BeginPlay()
{
    Super::BeginPlay();
    // PrimaryActorTick.TickInterval = 1.0f;
    // LifeSpan
    SetLifeSpan(5.0f);
    UE_LOG(LogTemp, Error, TEXT("R Damage Amount : %f"), DamageAmount);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMagicianRProjectile::OnOverlapBegin);
}

// Called every frame
void AMagicianRProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// �߻� �������� ������Ÿ�� �ӵ� �ʱ�ȭ
void AMagicianRProjectile::FireDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AMagicianRProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Log, TEXT("Overlap with %s"), *OtherActor->GetName());
    AEnemy* Enemy = Cast<AEnemy>(OtherActor);
    if (Enemy)
    {
        UE_LOG(LogTemp, Error, TEXT("R Attack Damaged!"));
        // ��ƼŬ �߰�
        UParticleSystemComponent* SpawnedParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleComponent2->Template, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());
        // �߰��� ��ƼŬ�� �����ϰ�, ���ϴ� ��쿡�� ũ�⸦ ������ �� �ֽ��ϴ�.
        FDamageEvent DamageEvent;
        Enemy->TakeDamage(DamageAmount, DamageEvent, GetInstigatorController(), this);

    }
}
