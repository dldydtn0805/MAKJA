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
	void CheckEnemy(); // ���� �ִ��� üũ

	UFUNCTION()
	void StartAttack(); // ���� ����

	UFUNCTION()
	void DoAttack(TArray<FOverlapResult> OverlapResults); // ���� �ǽ�

	UFUNCTION()
	void StopAttack(); // ���� ����

	// ��ƼŬ ����/����
	UFUNCTION()
	UParticleSystemComponent* SpawnAttackParticle(float AttackRange);
	UFUNCTION()
	void DestroyAttackParticle();

public:
	virtual void LevelUp() override; // ���� ��
	virtual void Sell() override; // �Ǹ�

private:
	bool isAttack = false; // ���� ����

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* AttackParticle; // ���� �� ������ ��ƼŬ
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FVector ScalePerRange; // ���� ������ ��ƼŬ ����� ���߱� ���� ���

	// ���� ���� ȿ����
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	//class USoundWave* SpawnSoundWave;

	//// �Ҹ� ����
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	//float AudioVolumeMultiplier = 1.0f;

	//// �Ҹ� ���� ����
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	//bool AudioLoop;

	//// ��ġ ���� (���� �ӵ� ����)
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Audio)
	//float AudioPitchMultiplier = 1.0f;

private:
	FTimerHandle TimerHandle;

	// ���� �������� ���� ��ƼŬ ������
	class UParticleSystemComponent* SpawnedAttackParticle = nullptr;

	// �߻� �Ҹ��� ��� ����� ������Ʈ
	/*class UAudioComponent* SpawnSoundComponent;*/
};
