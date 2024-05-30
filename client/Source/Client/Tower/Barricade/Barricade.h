// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Barricade.generated.h"

UENUM()
enum class FBarricadeDirection {
	UP, DOWN, LEFT, RIGHT
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDestroyBarricade, FBarricadeDirection)
UCLASS()
class CLIENT_API ABarricade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABarricade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// �ǰ�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void SetBarricadeDirection(FBarricadeDirection NewDirection) { BarricadeDirection = NewDirection; }
	int32 GetCost() { return Cost;  }

protected:
	// �ٸ����̵� ����
	FBarricadeDirection BarricadeDirection;

	// �ٸ����̵� �޽� ������Ʈ
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponent* BarricadeMeshComp;

	// ü��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Hp;

	// ��ġ ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 Cost;

public:
	FOnDestroyBarricade OnDestroyBarricade;
};
