// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerDetector.generated.h"

UCLASS()
class CLIENT_API ATowerDetector : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ATowerDetector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//�ݸ��� ������Ʈ 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	class USphereComponent* CollisionComponent;

	// ������ Detector Actor
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	UStaticMeshComponent* SpawnDetector;

	// ������ Detector Actor
	UPROPERTY()
	UStaticMeshComponent* TowerPreviewComponent;

	// ���� ���� (���� �ٸ� ž ������ ������ �ּ� DetectRange���� ���� ��)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Detector)
	float DetectRange;

	// ��ġ�� ������ �� ����ϴ� �޽�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Detector)
	UStaticMesh* DetectMesh;

	// ��ġ�� �Ұ����� �� ����ϴ� �޽�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Detector)
	UStaticMesh* NotDetectMesh;

private:
	std::atomic<int32> DetectedTowerNum;
	bool DetectedNexus;

	void InitDetectedStatus();

public:
	// ������ ž�� �� �ʱ�ȭ
	int32 GetDetectedTowerNum() { return DetectedTowerNum.load(); }

	// �ؼ��� �������� Ȯ��
	bool IsDetectedNexus() { return DetectedNexus; }

	// ������ �� Ÿ�� �� �ϳ��� ��ȯ
	class ATower* GetOverlappedTowerOnce();
};
