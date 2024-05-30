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

	//콜리전 컴포넌트 
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	class USphereComponent* CollisionComponent;

	// 스폰된 Detector Actor
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	UStaticMeshComponent* SpawnDetector;

	// 스폰된 Detector Actor
	UPROPERTY()
	UStaticMeshComponent* TowerPreviewComponent;

	// 감지 범위 (서로 다른 탑 사이의 간격은 최소 DetectRange임이 보장 됨)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Detector)
	float DetectRange;

	// 설치가 가능할 때 사용하는 메쉬
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Detector)
	UStaticMesh* DetectMesh;

	// 설치가 불가능할 때 사용하는 메쉬
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Detector)
	UStaticMesh* NotDetectMesh;

private:
	std::atomic<int32> DetectedTowerNum;
	bool DetectedNexus;

	void InitDetectedStatus();

public:
	// 감지된 탑의 수 초기화
	int32 GetDetectedTowerNum() { return DetectedTowerNum.load(); }

	// 넥서스 감지여부 확인
	bool IsDetectedNexus() { return DetectedNexus; }

	// 오버랩 된 타워 중 하나를 반환
	class ATower* GetOverlappedTowerOnce();
};
