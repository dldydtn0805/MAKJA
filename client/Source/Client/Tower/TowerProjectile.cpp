// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundWave.h"

// Sets default values
ATowerProjectile::ATowerProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �ʿ��� ��ü ����
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE"));

	CollisionComponent->InitSphereRadius(105.0f); // ��ü ������ ����
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile")); // ��ü�� ������Ÿ�� �ݸ��� ä�� ����
	RootComponent = CollisionComponent; // ��Ʈ ������Ʈ�� ����

	ProjectileParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PROJECTILE_PARTICLE"));
	ProjectileParticleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATowerProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent); // CollisionComponent ����
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed; // �ʱ� �ӵ�
	ProjectileMovementComponent->MaxSpeed = ProjectileSpeed; // �ִ� �ӵ�
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // Velocity�� ������ ���ߵ��� ȸ������ ������Ʈ 
	ProjectileMovementComponent->bShouldBounce = false; // �ٿ�� �ʿ����� �����Ƿ� false�� ����
	ProjectileMovementComponent->ProjectileGravityScale = 0; // �߷� ��Ȱ��ȭ

	SetLifeSpan(5.f); // �߻�ü�� ���� ����
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATowerProjectile::OnOverlapBegin); // �浹�� �浹 ������ �����ϵ��� ����

	ColorVector = FVector(ProjectileColor.R, ProjectileColor.G, ProjectileColor.B);
	ProjectileParticleComponent->SetVectorParameter(TEXT("Color"), ColorVector);
}

// Called every frame
void ATowerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATowerProjectile::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �浹�� ����� ĳ���͸� �ǰ� ���� ����
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy && Enemy->Stat->GetHp() > 0) {
		FDamageEvent DamageEvent;
		Enemy->TakeDamage(1.f * Attack, DamageEvent, GetInstigatorController(), this);

		// �ǰ� �������� ���� ȿ��
		UParticleSystemComponent* DamageParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamageParticle, SweepResult.ImpactPoint, FRotator::ZeroRotator, FVector(3.0f, 3.0f, 3.0f));
		DamageParticleComponent->SetVectorParameter(TEXT("Color"), ColorVector);

		// �߻�ü �ı�
		Destroy();
	}
}

// �߻�ü�� ������ �߻� �������� �߻�
void ATowerProjectile::FireInDirection(const FVector& ShootDir)
{
	// �߻� �������� �߻� ȿ��
	UParticleSystemComponent* MuzzleParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticle, GetActorLocation(), FRotator::ZeroRotator, FVector(3.0f, 3.0f, 3.0f));
	MuzzleParticleComponent->SetVectorParameter(TEXT("Color"), ColorVector);

	ProjectileMovementComponent->Velocity = ShootDir * ProjectileMovementComponent->InitialSpeed;
	
	// ȿ���� ���
	if (ShootSoundWave) {
		ShootSoundComponent = UGameplayStatics::SpawnSound2D(this, ShootSoundWave);
		if (ShootSoundComponent) {
			ShootSoundComponent->SetVolumeMultiplier(AudioVolumeMultiplier); // ���� ����
			ShootSoundComponent->Play();
		}
	}
}

