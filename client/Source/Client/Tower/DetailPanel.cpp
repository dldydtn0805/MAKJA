// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/MultiLineEditableTextBox.h"

// �̹��� �ؽ�ó ���� (�Է� : UTexture2D)
void UDetailPanel::SetTexture2D(UTexture2D* ImageTexture)
{
	Image->SetBrushFromTexture(ImageTexture, true);
}

// �̹��� �ؽ�ó ���� (�Է� : TCHAR* -> ���� ���)
void UDetailPanel::SetTexture2DFromPath(TCHAR* Path)
{
	UTexture2D* LoadTexture = LoadObject<UTexture2D>(nullptr, Path);
	SetTexture2D(LoadTexture);
}

// Title ���� (�Է� : FText)
void UDetailPanel::SetTitle(FText NewText)
{
	Title->SetText(NewText);
}

// Title ���� (�Է� : FString)
void UDetailPanel::SetTitleFromString(FString NewString)
{
	Title->SetText(FText::FromString(NewString));
}

// Cost ���� (�Է� : FText)
void UDetailPanel::SetCostText(FText NewText)
{
	Cost->SetText(NewText);
}

// Cost ���� (�Է� : int32)
void UDetailPanel::SetCostTextFromInt(int32 NewInt)
{
	Cost->SetText(FText::FromString(FString::FromInt(NewInt)));
}

// Description ���� (�Է� : FText)
void UDetailPanel::SetDescription(FText NewText)
{
	Description->SetText(NewText);
}

// Description ���� (�Է� : FString)
void UDetailPanel::SetDescriptionFromString(FString NewString)
{
	Description->SetText(FText::FromString(NewString));
}