// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerLevelUpOrSellWidget.h"
#include "TowerButtonWidget.h"
#include "Tower.h"
#include "DetailPanel.h"
#include "../ClientGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UTowerLevelUpOrSellWidget::NativeConstruct() {
	Super::NativeConstruct();
	
	LevelUp->SetTexture2D(LevelUpButtonTexture);
	LevelUp->GetButton()->OnHovered.AddDynamic(this, &UTowerLevelUpOrSellWidget::HoveredLevelUpButton);
	LevelUp->GetButton()->OnUnhovered.AddDynamic(this, &UTowerLevelUpOrSellWidget::UnhoveredButton);
	
	Sell->SetTexture2D(SellButtonTexture);
	Sell->GetButton()->OnHovered.AddDynamic(this, &UTowerLevelUpOrSellWidget::HoveredSellButton);
	Sell->GetButton()->OnUnhovered.AddDynamic(this, &UTowerLevelUpOrSellWidget::UnhoveredButton);
}

// 선택된 타워를 토대로 위젯 세팅
void UTowerLevelUpOrSellWidget::BindTower(ATower* SelectedTower)
{
	Tower = SelectedTower;
	UpdateWidget();
}

// 위젯 엡데이트
void UTowerLevelUpOrSellWidget::UpdateWidget()
{
	Sell->SetBottomTextFromString(TEXT("Sell"));

	if (Tower->GetTowerStatComponent()->IsMaxLevel()) {
		LevelUp->SetButtonEnabled(false);
		LevelUp->SetBottomTextFromString(TEXT("MAX"));
	}
	else {
		int32 LevelUpCost = Tower->GetTowerStatComponent()->GetCost();
		LevelUp->SetButtonEnabled(true);
		LevelUp->SetBottomTextFromInt(LevelUpCost);
	}
}

// 호버 이벤트
void UTowerLevelUpOrSellWidget::HoveredLevelUpButton()
{
	DetailPanel->SetVisibility(ESlateVisibility::Visible);

	int32 TowerLevel = Tower->GetTowerStatComponent()->GetLevel();

	UClientGameInstance* ClientGameInstance = ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	auto CurrentTowerStatData = ClientGameInstance->GetTowerStatData(Tower->GetTowerId(), TowerLevel);
	auto NextTowerStatData = ClientGameInstance->GetTowerStatData(Tower->GetTowerId(), TowerLevel + 1);

	DetailPanel->SetTexture2D(LevelUpButtonTexture);
	DetailPanel->SetTitleFromString(LevelUpTitle);
	DetailPanel->SetCostTextFromInt(Tower->GetTowerStatComponent()->GetCost());
	DetailPanel->SetDescriptionFromString(
		LevelUpDescription
		+ FString::Printf(TEXT("\nLv.%d -> Lv.%d"), TowerLevel, TowerLevel + 1)
		+ FString::Printf(TEXT("\n\n공격력 : %d -> %d"), CurrentTowerStatData->Attack, NextTowerStatData->Attack)
		+ FString::Printf(TEXT("\n공격 범위 : %d -> %d"), CurrentTowerStatData->AttackRange, NextTowerStatData->AttackRange)
		+ FString::Printf(TEXT("\n공격 속도 : %.2f -> %.2f"), CurrentTowerStatData->AttackSpeed, NextTowerStatData->AttackSpeed)
		+ FString::Printf(TEXT("\n안정치 : %d -> %d"), CurrentTowerStatData->Stable, NextTowerStatData->Stable)
	);
}

void UTowerLevelUpOrSellWidget::HoveredSellButton()
{
	DetailPanel->SetVisibility(ESlateVisibility::Visible);
	
	DetailPanel->SetTexture2D(SellButtonTexture);
	DetailPanel->SetTitleFromString(SellTitle);
	DetailPanel->SetCostTextFromInt(Tower->GetTowerStatComponent()->SellCost());
	DetailPanel->SetDescriptionFromString(SellDescription);
}

// 호버 해제 이벤트
void UTowerLevelUpOrSellWidget::UnhoveredButton()
{
	DetailPanel->SetVisibility(ESlateVisibility::Collapsed);
}