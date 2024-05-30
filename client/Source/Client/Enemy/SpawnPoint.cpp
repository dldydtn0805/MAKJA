// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnPoint.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyWidget.h"
#include "Components/WidgetComponent.h"
#include "../ClientGameMode.h"

// Sets default values
ASpawnPoint::ASpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ASpawnPoint::Spawn(TSubclassOf<AEnemy> Enemy, int32 EnemyID, int32 Count)
{
	if (!Enemy)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy class is not valid"));
		return;
	}

	FVector Location = GetActorLocation(); // ���� ����Ʈ�� ���� ��ġ ���
	FRotator Rotation = GetActorRotation(); // ���� ����Ʈ�� ���� ȸ�� ���

	// ���� �Ķ���� ����
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


	for (int32 i = 0; i < Count; i++)
	{
		// ���ʹ� ����
		AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(Enemy, Location, Rotation, SpawnParams);
		SpawnedEnemy->Stat->SetEnemyStat(EnemyID);
		auto EnemyWidget = Cast<UEnemyWidget>(SpawnedEnemy->EnemyUI->GetUserWidgetObject());
		if (EnemyWidget)
			EnemyWidget->BindStat(SpawnedEnemy->Stat);
	}
}

// Called when the game starts or when spawned
void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}