// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedTowerChecker.h"

// Sets default values
ASelectedTowerChecker::ASelectedTowerChecker()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
}

// Called when the game starts or when spawned
void ASelectedTowerChecker::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComp->SetStaticMesh(StaticMesh);
	StaticMeshComp->SetWorldScale3D(FVector(5.f, 5.f, 0.2f));
}

