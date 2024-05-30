// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartLevelWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UStartLevelWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	class UClientGameInstance* ClientGameInstance;
	class AHttpActor* HttpActor;
	void CreateNewGameResponse(int32 StatusCode, FString Message, int32 GameId);
	void SignOutReseponse(int32 StatusCode, FString Message, FString Result);
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* StartButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* LogOutButton;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GameExitBtn;
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* MyPageButton;
	*/

private:
	UFUNCTION(BlueprintCallable)
	void StartButtonCallback();
	
	UFUNCTION(BlueprintCallable)
	void LogOutButtonCallback();

	UFUNCTION(BlueprintCallable)
	void GameExitBtnCallback();


};
