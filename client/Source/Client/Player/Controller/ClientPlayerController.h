// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ClientPlayerController.generated.h"

// 타워와 관련한 enum class
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
	// InputComponent 설정
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

	// 매 프레임마다 동작
	virtual void PlayerTick(float DeltaTime) override;

public:
	// 현재 선택된 타워의 인덱스
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

	// 현재 스폰된 SpawningDetector
	class ATowerDetector* SpawningDetector = nullptr;

	// 빌드모드 여부 (false면 타워 레벨 업 모드, true면 타워 짓기 모드)
	FTowerControlMode TowerControlMode;

	// 현재 선택된 타워
	class ATower* SelectedTower;
	class ASelectedTowerChecker* SpawnedSelectedTowerChecker = nullptr;

	// 모드 전환
	UFUNCTION(BlueprintCallable)
	void ChangeMode(FTowerControlMode NewMode, int32 SelectedTowerIndex = -1);

	void ReturnInit();

	// TowerControl 작동 여부
	bool IsActiveTowerControl() { return TowerControlMode != FTowerControlMode::OFF; }

	// 왼쪽 클릭시 실행되는 메서드
	void ClickLeft();

	// 타워 컨트롤이 동작할 때 클릭하면 작동하는 메서드
	void ClickAction();

	// 타워 스폰
	void SpawnTower();

	// 현재 선택중인 타워로 프리뷰를 띄운다.
	void SetPreviewStaticMesh(int32 SelectedTowerIndex);

	// 위젯 컴포넌트들
	class UWidgetComponent* TowerSelectWidgetComp;
	class UWidgetComponent* TowerLevelUpOrSellWidgetComp;
	class UWidgetComponent* BarricadeBuildWidgetComp;
	class UWidgetComponent* MessageWidgetComp;

public:
	// 버튼의 OnClicked 이벤트 함수들
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

	// 바리케이드 파괴 이벤트
	void OnDestroyBarricade(enum class FBarricadeDirection BarricadeDirection);
};
