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

	// 필요한 객체 생성
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SPHERE"));
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE"));

	CollisionComponent->InitSphereRadius(105.0f); // 구체 반지름 설정
	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile")); // 구체에 프로젝타일 콜리전 채널 설정
	RootComponent = CollisionComponent; // 루트 컴포넌트로 지정

	ProjectileParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("PROJECTILE_PARTICLE"));
	ProjectileParticleComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATowerProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent); // CollisionComponent 설정
	ProjectileMovementComponent->InitialSpeed = ProjectileSpeed; // 초기 속도
	ProjectileMovementComponent->MaxSpeed = ProjectileSpeed; // 최대 속도
	ProjectileMovementComponent->bRotationFollowsVelocity = true; // Velocity와 방향을 맞추도록 회전값을 업데이트 
	ProjectileMovementComponent->bShouldBounce = false; // 바운스는 필요하지 않으므로 false로 설정
	ProjectileMovementComponent->ProjectileGravityScale = 0; // 중력 비활성화

	SetLifeSpan(5.f); // 발사체의 수명 설정
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ATowerProjectile::OnOverlapBegin); // 충돌시 충돌 로직을 수행하도록 설정

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
	// 충돌한 대상이 캐릭터면 피격 로직 수행
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy && Enemy->Stat->GetHp() > 0) {
		FDamageEvent DamageEvent;
		Enemy->TakeDamage(1.f * Attack, DamageEvent, GetInstigatorController(), this);

		// 피격 지점에서 폭발 효과
		UParticleSystemComponent* DamageParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DamageParticle, SweepResult.ImpactPoint, FRotator::ZeroRotator, FVector(3.0f, 3.0f, 3.0f));
		DamageParticleComponent->SetVectorParameter(TEXT("Color"), ColorVector);

		// 발사체 파괴
		Destroy();
	}
}

// 발사체를 지정한 발사 방향으로 발사
void ATowerProjectile::FireInDirection(const FVector& ShootDir)
{
	// 발사 지점에서 발사 효과
	UParticleSystemComponent* MuzzleParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticle, GetActorLocation(), FRotator::ZeroRotator, FVector(3.0f, 3.0f, 3.0f));
	MuzzleParticleComponent->SetVectorParameter(TEXT("Color"), ColorVector);

	ProjectileMovementComponent->Velocity = ShootDir * ProjectileMovementComponent->InitialSpeed;
	
	// 효과음 재생
	if (ShootSoundWave) {
		ShootSoundComponent = UGameplayStatics::SpawnSound2D(this, ShootSoundWave);
		if (ShootSoundComponent) {
			ShootSoundComponent->SetVolumeMultiplier(AudioVolumeMultiplier); // 볼륨 조정
			ShootSoundComponent->Play();
		}
	}
}

