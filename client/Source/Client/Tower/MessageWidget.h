// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MessageWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UMessageWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Message;

public:
	// 메시지 보여주기
	UFUNCTION() void ShowMessage(FString MessageString, float Lifetime = -1.f);

private:
	FTimerHandle MessageTimerHandle;
};
