// Fill out your copyright notice in the Description page of Project Settings.


#include "Barricade.h"
#include "NavModifierComponent.h"
#include "Components/ActorComponent.h"

// Sets default values
ABarricade::ABarricade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BarricadeMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarricadeMeshComp"));
	SetRootComponent(BarricadeMeshComp);
}

// Called when the game starts or when spawned
void ABarricade::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABarricade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABarricade::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (Hp <= 0) return 0.0f;

	// 체력 감소
	Hp -= DamageAmount;
	UE_LOG(LogTemp, Log, TEXT("Barricade Hp : %d"), Hp)

	// 체력이 0 이하이면 바리케이드 파괴 및 파괴 사실을 알림
	if (Hp <= 0)
	{
		OnDestroyBarricade.Broadcast(BarricadeDirection);
		Destroy();
	}

	return DamageAmount;
}

