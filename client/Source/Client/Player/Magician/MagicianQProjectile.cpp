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
    // ��ü�� �ܼ� �ݸ��� ǥ������ ����մϴ�.
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("ProjectileQ"));
    // ��ü�� �ݸ��� �ݰ��� �����մϴ�.
    CollisionComponent->InitSphereRadius(15.0f);
    // ��Ʈ ������Ʈ�� �ݸ��� ������Ʈ�� �����մϴ�.
    RootComponent = CollisionComponent;

    // �� ������Ʈ�� ����Ͽ� �� ������Ÿ���� �����Ʈ�� ������ŵ�ϴ�.
    ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileQMovementComponent"));
    ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
    ProjectileMovementComponent->InitialSpeed = 3000.0f;
    ProjectileMovementComponent->MaxSpeed = 3000.0f;
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

// ������Ÿ���� �ӵ��� �߻� �������� �ʱ�ȭ��Ű�� �Լ��Դϴ�.
void AMagicianQProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}


void AMagicianQProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // �浹�� ����� ĳ���͸� �ǰ� ���� ����
    //UE_LOG(LogTemp, Log, TEXT("AttackNormal"));
    UE_LOG(LogTemp, Log, TEXT("Overlap with %s"), *OtherActor->GetName());

    // ���� ��Ƶ� ������ ��� �ٵ� ���߿� ���� ������ �Ծ����;
    AEnemy* Enemy = Cast<AEnemy>(OtherActor);
    if (Enemy) {
        UE_LOG(LogTemp, Log, TEXT("Damaged Q! %f"), DamageAmount);

        // �浹 ��ġ���� ��ƼŬ �ý��� ����
        UParticleSystemComponent* SpawnedParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleComponent2->Template, SweepResult.ImpactPoint, SweepResult.ImpactNormal.Rotation());

        // ��ƼŬ �ý��� ũ�� 5�� ����
        SpawnedParticleSystem->SetRelativeScale3D(FVector(2.5f, 2.5f, 2.5f));

        // ��ƼŬ �ý����� �ݰ� ��������
        float ParticleSystemRadius = SpawnedParticleSystem->Bounds.SphereRadius;

        // ��ƼŬ �ý��� �ݰ� ���� �ִ� ���� Ž��
        TArray<FOverlapResult> OverlappingEnemies;
        GetWorld()->OverlapMultiByObjectType(OverlappingEnemies, SweepResult.ImpactPoint, FQuat::Identity, FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn), FCollisionShape::MakeSphere(ParticleSystemRadius));

        // Ž���� ������ ���� ���͸� ����
        TArray<AActor*> UniqueEnemyActors;
        for (const FOverlapResult& OverlappingEnemy : OverlappingEnemies) {
            AActor* OverlappingActor = OverlappingEnemy.GetActor();
            if (OverlappingActor && !UniqueEnemyActors.Contains(OverlappingActor)) {
                UniqueEnemyActors.Add(OverlappingActor);
            }
        }

        // ���� ���鿡 ���ؼ��� ������ ����
        for (AActor* EnemyActor : UniqueEnemyActors) {
            AEnemy* UniqueEnemy = Cast<AEnemy>(EnemyActor);
            if (UniqueEnemy) {
                FVector PushDirection = UniqueEnemy->GetActorLocation() - SweepResult.ImpactPoint;
                PushDirection.Normalize();

                // TakeDamage �Լ� ȣ��
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