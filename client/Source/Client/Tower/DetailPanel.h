// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DetailPanel.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UDetailPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	// Image
	void SetTexture2D(UTexture2D* ImageTexture);
	void SetTexture2DFromPath(TCHAR* Path);

	// Title
	void SetTitle(FText NewText);
	void SetTitleFromString(FString NewString);

	// Cost
	void SetCostText(FText NewText);
	void SetCostTextFromInt(int32 NewInt);

	// Description
	void SetDescription(FText NewText);
	void SetDescriptionFromString(FString NewString);
	
private:
	UPROPERTY(meta = (BindWidget))
	class UImage* Image;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Title;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Cost;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Description;
};
