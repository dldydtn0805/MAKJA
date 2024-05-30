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

    // 구체를 단순 콜리전 표현으로 사용합니다.
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("ProjectileR"));

    // 구체의 콜리전 반경을 설정합니다.
    CollisionComponent->InitSphereRadius(1000.0f);
    // 루트 컴포넌트를 콜리전 컴포넌트가 되도록 설정합니다.
    RootComponent = CollisionComponent;

    // 이 컴포넌트를 사용하여 이 프로젝타일의 무브먼트를 구동시킵니다.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 1000.0f;
    ProjectileMovementComponent->MaxSpeed = 1500.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;

    ParticleComponent1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent1"));
    ParticleComponent1->bAutoActivate = true; // 자동 활성화 설정
    ParticleComponent1->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset1(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
    if (ParticleAsset1.Succeeded())
    {
        ParticleComponent1->SetTemplate(ParticleAsset1.Object);
    }

    ParticleComponent2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent2"));
    ParticleComponent2->bAutoActivate = false; // 자동 활성화 설정
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

// 발사 방향으로 프로젝타이 속도 초기화
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
        // 파티클 추가
        UParticleSystemComponent* SpawnedParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleComponent2->Template, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());
        // 추가한 파티클이 존재하고, 원하는 경우에만 크기를 조정할 수 있습니다.
        FDamageEvent DamageEvent;
        Enemy->TakeDamage(DamageAmount, DamageEvent, GetInstigatorController(), this);

    }
}
