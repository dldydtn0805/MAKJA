// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerSelectWidget.h"
#include "TowerButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "DetailPanel.h"
#include "../ClientGameInstance.h"
#include "../Player/Magician/MagicianCharacter.h"
#include "Kismet/GameplayStatics.h"

void UTowerSelectWidget::NativeConstruct() {
	Super::NativeConstruct();

	int32 cnt = FMath::Min3(6, ButtonTextures.Num(), TowerIDs.Num());
	for (int i = 0; i < cnt; i++) {
		UTowerButtonWidget* ButtonWidget = GetButtonWidget(i);
		ButtonWidget->SetTexture2D(ButtonTextures[i]);
		
		auto ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		int32 Cost = ClientGameInstance->GetTowerStatData(TowerIDs[i], 1)->Cost;
		ButtonWidget->SetBottomTextFromInt(Cost);

		ButtonWidget->GetButton()->OnUnhovered.AddDynamic(this, &UTowerSelectWidget::UnhoveredButton);

		AMagicianCharacter* MagicianCharacter = Cast<AMagicianCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	GetButtonWidget(0)->GetButton()->OnHovered.AddDynamic(this, &UTowerSelectWidget::HoveredButton0);
	GetButtonWidget(1)->GetButton()->OnHovered.AddDynamic(this, &UTowerSelectWidget::HoveredButton1);
	GetButtonWidget(2)->GetButton()->OnHovered.AddDynamic(this, &UTowerSelectWidget::HoveredButton2);
	GetButtonWidget(3)->GetButton()->OnHovered.AddDynamic(this, &UTowerSelectWidget::HoveredButton3);
	GetButtonWidget(4)->GetButton()->OnHovered.AddDynamic(this, &UTowerSelectWidget::HoveredButton4);
	GetButtonWidget(5)->GetButton()->OnHovered.AddDynamic(this, &UTowerSelectWidget::HoveredButton5);
}

UTowerButtonWidget* UTowerSelectWidget::GetButtonWidget(int32 Index)
{
	return Cast<UTowerButtonWidget>(GetWidgetFromName(*FString::Printf(TEXT("TowerButton%d"), Index)));
}

UTextBlock* UTowerSelectWidget::GetReturnInitText()
{
	return Cast<UTextBlock>(GetWidgetFromName(TEXT("ReturnInitText")));
}

void UTowerSelectWidget::SetDetailPanelContentsFromIndex(int Index)
{
	auto ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	int32 Cost = ClientGameInstance->GetTowerStatData(TowerIDs[Index], 1)->Cost;

	DetailPanel->SetTexture2D(ButtonTextures[Index]);
	DetailPanel->SetTitleFromString(PanelTitles[Index]);
	DetailPanel->SetCostTextFromInt(Cost);
	DetailPanel->SetDescriptionFromString(PanelDescriptions[Index]);
}

// 호버 이벤트들
void UTowerSelectWidget::HoveredButton0() {
	SetDetailPanelContentsFromIndex(0);
	DetailPanel->SetVisibility(ESlateVisibility::Visible);
}

void UTowerSelectWidget::HoveredButton1() {
	SetDetailPanelContentsFromIndex(1);
	DetailPanel->SetVisibility(ESlateVisibility::Visible);
}

void UTowerSelectWidget::HoveredButton2() {
	SetDetailPanelContentsFromIndex(2);
	DetailPanel->SetVisibility(ESlateVisibility::Visible);
}

void UTowerSelectWidget::HoveredButton3() {
	SetDetailPanelContentsFromIndex(3);
	DetailPanel->SetVisibility(ESlateVisibility::Visible);
}

void UTowerSelectWidget::HoveredButton4() {
	SetDetailPanelContentsFromIndex(4);
	DetailPanel->SetVisibility(ESlateVisibility::Visible);
}
	
void UTowerSelectWidget::HoveredButton5() {
	SetDetailPanelContentsFromIndex(5);
	DetailPanel->SetVisibility(ESlateVisibility::Visible);
}

// 호버 해제 이벤트
void UTowerSelectWidget::UnhoveredButton() {
	DetailPanel->SetVisibility(ESlateVisibility::Collapsed);
}