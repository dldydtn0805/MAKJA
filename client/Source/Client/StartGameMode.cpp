// Fill out your copyright notice in the Description page of Project Settings.

#include "StartGameMode.h"
#include "StartLevelWidget.h"
#include "GameExitConfirmWidget.h"
#include "Kismet/GameplayStatics.h"


AStartGameMode::AStartGameMode() {
	static ConstructorHelpers::FClassFinder<UStartLevelWidget> StartWidgetClass(TEXT("WidgetBlueprint'/Game/UI/WBP_Start.WBP_Start_C'"));
	if (StartWidgetClass.Succeeded())
		StartLevelWidgetClass = StartWidgetClass.Class;
	static ConstructorHelpers::FClassFinder<UGameExitConfirmWidget> GECW(TEXT("WidgetBlueprint'/Game/UI/WBP_GameExitConfirm.WBP_GameExitConfirm_C'"));
	if (GECW.Succeeded())
	{
		GameExitWidgetClass = GECW.Class;
	}
	static ConstructorHelpers::FObjectFinder<USoundWave> BackgroundSound(TEXT("SoundWave'/Game/Sounds/Background/Background1.Background1'"));
	if (BackgroundSound.Succeeded())
	{
		UE_LOG(LogTemp, Log, TEXT("1"));
		BackgroundSoundWave = BackgroundSound.Object;
		BackgroundSoundWave->bLooping = true;
	}
}

void AStartGameMode::BeginPlay() {
	Super::BeginPlay();
	if (IsValid(StartLevelWidgetClass)) {
		StartLevelWidget = Cast<UStartLevelWidget>(CreateWidget(GetWorld(), StartLevelWidgetClass));
		if (IsValid(StartLevelWidget)) {
			UE_LOG(LogTemp, Warning, TEXT("start widget display"));
			StartLevelWidget->AddToViewport();
		}
	}
	if (IsValid(GameExitWidgetClass))
	{
		GameExitConfirmWidget = Cast<UGameExitConfirmWidget>(CreateWidget(GetWorld(), GameExitWidgetClass));
		if (IsValid(GameExitConfirmWidget)) {
			GameExitConfirmWidget->AddToViewport();
			GameExitConfirmWidget->SetVisibility(ESlateVisibility::Hidden);
			UE_LOG(LogTemp, Warning, TEXT("GameExitConfirmWidget display"));
		}
	}
	if (BackgroundSoundWave)
	{
		BackgroundMusicComponent = UGameplayStatics::SpawnSound2D(this, BackgroundSoundWave);
		if (BackgroundMusicComponent)
		{
			UE_LOG(LogTemp, Log, TEXT("2"));
			BackgroundMusicComponent->SetVolumeMultiplier(0.5f); // 볼륨 조정
			BackgroundMusicComponent->Play(); // 반복 재생 설정
		}
	}
}