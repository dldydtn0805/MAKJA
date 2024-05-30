// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicianRightProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../Enemy/Enemy.h"
#include "MagicianStatComponent.h"

// Sets default values
AMagicianRightProjectile::AMagicianRightProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    // 구체를 단순 콜리전 표현으로 사용합니다.
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("ProjectileRight"));
    // 구체의 콜리전 반경을 설정합니다.
    CollisionComponent->InitSphereRadius(15.0f);
    // 루트 컴포넌트를 콜리전 컴포넌트로 설정합니다.
    RootComponent = CollisionComponent;

    // 이 컴포넌트를 사용하여 이 프로젝타일의 무브먼트를 구동시킵니다.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 3333.0f;
    ProjectileMovementComponent->MaxSpeed = 3333.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;
    ProjectileMovementComponent->ProjectileGravityScale = 0.f; // 중력 값 설정 (0이면 중력 없음)

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

    // 캐릭터 스탯 가져오기
    MagicianStat = CreateDefaultSubobject<UMagicianStatComponent>(TEXT("MagicianStat"));
}

// Called when the game starts or when spawned
void AMagicianRightProjectile::BeginPlay()
{
	Super::BeginPlay();
    SetLifeSpan(5.f);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMagicianRightProjectile::OnOverlapBegin);
}

// Called every frame
void AMagicianRightProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 프로젝타일의 속도를 발사 방향으로 초기화시키는 함수입니다.
void AMagicianRightProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AMagicianRightProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 충돌한 대상이 캐릭터면 피격 로직 수행
    //UE_LOG(LogTemp, Log, TEXT("AttackNormal"));
    UE_LOG(LogTemp, Log, TEXT("Overlap with %s"), *OtherActor->GetName());
    AEnemy* Enemy = Cast<AEnemy>(OtherActor);
    if (Enemy) {
        UE_LOG(LogTemp, Log, TEXT("Damaged Right! %f"), DamageAmount);
        UParticleSystemComponent* SpawnedParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleComponent2->Template, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());

        // 파티클 시스템 크기 5배 증가
        SpawnedParticleSystem->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));

		FDamageEvent DamageEvent;
		Enemy->TakeDamage(DamageAmount, DamageEvent, GetInstigatorController(), this);

        UE_LOG(LogTemp, Log, TEXT("right attack damgage : %f"), MagicianStat->GetRightDamage());
        Destroy();
    }
}