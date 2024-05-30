// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameExitConfirmWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UGameExitConfirmWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* CancelBtn;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* ExitBtn;
public:
	UFUNCTION(BlueprintCallable)
	void ExitBtnCallback();

	UFUNCTION(BlueprintCallable)
	void CancelCallback();


};
