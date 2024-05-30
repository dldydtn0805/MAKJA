// Fill out your copyright notice in the Description page of Project Settings.

#include "GameExitConfirmWidget.h"
#include "GenericPlatform/GenericPlatformMisc.h"

void UGameExitConfirmWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ExitBtn && CancelBtn)
	{
		ExitBtn->OnClicked.AddDynamic(this, &UGameExitConfirmWidget::ExitBtnCallback);
		CancelBtn->OnClicked.AddDynamic(this, &UGameExitConfirmWidget::CancelCallback);
	}
}

void UGameExitConfirmWidget::ExitBtnCallback()
{
	UE_LOG(LogTemp, Log, TEXT("Exit Btn Call Back"));
	FGenericPlatformMisc::RequestExit(false);

}

void UGameExitConfirmWidget::CancelCallback()
{
	UE_LOG(LogTemp, Log, TEXT("Exit Btn Call Back"));
	SetVisibility(ESlateVisibility::Hidden);
	// RemoveFromViewport();
}
