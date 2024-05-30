// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerLevelUpOrSellWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UTowerLevelUpOrSellWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UTowerButtonWidget* LevelUp;

	UPROPERTY(meta = (BindWidget))
	class UTowerButtonWidget* Sell;

	UPROPERTY(meta = (BindWidget))
		class UDetailPanel* DetailPanel;

public:
	// 위젯에 바인딩 된 타워
	class ATower* Tower;

	// 강화 버튼에 사용할 텍스처
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	UTexture2D* LevelUpButtonTexture;

	// 판매 버튼에 사용할 텍스처
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	UTexture2D* SellButtonTexture;

	// 강화 타이틀
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PanelContents)
	FString LevelUpTitle;

	// 강화 세부 내용
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PanelContents)
	FString LevelUpDescription;

	// 판매 타이틀
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PanelContents)
	FString SellTitle;

	// 판매 세부 내용
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PanelContents)
	FString SellDescription;

private:
	// 위젯 엡데이트
	void UpdateWidget();

	// 호버 이벤트
	UFUNCTION() void HoveredLevelUpButton();
	UFUNCTION() void HoveredSellButton();

	// 호버 해제 이벤트
	UFUNCTION() void UnhoveredButton();

public:
	// 위젯에 타워를 바인딩
	void BindTower(class ATower* Tower);

	// 강화 버튼 반환
	UTowerButtonWidget* GetLevelUpButtonWidget() { return LevelUp; }

	// 판매 버튼 반환
	UTowerButtonWidget* GetSellButtonWidget() { return Sell; }
};
