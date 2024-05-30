// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectedTowerChecker.generated.h"

UCLASS()
class CLIENT_API ASelectedTowerChecker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASelectedTowerChecker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UStaticMesh* StaticMesh;
};
