// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	class AHttpActor* HttpActor;
	void LoginResponse(int32 StatusCode, FString Message, FString AccessToken, FString RefreshToken, FString Nickname, int32 Id);
	class UClientGameInstance* ClientGameInstance;


protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* LoginBtn;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SignUpBtn;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GameExitBtn;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UEditableTextBox* IdText;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UEditableTextBox* PwdText;
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UButton* MyPageButton;
	*/
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* LoginWarningText;

private:
	UFUNCTION(BlueprintCallable)
	void LoginBtnCallback();

	UFUNCTION(BlueprintCallable)
	void SignUpBtnCallback();
	
	UFUNCTION(BlueprintCallable)
	void GameExitBtnCallback();


	UFUNCTION(BlueprintCallable)
	void OnIdTextChanged(const FText& Text);

	UFUNCTION(BlueprintCallable)
	void OnPwdTextChanged(const FText& Text);
};