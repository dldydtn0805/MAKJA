// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tower.h"
#include "AOETower.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API AAOETower : public ATower
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAOETower();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UFUNCTION()
	void CheckEnemy(); // 적이 있는지 체크

	UFUNCTION()
	void StartAttack(); // 공격 시작

	UFUNCTION()
	void DoAttack(TArray<FOverlapResult> OverlapResults); // 공격 실시

	UFUNCTION()
	void StopAttack(); // 공격 중지

	// 파티클 스폰/제거
	UFUNCTION()
	UParticleSystemComponent* SpawnAttackParticle(float AttackRange);
	UFUNCTION()
	void DestroyAttackParticle();

public:
	virtual void LevelUp() override; // 레벨 업
	virtual void Sell() override; // 판매

private:
	bool isAttack = false; // 공격 상태

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* AttackParticle; // 공격 시 나오는 파티클
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector ScalePerRange; // 공격 범위와 파티클 사이즈를 맞추기 위한 상수

	// 공격 스폰 효과음
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	//class USoundWave* SpawnSoundWave;

	//// 소리 배율
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	//float AudioVolumeMultiplier = 1.0f;

	//// 소리 루프 여부
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	//bool AudioLoop;

	//// 피치 배율 (음원 속도 조절)
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	//float AudioPitchMultiplier = 1.0f;

private:
	FTimerHandle TimerHandle;

	// 현재 스폰중인 공격 파티클 포인터
	class UParticleSystemComponent* SpawnedAttackParticle = nullptr;

	// 발사 소리가 담길 오디오 컴포넌트
	/*class UAudioComponent* SpawnSoundComponent;*/
};
