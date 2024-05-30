// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/MultiLineEditableTextBox.h"

// 이미지 텍스처 설정 (입력 : UTexture2D)
void UDetailPanel::SetTexture2D(UTexture2D* ImageTexture)
{
	Image->SetBrushFromTexture(ImageTexture, true);
}

// 이미지 텍스처 설정 (입력 : TCHAR* -> 파일 경로)
void UDetailPanel::SetTexture2DFromPath(TCHAR* Path)
{
	UTexture2D* LoadTexture = LoadObject<UTexture2D>(nullptr, Path);
	SetTexture2D(LoadTexture);
}

// Title 설정 (입력 : FText)
void UDetailPanel::SetTitle(FText NewText)
{
	Title->SetText(NewText);
}

// Title 설정 (입력 : FString)
void UDetailPanel::SetTitleFromString(FString NewString)
{
	Title->SetText(FText::FromString(NewString));
}

// Cost 설정 (입력 : FText)
void UDetailPanel::SetCostText(FText NewText)
{
	Cost->SetText(NewText);
}

// Cost 설정 (입력 : int32)
void UDetailPanel::SetCostTextFromInt(int32 NewInt)
{
	Cost->SetText(FText::FromString(FString::FromInt(NewInt)));
}

// Description 설정 (입력 : FText)
void UDetailPanel::SetDescription(FText NewText)
{
	Description->SetText(NewText);
}

// Description 설정 (입력 : FString)
void UDetailPanel::SetDescriptionFromString(FString NewString)
{
	Description->SetText(FText::FromString(NewString));
}