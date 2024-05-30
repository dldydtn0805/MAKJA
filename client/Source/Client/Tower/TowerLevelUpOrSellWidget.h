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
	// ������ ���ε� �� Ÿ��
	class ATower* Tower;

	// ��ȭ ��ư�� ����� �ؽ�ó
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	UTexture2D* LevelUpButtonTexture;

	// �Ǹ� ��ư�� ����� �ؽ�ó
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	UTexture2D* SellButtonTexture;

	// ��ȭ Ÿ��Ʋ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PanelContents)
	FString LevelUpTitle;

	// ��ȭ ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PanelContents)
	FString LevelUpDescription;

	// �Ǹ� Ÿ��Ʋ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PanelContents)
	FString SellTitle;

	// �Ǹ� ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PanelContents)
	FString SellDescription;

private:
	// ���� ������Ʈ
	void UpdateWidget();

	// ȣ�� �̺�Ʈ
	UFUNCTION() void HoveredLevelUpButton();
	UFUNCTION() void HoveredSellButton();

	// ȣ�� ���� �̺�Ʈ
	UFUNCTION() void UnhoveredButton();

public:
	// ������ Ÿ���� ���ε�
	void BindTower(class ATower* Tower);

	// ��ȭ ��ư ��ȯ
	UTowerButtonWidget* GetLevelUpButtonWidget() { return LevelUp; }

	// �Ǹ� ��ư ��ȯ
	UTowerButtonWidget* GetSellButtonWidget() { return Sell; }
};
