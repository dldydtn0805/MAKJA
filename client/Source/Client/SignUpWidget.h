// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "SignUpWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API USignUpWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	class AHttpActor* HttpActor;
	class UClientGameInstance* ClientGameInstance;
	void NickNameCheckResponse(int32 StatusCode, FString Message, bool Result);
	void SignUpResponse(int32 StatusCode, FString Message, int32 Id, FString Nickname);
	void SignInResponse(int32 StatusCode, FString Message, FString AccessToken, FString RefreshToken, FString Nickname, int32 Id);

protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* NickNameCheckBtn;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* SignUpBtn;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* GameExitBtn;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UEditableTextBox* NickName;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UEditableTextBox* Pwd;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UEditableTextBox* PwdCheck;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* SignUpWarningText;


private:
	bool IsNickNameChecked = false;

	// NickNameCheckBtnCallback -> 비어있는지 확인 -> 비어있지 않을 경우 API 요청
	// 요청에 대한 결과 방송되면 NickNameCheck 함수 호출됨
	UFUNCTION(BlueprintCallable)
	void NickNameCheckBtnCallback();
	UFUNCTION(BlueprintCallable)
	void SignUpBtnCallback();
	UFUNCTION(BlueprintCallable)
	void GameExitBtnCallback();

	UFUNCTION(BlueprintCallable)
	void OnNickNameTextChanged(const FText& Text);

	UFUNCTION(BlueprintCallable)
	void OnPwdTextChanged(const FText& Text);

	UFUNCTION(BlueprintCallable)
	void OnPwdCheckTextChanged(const FText& Text);
};
