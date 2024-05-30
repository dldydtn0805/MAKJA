// Fill out your copyright notice in the Description page of Project Settings.


#include "BarricadeBuildWidget.h"
#include "Barricade.h"
#include "../TowerButtonWidget.h"
#include "../DetailPanel.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UBarricadeBuildWidget::NativeConstruct() {
	Super::NativeConstruct();

	BuildUp->SetTexture2D(BuildUpTexture);
	BuildUp->GetButton()->OnHovered.AddDynamic(this, &UBarricadeBuildWidget::HoveredBuildUpButton);
	BuildUp->GetButton()->OnUnhovered.AddDynamic(this, &UBarricadeBuildWidget::UnhoveredButton);

	BuildDown->SetTexture2D(BuildDownTexture);
	BuildDown->GetButton()->OnHovered.AddDynamic(this, &UBarricadeBuildWidget::HoveredBuildDownButton);
	BuildDown->GetButton()->OnUnhovered.AddDynamic(this, &UBarricadeBuildWidget::UnhoveredButton);

	BuildLeft->SetTexture2D(BuildLeftTexture);
	BuildLeft->GetButton()->OnHovered.AddDynamic(this, &UBarricadeBuildWidget::HoveredBuildLeftButton);
	BuildLeft->GetButton()->OnUnhovered.AddDynamic(this, &UBarricadeBuildWidget::UnhoveredButton);
	
	BuildRight->SetTexture2D(BuildRightTexture);
	BuildRight->GetButton()->OnHovered.AddDynamic(this, &UBarricadeBuildWidget::HoveredBuildRightButton);
	BuildRight->GetButton()->OnUnhovered.AddDynamic(this, &UBarricadeBuildWidget::UnhoveredButton);
}

void UBarricadeBuildWidget::SetBarricadeCost(int32 Cost) {
	BuildUp->SetBottomTextFromInt(Cost);
	BuildDown->SetBottomTextFromInt(Cost);
	BuildLeft->SetBottomTextFromInt(Cost);
	BuildRight->SetBottomTextFromInt(Cost);
	DetailPanel->SetCostTextFromInt(Cost);
}

void UBarricadeBuildWidget::HoveredBuildUpButton()
{
	DetailPanel->SetVisibility(ESlateVisibility::Visible);

	DetailPanel->SetTexture2D(BuildUpTexture);
	DetailPanel->SetTitleFromString(BuildUpTitle);
	DetailPanel->SetDescriptionFromString(BuildUpDescription);
}

void UBarricadeBuildWidget::HoveredBuildDownButton()
{
	DetailPanel->SetVisibility(ESlateVisibility::Visible);

	DetailPanel->SetTexture2D(BuildDownTexture);
	DetailPanel->SetTitleFromString(BuildDownTitle);
	DetailPanel->SetDescriptionFromString(BuildDownDescription);
}

void UBarricadeBuildWidget::HoveredBuildLeftButton()
{
	DetailPanel->SetVisibility(ESlateVisibility::Visible);

	DetailPanel->SetTexture2D(BuildLeftTexture);
	DetailPanel->SetTitleFromString(BuildLeftTitle);
	DetailPanel->SetDescriptionFromString(BuildLeftDescription);
}

void UBarricadeBuildWidget::HoveredBuildRightButton()
{
	DetailPanel->SetVisibility(ESlateVisibility::Visible);

	DetailPanel->SetTexture2D(BuildRightTexture);
	DetailPanel->SetTitleFromString(BuildRightTitle);
	DetailPanel->SetDescriptionFromString(BuildRightDescription);
}

void UBarricadeBuildWidget::UnhoveredButton()
{
	DetailPanel->SetVisibility(ESlateVisibility::Collapsed);
}
