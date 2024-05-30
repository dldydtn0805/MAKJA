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
	// ȣ�� �̺�Ʈ
	UFUNCTION() void HoveredButton0();
	UFUNCTION() void HoveredButton1();
	UFUNCTION() void HoveredButton2();
	UFUNCTION() void HoveredButton3();
	UFUNCTION() void HoveredButton4();
	UFUNCTION() void HoveredButton5();

	// ȣ�� ���� �̺�Ʈ
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
	// ��ư�� ���� �ؽ�ó��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	TArray<UTexture2D*> ButtonTextures;

	// �� ��ư�� �����Ǵ� Ÿ���� ID
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TowerID)
	TArray<FString> TowerIDs;

	// ������ �гο� ��� Ÿ��Ʋ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TowerID)
	TArray<FString> PanelTitles;

	// ������ �гο� ��� ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = TowerID)
	TArray<FString> PanelDescriptions;
};
