// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyWidget.h"
#include "EnemyStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyWidget::BindStat(class UEnemyStatComponent* StatComp) {
	CurrentStatComp = StatComp;
	if (EnemyInfo) {
		FString InfoString = TEXT("Lv.") + FString::FromInt(StatComp->GetLevel()) + TEXT(" ") + StatComp->GetKrName();
		UE_LOG(LogTemp, Error, TEXT("Lv: %i, Name: %s"),StatComp->GetLevel(), *StatComp->GetKrName());
		EnemyInfo->SetText(FText::FromString(InfoString));
	}
	StatComp->OnHpChanged.AddUObject(this, &UEnemyWidget::UpdateHp);
}

void UEnemyWidget::UpdateHp() {
	if (CurrentStatComp.IsValid())
		PB_HPBar->SetPercent(CurrentStatComp->GetHpRatio());
}