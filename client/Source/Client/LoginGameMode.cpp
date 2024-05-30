// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginGameMode.h"
#include "LoginWidget.h"
#include "SignUpWidget.h"
#include "GameExitConfirmWidget.h"
#include "Kismet/GameplayStatics.h"

ALoginGameMode::ALoginGameMode() {
	static ConstructorHelpers::FClassFinder<ULoginWidget> LWC(TEXT("WidgetBlueprint'/Game/UI/WBP_Login.WBP_Login_C'"));
	if (LWC.Succeeded())
	{
		LoginWidgetClass = LWC.Class;
	}
	static ConstructorHelpers::FClassFinder<USignUpWidget> SUWC(TEXT("WidgetBlueprint'/Game/UI/WBP_SignUp.WBP_SignUp_C'"));
	if (SUWC.Succeeded())
	{
		SignUpWidgetClass = SUWC.Class;
	}
	static ConstructorHelpers::FClassFinder<UGameExitConfirmWidget> GECW(TEXT("WidgetBlueprint'/Game/UI/WBP_GameExitConfirm.WBP_GameExitConfirm_C'"));
	if (GECW.Succeeded())
	{
		GameExitWidgetClass = GECW.Class;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> BackgroundSound(TEXT("SoundWave'/Game/Sounds/Background/Background1.Background1'"));
	if (BackgroundSound.Succeeded())
	{
		BackgroundSoundWave = BackgroundSound.Object;
		BackgroundSoundWave->bLooping = true;
	}
}

void ALoginGameMode::BeginPlay() {
	Super::BeginPlay();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
	if (IsValid(LoginWidgetClass) && IsValid(SignUpWidgetClass)) 
	{
		LoginWidget = Cast<ULoginWidget>(CreateWidget(GetWorld(), LoginWidgetClass));
		if (IsValid(LoginWidget)) {
			UE_LOG(LogTemp, Warning, TEXT("Login widget display"));
			LoginWidget->AddToViewport();
		}
		SignUpWidget = Cast<USignUpWidget>(CreateWidget(GetWorld(), SignUpWidgetClass));
		if (IsValid(SignUpWidget)) {
			UE_LOG(LogTemp, Warning, TEXT("SignUp widget display"));
			// SignUpWidget->AddToViewport();
		}
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
			BackgroundMusicComponent->SetVolumeMultiplier(0.5f); // 볼륨 조정
			BackgroundMusicComponent->Play(); // 반복 재생 설정
		}
	}
}