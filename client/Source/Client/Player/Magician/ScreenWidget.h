// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "ScreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UScreenWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// HP 관련
	void BindHp(class UMagicianStatComponent* StatComp);
	void UpdateHp();
	// Level 관련
	void BindPlayerLevel(class UMagicianStatComponent* StatComp);
	void UpdatePlayerLevel();
	// Weapon Level 관련
	void BindWeaponLevel(class UMagicianStatComponent* StatComp);
	void UpdateWeaponLevel();
	// Money 관련
	void BindMoney(class UMagicianStatComponent* StatComp);
	void UpdateMoney();

	// Life 관련
	void BindLife(class UMagicianStatComponent* StatComp);
	void UpdateLife();

	// MonterKillCount
	void BindMonsterKillCount(int32 CurrentEnemy, int32 TotalEnemy);

	// CurrentStage
	void BindCurrentStage(int32 CurrentWave);

	// Skill CoolTime 관련
	void BindRightCoolTime(class UMagicianStatComponent* StatComp);
	void UpdateRightCoolTime(float RemainingTime);
	void BindShiftCoolTime(class UMagicianStatComponent* StatComp);
	void UpdateShiftCoolTime(float RemainingTime);
	void BindQCoolTime(class UMagicianStatComponent* StatComp);
	void UpdateQCoolTime(float RemainingTime);
	void BindECoolTime(class UMagicianStatComponent* StatComp);
	void UpdateECoolTime(float RemainingTime);
	void BindRCoolTime(class UMagicianStatComponent* StatComp);
	void UpdateRCoolTime(float RemainingTime);


	// LevelUpBtn 클릭 이벤트 핸들러
	UFUNCTION()
	void OnPlayerLevelUpBtnClicked();
	// WeaponLevelUpBtn 클릭 이벤트 핸들러
	UFUNCTION()
	void OnWeaponLevelUpBtnClicked();

protected:
	// 위젯 생성 시 호출되는 함수
	virtual void NativeConstruct() override;
	//virtual void SetupInputComponent();

public:
	class UMagicianStatComponent* CurrentStatComp;
	class UClientGameInstance* CurrentGameInst;
	class UClientGameMode* GM;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HpBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HpNow;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HpMax;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* PlayerLevelUpBtn;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* WeaponLevelUpBtn;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* PlayerLevel;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* LevelUpCost;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* WeaponLevel;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* WeaponLevelUpCost;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* WeaponLevelUpPercentage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* RightCoolTime;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ShiftCoolTime;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* QCoolTime;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ECoolTime;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* RCoolTime;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* RightBorder;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* ShiftBorder;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* QBorder;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* EBorder;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UBorder* RBorder;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RightImage;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* QImage;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* EImage;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* RImage;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Money;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* Life;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* MonsterKillCount;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* CurrentStage;
};
