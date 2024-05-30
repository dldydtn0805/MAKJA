// Fill out your copyright notice in the Description page of Project Settings.

#include "StartLevelWidget.h"
#include "GameExitConfirmWidget.h"
#include "StartGameMode.h"
#include "./Network/HttpActor.h"
#include "ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UStartLevelWidget::NativeConstruct() {
	Super::NativeConstruct();
	ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	HttpActor = GetWorld()->SpawnActor<AHttpActor>(AHttpActor::StaticClass());
	if (HttpActor)
	{
		// int32 StatusCode, FString Message, int32 GameId
		HttpActor->OnCompleteCreateNewGame.AddUObject(this, &UStartLevelWidget::CreateNewGameResponse);
		HttpActor->OnCompleteSignOut.AddUObject(this, &UStartLevelWidget::SignOutReseponse);
	}
	StartButton = Cast<UButton>(GetWidgetFromName(TEXT("S_Button")));
	LogOutButton = Cast<UButton>(GetWidgetFromName(TEXT("E_Button")));
	GameExitBtn = Cast<UButton>(GetWidgetFromName(TEXT("GameExitBtn")));
	//MyPageButton = Cast<UButton>(GetWidgetFromName(TEXT("")));

	StartButton->OnClicked.AddDynamic(this, &UStartLevelWidget::StartButtonCallback);
	LogOutButton->OnClicked.AddDynamic(this, &UStartLevelWidget::LogOutButtonCallback);
	GameExitBtn->OnClicked.AddDynamic(this, &UStartLevelWidget::GameExitBtnCallback);
}

void UStartLevelWidget::StartButtonCallback()
{
	// 클릭 버튼음 
	USoundBase* ClickSound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/Sounds/System/ClickButton.ClickButton'"));
	if (ClickSound)
	{
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}

	HttpActor->CreateNewGame(TEXT("Easy"));
}


void UStartLevelWidget::CreateNewGameResponse(int32 StatusCode, FString Message, int32 GameId)
{
	if (StatusCode/100 == 2)
	{
		ClientGameInstance->SetGameId(GameId);
		UE_LOG(LogTemp, Log, TEXT("%d"), ClientGameInstance->GetGameId());
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Map"));
	}
}

void UStartLevelWidget::LogOutButtonCallback()
{
	// 클릭 버튼음 
	USoundBase* ClickSound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/Sounds/System/ClickButton.ClickButton'"));
	if (ClickSound)
	{
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}

	HttpActor->SignOut();
}

void UStartLevelWidget::SignOutReseponse(int32 StatusCode, FString Message, FString Result)
{
	if (StatusCode == 200)
	{
		ClientGameInstance->SetAccessToken(TEXT(""));
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Login"));
	}
	else 
	{
		UE_LOG(LogTemp, Log, TEXT("Logout Failed : %d Error"), StatusCode);
	}
}

void UStartLevelWidget::GameExitBtnCallback()
{
	USoundBase* ClickSound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/Sounds/System/ClickButton.ClickButton'"));
	if (ClickSound)
	{
		UGameplayStatics::PlaySound2D(this, ClickSound);
	}
	AStartGameMode* GameMode = GetWorld()->GetAuthGameMode<AStartGameMode>();
	if (GameMode && GameMode->GameExitConfirmWidget)
	{
		// GameMode->GameExitConfirmWidget->AddToViewport();
		GameMode->GameExitConfirmWidget->SetVisibility(ESlateVisibility::Visible);
	}


}
