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
	// 버튼의 이미지 텍스처 설정
	void SetTexture2D(UTexture2D* ImageTexture);
	void SetTexture2DFromPath(TCHAR* Path);

	// 텍스트 설정
	void SetBottomText(FText NewText);
	void SetBottomTextFromString(FString NewString);
	void SetBottomTextFromInt(int32 NewInt);

	// 버튼 활성화 상태 변경
	void SetButtonEnabled(bool IsEnabled);

	// 위젯 참조 반환
	class UButton* GetButton() { return Button; }

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Button;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text;
};
