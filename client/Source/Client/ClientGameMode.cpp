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

	static ConstructorHelpers::FClassFinder<ACharacter> BP_Char(TEXT("Blueprint'/Game/BluePrint/Player/Magician/BP_MagicianCharacter.BP_MagicianCharacter_C'")); // _C�� ���δ�

	if (BP_Char.Succeeded())
	{
		DefaultPawnClass = BP_Char.Class;
	}

	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("Blueprint'/Game/BluePrint/Player/Controller/BP_ClientPlayerController.BP_ClientPlayerController_C'"));

	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	int32 RandomNumber = FMath::RandRange(2, 4); // 2���� 4����(����)�� ������ ���� ����
	// ������ ���ڸ� ���ڿ��� ��ȯ�Ͽ� ���� ���̺� ��� �����
	FString RandomSoundPath = FString::Printf(TEXT("SoundWave'/Game/Sounds/Background/Background%d.Background%d'"), RandomNumber, RandomNumber);

	// ���� ���̺� �ε�
	static ConstructorHelpers::FObjectFinder<USoundWave> RandomBackgroundSound(*RandomSoundPath);
	if (RandomBackgroundSound.Succeeded())
	{
		// �����ϰ� ���õ� ���� ���̺� ��������
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
			BackgroundMusicComponent->SetVolumeMultiplier(0.5f); // ���� ����
			BackgroundMusicComponent->Play(); // �ݺ� ��� ����
		}
	}
	StartWave();
}

void AClientGameMode::StartWave()
{
	// Enemy Spawn
	// game instance  - Current Wave ���� ��������
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
	// total Enemy 0 �Ǹ� ȣ��
	// ������
	UE_LOG(LogTemp, Warning, TEXT("%i wave clear weqeqwe"), CurrentWave);
	UE_LOG(LogTemp, Warning, TEXT("%i wave clear weqeqwe"), CurrentWave);

	// 5.0�� �� �� ���̺� ����
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
	// ���� ����
}

void AClientGameMode::GameClear()
{
	// ���� Ŭ����
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