// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "GameExitConfirmWidget.h"
#include "SignUpWidget.h"
#include "LoginGameMode.h"
#include "./Network/HttpActor.h"
#include "Kismet/GameplayStatics.h"
#include "ClientGameInstance.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HttpActor = GetWorld()->SpawnActor<AHttpActor>(AHttpActor::StaticClass());
	if (HttpActor)
	{
		// int32 StatusCode, FString Message, FString AccessToken, FString RefreshToken, FString Nickname, int32 Id
		HttpActor->OnCompleteSignIn.AddUObject(this, &ULoginWidget::LoginResponse);
	}
	if (IdText && PwdText)
	{
		IdText->OnTextChanged.AddDynamic(this, &ULoginWidget::OnIdTextChanged);
		PwdText->OnTextChanged.AddDynamic(this, &ULoginWidget::OnPwdTextChanged);
		PwdText->SetIsPassword(true);
	}
	if (LoginBtn && SignUpBtn && GameExitBtn)
	{ 
		LoginBtn->OnClicked.AddDynamic(this, &ULoginWidget::LoginBtnCallback);
		SignUpBtn->OnClicked.AddDynamic(this, &ULoginWidget::SignUpBtnCallback);
		GameExitBtn->OnClicked.AddDynamic(this, &ULoginWidget::GameExitBtnCallback);
	}
	ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void ULoginWidget::LoginBtnCallback()
{

	// 클릭 버튼음 
	USoundBase* ClickSound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/Sounds/System/ClickButton.ClickButton'"));
	if (ClickSound)
	{
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}


	FString IdValue = IdText->GetText().ToString();
	FString PwdValue = PwdText->GetText().ToString();

	if (IdValue.IsEmpty())
	{
		FString WarningMessage = TEXT("Press Your Id.");
		LoginWarningText->SetText(FText::FromString(WarningMessage));
		LoginWarningText->SetVisibility(ESlateVisibility::Visible);
	}
	else if (PwdValue.IsEmpty())
	{
		FString WarningMessage = TEXT("Press Your Pwd.");
		LoginWarningText->SetText(FText::FromString(WarningMessage));
		LoginWarningText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ID : %s"), *IdValue);
		UE_LOG(LogTemp, Log, TEXT("PWD : %s"), *PwdValue);
		HttpActor->SignIn(IdValue, PwdValue);
		// UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
	}
}
void ULoginWidget::LoginResponse(int32 StatusCode, FString Message, FString AccessToken, FString RefreshToken, FString Nickname, int32 Id)
{
	UE_LOG(LogTemp, Log, TEXT("AccessToken : %s"), *AccessToken);
	if (StatusCode == 200)
	{
		ClientGameInstance->SetAccessToken(AccessToken);
		UE_LOG(LogTemp, Log, TEXT("%s"), *ClientGameInstance->GetAccessToken());
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
	}
	else if (StatusCode == 401)
	{
		FString WarningMessage = TEXT("Check Your Id and Pwd Again.");
		LoginWarningText->SetText(FText::FromString(WarningMessage));
		LoginWarningText->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		FString WarningMessage = TEXT("Server Error Occurs.");
		LoginWarningText->SetText(FText::FromString(WarningMessage));
		LoginWarningText->SetVisibility(ESlateVisibility::Visible);
	}
}

void ULoginWidget::SignUpBtnCallback()
{
	// 클릭 버튼음 
	USoundBase* ClickSound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/Sounds/System/ClickButton.ClickButton'"));
	if (ClickSound)
	{
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}


	if (GetWorld())
	{
		SetVisibility(ESlateVisibility::Hidden);
		// RemoveFromParent();

		ALoginGameMode* GameMode = GetWorld()->GetAuthGameMode<ALoginGameMode>();
		if (GameMode && GameMode->SignUpWidget) // 이미 생성된 SignUpWidget이 유효한지 확인
		{
			GameMode->SignUpWidget->AddToViewport(); // 이미 생성된 SignUpWidget을 화면에 붙임
		}

		else
		{

		}
	}
}

void ULoginWidget::GameExitBtnCallback()
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

void ULoginWidget::OnIdTextChanged(const FText& Text)
{
	IdText->SetText(Text);
}

void ULoginWidget::OnPwdTextChanged(const FText& Text)
{
	PwdText->SetText(Text);
}
