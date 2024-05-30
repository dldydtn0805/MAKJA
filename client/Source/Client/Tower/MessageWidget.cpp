// Fill out your copyright notice in the Description page of Project Settings.


#include "MessageWidget.h"
#include "Components/TextBlock.h"

// �޽��� �����ֱ�
void UMessageWidget::ShowMessage(FString MessageString, float Lifetime) {
	// �۵����� Ÿ�̸Ӱ� �ִٸ� ����
	GetWorld()->GetTimerManager().ClearTimer(MessageTimerHandle);

	// �޽��� ���� �� �����ֱ�
	Message->SetText(FText::FromString(MessageString));
	Message->SetVisibility(ESlateVisibility::Visible);

	// Lifetime�� ������ Ÿ�̸� ����
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