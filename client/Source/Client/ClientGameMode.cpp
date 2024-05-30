// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientGameMode.h"
#include "ClientGameState.h"
#include "ClientGameInstance.h"
#include "./Player/Magician/ScreenWidget.h"
#include "./Player/Magician/GameEndWidget.h"
#include "./Player/Magician/MagicianCharacter.h"
#include "Kismet/GameplayStatics.h"

// #include "TimerManager.h"

AClientGameMode::AClientGameMode() {
	CurrentWave = 1;
	TotalWave = 20;
	TotalEnemy = 0;

	DefaultPawnClass = AMagicianCharacter::StaticClass();

	static ConstructorHelpers::FClassFinder<ACharacter> BP_Char(TEXT("Blueprint'/Game/BluePrint/Player/Magician/BP_MagicianCharacter.BP_MagicianCharacter_C'")); // _C를 붙인다

	if (BP_Char.Succeeded())
	{
		DefaultPawnClass = BP_Char.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("Blueprint'/Game/BluePrint/Player/Controller/BP_ClientPlayerController.BP_ClientPlayerController_C'"));

	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	int32 RandomNumber = FMath::RandRange(2, 4); // 2부터 4까지(포함)의 랜덤한 정수 생성
	// 랜덤한 숫자를 문자열로 변환하여 사운드 웨이브 경로 만들기
	FString RandomSoundPath = FString::Printf(TEXT("SoundWave'/Game/Sounds/Background/Background%d.Background%d'"), RandomNumber, RandomNumber);

	// 사운드 웨이브 로드
	static ConstructorHelpers::FObjectFinder<USoundWave> RandomBackgroundSound(*RandomSoundPath);
	if (RandomBackgroundSound.Succeeded())
	{
		// 랜덤하게 선택된 사운드 웨이브 가져오기
		BackgroundSoundWave = RandomBackgroundSound.Object;
		BackgroundSoundWave->bLooping = true;
	}
	ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}


void AClientGameMode::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController)
	{
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
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
	StartWave();
}

void AClientGameMode::StartWave()
{
	// Enemy Spawn
	// game instance  - Current Wave 정보 가져오기
	auto CGI = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (!CGI) return; 

	WD = CGI->GetWaveData(CurrentWave);
	if (!WD) return;
	
	TotalEnemy = 0;
	for (const FSpawnData& Data : WD->SpawnData) {
		TSubclassOf<AEnemy> SpawnEnemy = (CGI->GetEnemyStatData(Data.EnemyId))->BP_Enemy;
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [SpawnPoint = SpawnPoints[Data.SpawnPoint], SpawnEnemy, 
			EnemyID = Data.EnemyId, Count = Data.Count, Data]()
		{
			SpawnPoint->Spawn(SpawnEnemy, EnemyID, Count);
		}, Data.Time, false);
		TotalEnemy += Data.Count;
	}
	CurrentEnemy = TotalEnemy;
	AMagicianCharacter* Player = Cast<AMagicianCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player) {
		UScreenWidget* SW = Cast<UScreenWidget>(Player->ScreenWidget);
		if (SW) {
			SW->BindMonsterKillCount(CurrentEnemy, TotalEnemy);
			SW->BindCurrentStage(CurrentWave);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Total Enemy: %i"), TotalEnemy);
	UE_LOG(LogTemp, Warning, TEXT("Total Enemy: %i"), TotalEnemy);
}


void AClientGameMode::EndWave()
{
	// total Enemy 0 되면 호출
	// 디버깅용
	UE_LOG(LogTemp, Warning, TEXT("%i wave clear weqeqwe"), CurrentWave);
	UE_LOG(LogTemp, Warning, TEXT("%i wave clear weqeqwe"), CurrentWave);

	// 5.0초 후 새 웨이브 시작
	FTimerHandle TimerHandle;
	if (CurrentWave < 25) {
		++CurrentWave;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AClientGameMode::StartWave, 5.0f, false);
		if (ClientGameInstance)
		{
			ClientGameInstance->SetCurrentStage();
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("%i Finishfdasfsdafsda"), CurrentWave);


		GameClear();
	}
}

void AClientGameMode::GameOver()
{
	// 게임 오버
}

void AClientGameMode::GameClear()
{
	// 게임 클리어
	UE_LOG(LogTemp, Warning, TEXT("Game Clear"));
	AMagicianCharacter* Player = Cast<AMagicianCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		if (Player->GameEndWidget)
		{
			Player->GameEndWidget->PostRankingList();
			Player->GameEndWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AClientGameMode::DecreaseEnemy()
{
	if (CurrentEnemy > 0) {
		CurrentEnemy--;
		AMagicianCharacter* Player = Cast<AMagicianCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (Player) {
			UScreenWidget* SW = Cast<UScreenWidget>(Player->ScreenWidget);
			if (SW) SW->BindMonsterKillCount(CurrentEnemy, TotalEnemy);
		}
		if (CurrentEnemy == 0) EndWave();
		UE_LOG(LogTemp, Warning, TEXT("%i end Wave fdasfsdafsda"), CurrentWave);
	}
	
}