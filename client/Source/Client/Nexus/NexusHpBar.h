// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NexusHpBar.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UNexusHpBar : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPercentage(float Percent);

public:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HPBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HpText;
};
