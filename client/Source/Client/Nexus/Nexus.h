// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Nexus.generated.h"

UCLASS()
class CLIENT_API ANexus : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANexus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 피격
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void GameOver();

public:
	// 바리케이드 메쉬 컴포넌트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* NexusMeshComp;
	
	// 위젯 컴포넌트
	class UWidgetComponent* NexusHpBarComp;

	int32 Hp; // 체력

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 MaxHp; // 최대 체력
	
	// 테스트
	// void Test();
};
