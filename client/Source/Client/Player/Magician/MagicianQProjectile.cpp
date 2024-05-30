// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicianQProjectile.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../../Enemy/Enemy.h"
#include "MagicianStatComponent.h"
#include "MagicianCharacter.h"

// Sets default values
AMagicianQProjectile::AMagicianQProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    // 구체를 단순 콜리전 표현으로 사용합니다.
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("ProjectileQ"));
    // 구체의 콜리전 반경을 설정합니다.
    CollisionComponent->InitSphereRadius(15.0f);
    // 루트 컴포넌트를 콜리전 컴포넌트로 설정합니다.
    RootComponent = CollisionComponent;

    // 이 컴포넌트를 사용하여 이 프로젝타일의 무브먼트를 구동시킵니다.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileQMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
    ProjectileMovementComponent->MaxSpeed = 3000.0f;
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
void AMagicianQProjectile::BeginPlay()
{
	Super::BeginPlay();
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMagicianQProjectile::OnOverlapBegin);
}

// Called every frame
void AMagicianQProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 프로젝타일의 속도를 발사 방향으로 초기화시키는 함수입니다.
void AMagicianQProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}


void AMagicianQProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 충돌한 대상이 캐릭터면 피격 로직 수행
    //UE_LOG(LogTemp, Log, TEXT("AttackNormal"));
    UE_LOG(LogTemp, Log, TEXT("Overlap with %s"), *OtherActor->GetName());

    // 내가 밟아도 데미지 닳게 근데 나중에 적만 데미지 입어야함;
    AEnemy* Enemy = Cast<AEnemy>(OtherActor);
    if (Enemy) {
        UE_LOG(LogTemp, Log, TEXT("Damaged Q! %f"), DamageAmount);

        // 충돌 위치에서 파티클 시스템 스폰
        UParticleSystemComponent* SpawnedParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleComponent2->Template, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());

        // 파티클 시스템 크기 5배 증가
        SpawnedParticleSystem->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));

        // 파티클 시스템의 반경 가져오기
        float ParticleSystemRadius = SpawnedParticleSystem->Bounds.SphereRadius;

        // 파티클 시스템 반경 내에 있는 적들 탐색
        TArray<FOverlapResult> OverlappingEnemies;
        GetWorld()->OverlapMultiByObjectType(OverlappingEnemies, SweepResult.ImpactPoint, FQuat::Identity, FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), FCollisionShape::MakeSphere(ParticleSystemRadius));

        // 탐색된 적들의 고유 액터만 추출
        TArray<AActor*> UniqueEnemyActors;
        for (const FOverlapResult& OverlappingEnemy : OverlappingEnemies) {
            AActor* OverlappingActor = OverlappingEnemy.GetActor();
            if (OverlappingActor && !UniqueEnemyActors.Contains(OverlappingActor)) {
                UniqueEnemyActors.Add(OverlappingActor);
            }
        }

        // 고유 적들에 대해서만 데미지 적용
        for (AActor* EnemyActor : UniqueEnemyActors) {
            AEnemy* UniqueEnemy = Cast<AEnemy>(EnemyActor);
            if (UniqueEnemy) {
                FVector PushDirection = UniqueEnemy->GetActorLocation() - SweepResult.ImpactPoint;
                PushDirection.Normalize();

                // TakeDamage 함수 호출
                FDamageEvent DamageEvent;
                UniqueEnemy->TakeDamage(DamageAmount, DamageEvent, GetInstigatorController(), this);
                USoundBase* ExplosionSound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/Sounds/Player/Magician/AttackSound/AttackQExplosion.AttackQExplosion'"));
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetOwner()->GetActorLocation());
                UniqueEnemy->LaunchCharacter(PushDirection * 777.0f, true, true);
            }
        }

        Destroy();
    }
}