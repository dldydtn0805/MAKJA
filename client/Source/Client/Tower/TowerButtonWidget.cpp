// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// ��ư�� �̹��� �ؽ�ó ���� (�Է� : UTexture2D)
void UTowerButtonWidget::SetTexture2D(UTexture2D* ImageTexture) {
	Image->SetBrushFromTexture(ImageTexture, true);
}

// ��ư�� �̹��� �ؽ�ó ���� (�Է� : TCHAR* -> ���� ���)
void UTowerButtonWidget::SetTexture2DFromPath(TCHAR* Path)
{
	UTexture2D* LoadTexture = LoadObject<UTexture2D>(nullptr, Path);
	SetTexture2D(LoadTexture);
}

// �ؽ�Ʈ ���� (�Է� : FText)
void UTowerButtonWidget::SetBottomText(FText NewText)
{
	Text->SetText(NewText);
}

// �ؽ�Ʈ ���� (�Է� : FString)
void UTowerButtonWidget::SetBottomTextFromString(FString NewString)
{
	SetBottomText(FText::FromString(NewString));
}

// �ؽ�Ʈ ���� (�Է� : int32)
void UTowerButtonWidget::SetBottomTextFromInt(int32 NewInt)
{
	SetBottomText(FText::FromString(FString::FromInt(NewInt)));
}

void UTowerButtonWidget::SetButtonEnabled(bool IsEnabled)
{
	Button->SetIsEnabled(IsEnabled);
}
