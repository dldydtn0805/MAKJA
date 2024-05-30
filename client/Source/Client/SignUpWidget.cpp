// Fill out your copyright notice in the Description page of Project Settings.


#include "SignUpWidget.h"
#include "LoginWidget.h"
#include "GameExitConfirmWidget.h"
#include "LoginGameMode.h"
#include "./Network/HttpActor.h"
#include "Kismet/GameplayStatics.h"
#include "ClientGameInstance.h"

void USignUpWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HttpActor = GetWorld()->SpawnActor<AHttpActor>(AHttpActor::StaticClass());
	if (HttpActor)
	{
		// int32 StatusCode, FString Message, bool Result
		HttpActor->OnCompleteNicknameCheck.AddUObject(this, &USignUpWidget::NickNameCheckResponse);
		// int32 StatusCode, FString Message, int32 Id, FString Nickname
		HttpActor->OnCompleteSignUp.AddUObject(this, &USignUpWidget::SignUpResponse);
		// int32 StatusCode, FString Message, FString AccessToken, FString RefreshToken, FString Nickname, int32 Id
		HttpActor->OnCompleteSignIn.AddUObject(this, &USignUpWidget::SignInResponse);
	}

	if (NickName && Pwd && PwdCheck)
	{
		NickName->OnTextChanged.AddDynamic(this, &USignUpWidget::OnNickNameTextChanged);
		Pwd->OnTextChanged.AddDynamic(this, &USignUpWidget::OnPwdTextChanged);
		Pwd->SetIsPassword(true);
		PwdCheck->OnTextChanged.AddDynamic(this, &USignUpWidget::OnPwdCheckTextChanged);
		PwdCheck->SetIsPassword(true);
	}
	if (NickNameCheckBtn && SignUpBtn && GameExitBtn)
	{
		NickNameCheckBtn->OnClicked.AddDynamic(this, &USignUpWidget::NickNameCheckBtnCallback);
		SignUpBtn->OnClicked.AddDynamic(this, &USignUpWidget::SignUpBtnCallback);
		GameExitBtn->OnClicked.AddDynamic(this, &USignUpWidget::GameExitBtnCallback);
	}
	ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void USignUpWidget::NickNameCheckBtnCallback()
{
	// 클릭 버튼음 
	USoundBase* ClickSound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/Sounds/System/ClickButton.ClickButton'"));
	if (ClickSound)
	{
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}

	FString NickNameValue = NickName->GetText().ToString();
	if (NickNameValue.IsEmpty())
	{
		FString WarningMessage = TEXT("Press Your NickName.");
		SignUpWarningText->SetText(FText::FromString(WarningMessage));
		SignUpWarningText->SetVisibility(ESlateVisibility::Visible);
		IsNickNameChecked = false;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("NickNameValue: %s"), *NickNameValue);
		HttpActor->NicknameCheck(NickNameValue);
	}
}

void USignUpWidget::NickNameCheckResponse(int32 StatusCode, FString Message, bool Result)
{
	UE_LOG(LogTemp, Log, TEXT("%d"), StatusCode);
	// 정상적인 Response
	if (StatusCode == 200)
	{
		// 중복된 닉네임 존재
		if (Result == true)
		{
			FString WarningMessage = TEXT("This NickName Already Exists.");
			SignUpWarningText->SetText(FText::FromString(WarningMessage));
			SignUpWarningText->SetVisibility(ESlateVisibility::Visible);
			IsNickNameChecked = false;
		}
		// 중복된 닉네임 존재 X
		else
		{
			FString WarningMessage = TEXT("You Can Use This NickName.");
			SignUpWarningText->SetText(FText::FromString(WarningMessage));
			SignUpWarningText->SetVisibility(ESlateVisibility::Visible);
			IsNickNameChecked = true;
		}
	}
	// 비정상적인 Response
	else if (StatusCode == 500)
	{
		FString WarningMessage = TEXT("NickName Can't Contain Korean.");
		SignUpWarningText->SetText(FText::FromString(WarningMessage));
		SignUpWarningText->SetVisibility(ESlateVisibility::Visible);
		IsNickNameChecked = false;
	}
}

void USignUpWidget::SignUpBtnCallback()
{

	// 클릭 버튼음 
	USoundBase* ClickSound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/Sounds/System/ClickButton.ClickButton'"));
	if (ClickSound)
	{
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}
	// 닉네임 중복 체크 유무 확인
	// 닉네임 중복 체크가 확인되지 않은 경우
	if (IsNickNameChecked == false)
	{
		FString WarningMessage = TEXT("Check Your NickName.");
		SignUpWarningText->SetText(FText::FromString(WarningMessage));
		SignUpWarningText->SetVisibility(ESlateVisibility::Visible);
	}
	// 닉네임 중복 체크가 확인 된 경우
	else
	{
		if (IsNickNameChecked == true)
		{
			FString NickNameValue = NickName->GetText().ToString();
			FString PwdValue = Pwd->GetText().ToString();
			FString PwdCheckValue = PwdCheck->GetText().ToString();
			if (PwdValue.IsEmpty())
			{
				FString WarningMessage = TEXT("Press Your Password.");
				SignUpWarningText->SetText(FText::FromString(WarningMessage));
				SignUpWarningText->SetVisibility(ESlateVisibility::Visible);
			}
			else if (PwdCheckValue.IsEmpty())
			{
				FString WarningMessage = TEXT("Press Your Password Check.");
				SignUpWarningText->SetText(FText::FromString(WarningMessage));
				SignUpWarningText->SetVisibility(ESlateVisibility::Visible);
			}
			else if (PwdValue != PwdCheckValue)
			{
				FString WarningMessage = TEXT("Your Password is Not Same with Password Check.");
				SignUpWarningText->SetText(FText::FromString(WarningMessage));
				SignUpWarningText->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				HttpActor->SignUp(NickNameValue, PwdValue);
			}
		}
	}
}

void USignUpWidget::SignUpResponse(int32 StatusCode, FString Message, int32 Id, FString Nickname)
{
	// 회원가입 성공 시 자동 로그인
	if (StatusCode == 201)
	{
		FString NickNameValue = NickName->GetText().ToString();
		FString PwdValue = Pwd->GetText().ToString();
		UE_LOG(LogTemp, Log, TEXT("%s : %s"), *NickNameValue, *PwdValue);
		HttpActor->SignIn(NickNameValue, PwdValue);
	}
	else if (StatusCode == 401)
	{
		FString WarningMessage = TEXT("Check Your SignUp Info Again.");
		SignUpWarningText->SetText(FText::FromString(WarningMessage));
		SignUpWarningText->SetVisibility(ESlateVisibility::Visible);
		IsNickNameChecked = false;
	}
	else
	{
		FString WarningMessage = TEXT("Server Error Occurs.");
		SignUpWarningText->SetText(FText::FromString(WarningMessage));
		SignUpWarningText->SetVisibility(ESlateVisibility::Visible);
		IsNickNameChecked = false;
	}
}

void USignUpWidget::SignInResponse(int32 StatusCode, FString Message, FString AccessToken, FString RefreshToken, FString Nickname, int32 Id)
{
	if (StatusCode == 200)
	{
		ClientGameInstance->SetAccessToken(AccessToken);
		UE_LOG(LogTemp, Log, TEXT("%s"), *ClientGameInstance->GetAccessToken());
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
	}

}

void USignUpWidget::GameExitBtnCallback()
{
	USoundBase* ClickSound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/Sounds/System/ClickButton.ClickButton'"));
	if (ClickSound)
	{
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}
	ALoginGameMode* GameMode = GetWorld()->GetAuthGameMode<ALoginGameMode>();
	if (GameMode && GameMode->GameExitConfirmWidget)
	{
		// GameMode->GameExitConfirmWidget->AddToViewport();
		GameMode->GameExitConfirmWidget->SetVisibility(ESlateVisibility::Visible);
	}

}


void USignUpWidget::OnNickNameTextChanged(const FText& Text)
{
	NickName->SetText(Text);
}

void USignUpWidget::OnPwdTextChanged(const FText& Text)
{
	Pwd->SetText(Text);
}

void USignUpWidget::OnPwdCheckTextChanged(const FText& Text)
{
	PwdCheck->SetText(Text);
}
