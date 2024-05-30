// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UEnemyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindStat(class UEnemyStatComponent* StatComp);

	void UpdateHp();

private:
	TWeakObjectPtr<class UEnemyStatComponent> CurrentStatComp;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HPBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EnemyInfo;
};
