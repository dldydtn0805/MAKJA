// Fill out your copyright notice in the Description page of Project Settings.


#include "NexusHpBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UNexusHpBar::SetPercentage(float Percent)
{
	PB_HPBar->SetPercent(Percent / 100.f);
	HpText->SetText(FText::FromString(FString::Printf(TEXT("%.2f%%"), Percent)));
}
