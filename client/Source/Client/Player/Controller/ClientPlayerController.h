// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ClientPlayerController.generated.h"

// Ÿ���� ������ enum class
UENUM(BlueprintType)
enum class FTowerControlMode : uint8 {
	OFF UMETA(DisplayName="OFF"),
	INIT UMETA(DisplayName = "INIT"),
	BUILD UMETA(DisplayName = "BUILD"),
	SELECTED UMETA(DisplayName = "SELECTED"),
	BARRICADE UMETA(DisplayName = "BARRICADE"),
};

UCLASS()
class CLIENT_API AClientPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AClientPlayerController();

protected:
	// InputComponent ����
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	// �� �����Ӹ��� ����
	virtual void PlayerTick(float DeltaTime) override;

public:
	// ���� ���õ� Ÿ���� �ε���
	int32 SelectedIndex;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tower)
	TArray<TSubclassOf<class ATower>> TowerClasses;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Barricade)
	TSubclassOf<class ABarricade> BarricadeClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tower)
	TSubclassOf<class ATowerDetector> TowerSpawnDetector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tower)
	TSubclassOf<class ATowerDetector> TowerLevelUpDetector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Tower)
	TSubclassOf<class ASelectedTowerChecker> SelectedTowerChecker;

	// ���� ������ SpawningDetector
	class ATowerDetector* SpawningDetector = nullptr;

	// ������ ���� (false�� Ÿ�� ���� �� ���, true�� Ÿ�� ���� ���)
	FTowerControlMode TowerControlMode;

	// ���� ���õ� Ÿ��
	class ATower* SelectedTower;
	class ASelectedTowerChecker* SpawnedSelectedTowerChecker = nullptr;

	// ��� ��ȯ
	UFUNCTION(BlueprintCallable)
	void ChangeMode(FTowerControlMode NewMode, int32 SelectedTowerIndex = -1);

	void ReturnInit();

	// TowerControl �۵� ����
	bool IsActiveTowerControl() { return TowerControlMode != FTowerControlMode::OFF; }

	// ���� Ŭ���� ����Ǵ� �޼���
	void ClickLeft();

	// Ÿ�� ��Ʈ���� ������ �� Ŭ���ϸ� �۵��ϴ� �޼���
	void ClickAction();

	// Ÿ�� ����
	void SpawnTower();

	// ���� �������� Ÿ���� �����並 ����.
	void SetPreviewStaticMesh(int32 SelectedTowerIndex);

	// ���� ������Ʈ��
	class UWidgetComponent* TowerSelectWidgetComp;
	class UWidgetComponent* TowerLevelUpOrSellWidgetComp;
	class UWidgetComponent* BarricadeBuildWidgetComp;
	class UWidgetComponent* MessageWidgetComp;

public:
	// ��ư�� OnClicked �̺�Ʈ �Լ���
	UFUNCTION() void SelectBuildTower0() { ChangeMode(FTowerControlMode::BUILD, 0); }
	UFUNCTION() void SelectBuildTower1() { ChangeMode(FTowerControlMode::BUILD, 1); }
	UFUNCTION() void SelectBuildTower2() { ChangeMode(FTowerControlMode::BUILD, 2); }
	UFUNCTION() void SelectBuildTower3() { ChangeMode(FTowerControlMode::BUILD, 3); }
	UFUNCTION() void SelectBuildTower4() { ChangeMode(FTowerControlMode::BUILD, 4); }
	UFUNCTION() void SelectBuildTower5() { ChangeMode(FTowerControlMode::BUILD, 5); }
	UFUNCTION() void TowerLevelUp();
	UFUNCTION() void SellTower();
	UFUNCTION() void BuildBarricadeUp();
	UFUNCTION() void BuildBarricadeDown();
	UFUNCTION() void BuildBarricadeLeft();
	UFUNCTION() void BuildBarricadeRight();

	// �ٸ����̵� �ı� �̺�Ʈ
	void OnDestroyBarricade(enum class FBarricadeDirection BarricadeDirection);
};
