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
    // ��ü�� �ܼ� �ݸ��� ǥ������ ����մϴ�.
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("ProjectileRight"));
    // ��ü�� �ݸ��� �ݰ��� �����մϴ�.
    CollisionComponent->InitSphereRadius(15.0f);
    // ��Ʈ ������Ʈ�� �ݸ��� ������Ʈ�� �����մϴ�.
    RootComponent = CollisionComponent;

    // �� ������Ʈ�� ����Ͽ� �� ������Ÿ���� �����Ʈ�� ������ŵ�ϴ�.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 3333.0f;
    ProjectileMovementComponent->MaxSpeed = 3333.0f;
    ProjectileMovementComponent->bRotationFollowsVelocity = true;
    ProjectileMovementComponent->bShouldBounce = false;
    ProjectileMovementComponent->ProjectileGravityScale = 0.f; // �߷� �� ���� (0�̸� �߷� ����)

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

    // ĳ���� ���� ��������
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

// ������Ÿ���� �ӵ��� �߻� �������� �ʱ�ȭ��Ű�� �Լ��Դϴ�.
void AMagicianRightProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AMagicianRightProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // �浹�� ����� ĳ���͸� �ǰ� ���� ����
    //UE_LOG(LogTemp, Log, TEXT("AttackNormal"));
    UE_LOG(LogTemp, Log, TEXT("Overlap with %s"), *OtherActor->GetName());
    AEnemy* Enemy = Cast<AEnemy>(OtherActor);
    if (Enemy) {
        UE_LOG(LogTemp, Log, TEXT("Damaged Right! %f"), DamageAmount);
        UParticleSystemComponent* SpawnedParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleComponent2->Template, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());

        // ��ƼŬ �ý��� ũ�� 5�� ����
        SpawnedParticleSystem->SetRelativeScale3D(FVector(2.0f, 2.0f, 2.0f));

		FDamageEvent DamageEvent;
		Enemy->TakeDamage(DamageAmount, DamageEvent, GetInstigatorController(), this);

        UE_LOG(LogTemp, Log, TEXT("right attack damgage : %f"), MagicianStat->GetRightDamage());
        Destroy();
    }
}