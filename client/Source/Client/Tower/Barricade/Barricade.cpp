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

	// ü�� ����
	Hp -= DamageAmount;
	UE_LOG(LogTemp, Log, TEXT("Barricade Hp : %d"), Hp)

	// ü���� 0 �����̸� �ٸ����̵� �ı� �� �ı� ����� �˸�
	if (Hp <= 0)
	{
		OnDestroyBarricade.Broadcast(BarricadeDirection);
		Destroy();
	}

	return DamageAmount;
}

