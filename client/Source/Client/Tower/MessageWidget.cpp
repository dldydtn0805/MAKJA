// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageWidget.h"
#include "Components/TextBlock.h"

// 메시지 보여주기
void UMessageWidget::ShowMessage(FString MessageString, float Lifetime) {
	// 작동중인 타이머가 있다면 제거
	GetWorld()->GetTimerManager().ClearTimer(MessageTimerHandle);

	// 메시지 세팅 후 보여주기
	Message->SetText(FText::FromString(MessageString));
	Message->SetVisibility(ESlateVisibility::Visible);

	// Lifetime이 양수라면 타이머 세팅
	if (Lifetime > 0.f) {
		GetWorld()->GetTimerManager().SetTimer(
			MessageTimerHandle,
			FTimerDelegate::CreateLambda([&]() {
				Message->SetVisibility(ESlateVisibility::Collapsed);
			}),
			Lifetime,
			false
		);
	}
}