// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UniformGridPanel.h"
#include "Blueprint/UserWidget.h"
#include "TowerSelectWidget.generated.h"

UCLASS()
class CLIENT_API UTowerSelectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

private:
	void SetDetailPanelContentsFromIndex(int Index);

private:
	// 호버 이벤트
	UFUNCTION() void HoveredButton0();
	UFUNCTION() void HoveredButton1();
	UFUNCTION() void HoveredButton2();
	UFUNCTION() void HoveredButton3();
	UFUNCTION() void HoveredButton4();
	UFUNCTION() void HoveredButton5();

	// 호버 해제 이벤트
	UFUNCTION() void UnhoveredButton();

public:
	class UUniformGridPanel* GetTowerButtonsGridPanel() { return TowerButtons; };
	class UTowerButtonWidget* GetButtonWidget(int32 Index);
	class UTextBlock* GetReturnInitText();

private:
	UPROPERTY(meta = (BindWidget))
	class UDetailPanel* DetailPanel;

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* TowerButtons;

public:
	// 버튼에 사용될 텍스처들
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	TArray<UTexture2D*> ButtonTextures;

	// 각 버튼에 대응되는 타워의 ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TowerID)
	TArray<FString> TowerIDs;

	// 디테일 패널에 띄울 타이틀
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TowerID)
	TArray<FString> PanelTitles;

	// 디테일 패널에 띄울 세부 내용
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TowerID)
	TArray<FString> PanelDescriptions;
};
