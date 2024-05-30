// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// 버튼의 이미지 텍스처 설정 (입력 : UTexture2D)
void UTowerButtonWidget::SetTexture2D(UTexture2D* ImageTexture) {
	Image->SetBrushFromTexture(ImageTexture, true);
}

// 버튼의 이미지 텍스처 설정 (입력 : TCHAR* -> 파일 경로)
void UTowerButtonWidget::SetTexture2DFromPath(TCHAR* Path)
{
	UTexture2D* LoadTexture = LoadObject<UTexture2D>(nullptr, Path);
	SetTexture2D(LoadTexture);
}

// 텍스트 설정 (입력 : FText)
void UTowerButtonWidget::SetBottomText(FText NewText)
{
	Text->SetText(NewText);
}

// 텍스트 설정 (입력 : FString)
void UTowerButtonWidget::SetBottomTextFromString(FString NewString)
{
	SetBottomText(FText::FromString(NewString));
}

// 텍스트 설정 (입력 : int32)
void UTowerButtonWidget::SetBottomTextFromInt(int32 NewInt)
{
	SetBottomText(FText::FromString(FString::FromInt(NewInt)));
}

void UTowerButtonWidget::SetButtonEnabled(bool IsEnabled)
{
	Button->SetIsEnabled(IsEnabled);
}
