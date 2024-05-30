// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TowerButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UTowerButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ��ư�� �̹��� �ؽ�ó ����
	void SetTexture2D(UTexture2D* ImageTexture);
	void SetTexture2DFromPath(TCHAR* Path);

	// �ؽ�Ʈ ����
	void SetBottomText(FText NewText);
	void SetBottomTextFromString(FString NewString);
	void SetBottomTextFromInt(int32 NewInt);

	// ��ư Ȱ��ȭ ���� ����
	void SetButtonEnabled(bool IsEnabled);

	// ���� ���� ��ȯ
	class UButton* GetButton() { return Button; }

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Button;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text;
};
