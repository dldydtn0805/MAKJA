// Fill out your copyright notice in the Description page of Project Settings.

#include "ClientPlayerController.h"
#include "../Magician/MagicianCharacter.h"
#include "../../ClientGameInstance.h"
#include "../../Tower/Tower.h"
#include "../../Tower/TowerButtonWidget.h"
#include "../../Tower/TowerSelectWidget.h"
#include "../../Tower/TowerLevelUpOrSellWidget.h"
#include "../../Tower/Barricade/BarricadeBuildWidget.h"
#include "../../Tower/MessageWidget.h"
#include "../../Tower/TowerDetector.h"
#include "../../Tower/SelectedTowerChecker.h"
#include "../../Tower/Barricade/Barricade.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "../../Network/HttpActor.h"

AClientPlayerController::AClientPlayerController()
{
	// 위젯 컴포넌트 생성
	TowerSelectWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("TowerSelectWidget"));
	TowerLevelUpOrSellWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("TowerLevelUpOrSellWidgetComp"));
	BarricadeBuildWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("BarricadeBuildWidgetComp"));
	MessageWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("MessageWidgetComp"));

	// 각 컴포넌트에 위젯 할당
	static ConstructorHelpers::FClassFinder<UTowerSelectWidget> TSW(TEXT("WidgetBlueprint'/Game/UI/Tower/WBP_TowerSelectWidget.WBP_TowerSelectWidget_C'"));
	if (TSW.Succeeded()) {
		TowerSelectWidgetComp->SetWidgetClass(TSW.Class);
	}

	static ConstructorHelpers::FClassFinder<UTowerLevelUpOrSellWidget> TLSW(TEXT("WidgetBlueprint'/Game/UI/Tower/WBP_TowerLevelUpOrSellWidget.WBP_TowerLevelUpOrSellWidget_C'"));
	if (TLSW.Succeeded()) {
		TowerLevelUpOrSellWidgetComp->SetWidgetClass(TLSW.Class);
	}

	static ConstructorHelpers::FClassFinder<UBarricadeBuildWidget> BBW(TEXT("WidgetBlueprint'/Game/UI/Tower/Barricade/WBP_BarricadeBuildWidget.WBP_BarricadeBuildWidget_C'"));
	if (BBW.Succeeded()) {
		BarricadeBuildWidgetComp->SetWidgetClass(BBW.Class);
	}

	static ConstructorHelpers::FClassFinder<UMessageWidget> MW(TEXT("WidgetBlueprint'/Game/UI/Tower/WBP_MessageWidget.WBP_MessageWidget_C'"));
	if (MW.Succeeded()) {
		MessageWidgetComp->SetWidgetClass(MW.Class);
	}
}

// InputComponent 설정
void AClientPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ClickLeft", IE_Pressed, this, &AClientPlayerController::ClickLeft);
	InputComponent->BindAction("ReturnInit", IE_Pressed, this, &AClientPlayerController::ReturnInit);
}

void AClientPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UTowerSelectWidget* TowerSelectWidget = Cast<UTowerSelectWidget>(TowerSelectWidgetComp->GetWidget());
	UTowerLevelUpOrSellWidget* TowerLevelUpOrSellWidget = Cast<UTowerLevelUpOrSellWidget>(TowerLevelUpOrSellWidgetComp->GetWidget());
	UBarricadeBuildWidget* BarricadeBuildWidget = Cast<UBarricadeBuildWidget>(BarricadeBuildWidgetComp->GetWidget());
	UMessageWidget* MessageWidget = Cast<UMessageWidget>(MessageWidgetComp->GetWidget());

	// 각 위젯을 월드에 띄운다.
	TowerSelectWidget->AddToViewport();
	TowerLevelUpOrSellWidget->AddToViewport();
	BarricadeBuildWidget->AddToViewport();
	MessageWidget->AddToViewport();

	// TowerSelectWidget에 있는 버튼에 OnCkicked에 대한 콜백 함수 할당
	TowerSelectWidget->GetButtonWidget(0)->GetButton()->OnPressed.AddDynamic(this, &AClientPlayerController::SelectBuildTower0);
	TowerSelectWidget->GetButtonWidget(1)->GetButton()->OnPressed.AddDynamic(this, &AClientPlayerController::SelectBuildTower1);
	TowerSelectWidget->GetButtonWidget(2)->GetButton()->OnPressed.AddDynamic(this, &AClientPlayerController::SelectBuildTower2);
	TowerSelectWidget->GetButtonWidget(3)->GetButton()->OnPressed.AddDynamic(this, &AClientPlayerController::SelectBuildTower3);
	TowerSelectWidget->GetButtonWidget(4)->GetButton()->OnPressed.AddDynamic(this, &AClientPlayerController::SelectBuildTower4);
	TowerSelectWidget->GetButtonWidget(5)->GetButton()->OnPressed.AddDynamic(this, &AClientPlayerController::SelectBuildTower5);

	// TowerLevelUpOrSellWidget에 있는 버튼에 OnCkicked에 대한 콜백 함수 할당
	TowerLevelUpOrSellWidget->GetLevelUpButtonWidget()->GetButton()->OnPressed.AddDynamic(this, &AClientPlayerController::TowerLevelUp);
	TowerLevelUpOrSellWidget->GetSellButtonWidget()->GetButton()->OnPressed.AddDynamic(this, &AClientPlayerController::SellTower);

	// BarricadeBuildWidget에 있는 버튼에 OnCkicked에 대한 콜백 함수 할당
	BarricadeBuildWidget->GetBuildUpButtonWidget()->GetButton()->OnPressed.AddDynamic(this, &AClientPlayerController::BuildBarricadeUp);
	BarricadeBuildWidget->GetBuildDownButtonWidget()->GetButton()->OnPressed.AddDynamic(this, &AClientPlayerController::BuildBarricadeDown);
	BarricadeBuildWidget->GetBuildLeftButtonWidget()->GetButton()->OnPressed.AddDynamic(this, &AClientPlayerController::BuildBarricadeLeft);
	BarricadeBuildWidget->GetBuildRightButtonWidget()->GetButton()->OnPressed.AddDynamic(this, &AClientPlayerController::BuildBarricadeRight);

	// BarricadeBuildWidget에 코스트 업데이트
	ABarricade* Barricade = NewObject<ABarricade>(this, BarricadeClass);
	BarricadeBuildWidget->SetBarricadeCost(Barricade->GetCost());

	// 초기 상태는 OFF로 설정
	ChangeMode(FTowerControlMode::OFF);
}

// 매 프레임마다 동작
void AClientPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// 현재 탑 뷰면 타워 빌드 및 선택 관련 UI(Actor)를 활성화
	if (IsActiveTowerControl()) {
		// 현재 마우스 커서의 위치를 획득
		float MouseLocationX, MouseLocationY;
		bool bInScreen = GetMousePosition(MouseLocationX, MouseLocationY);

		// 마우스 커서가 게임 화면 안에 있으면 해당 위치에 타워를 소환
		if (bInScreen) {
			// World Coordinate 기준 위치와 방향
			FVector WorldLocation, WorldDirection;
			bool bSucceed = DeprojectScreenPositionToWorld(
				MouseLocationX, MouseLocationY, WorldLocation, WorldDirection
			);

			// 좌표 변환에 성공하면 스폰 위치 계산 후 탑을 스폰
			if (bSucceed) {
				// 목표 위치 계산
				float Weight = (100.f - WorldLocation.Z) / WorldDirection.Z;
				FVector TargetLocation = WorldLocation + Weight * WorldDirection;

				if (SpawningDetector) { // 이미 활성화된 SpawningDetector가 있으면 위치만 옮겨준다.
					SpawningDetector->SetActorLocation(TargetLocation);
				}
				else { // 없으면 만든다.
					FTransform TargetTransform = FTransform(TargetLocation); // 목표 스폰 위치
					ATowerDetector* Detector; // 스폰할 TowerDetector

					// 빌드 모드면 ATowerSpawnDetector 생성
					if (TowerControlMode == FTowerControlMode::BUILD) {
						// TowerSpawnDetector 스폰 준비
						Detector = GetWorld()->SpawnActorDeferred<ATowerDetector>(TowerSpawnDetector, TargetTransform);
						SpawningDetector = Detector; // 스폰된 TowerDetector의 포인터를 SpawningDetector에 저장

						// 프리뷰 스태틱 메시 설정
						SetPreviewStaticMesh(SelectedIndex);
					}
					else { // 빌드 모드가 아니면 **생성
						// TowerSpawnDetector 스폰 준비
						Detector = GetWorld()->SpawnActorDeferred<ATowerDetector>(TowerLevelUpDetector, TargetTransform);
						SpawningDetector = Detector; // 스폰된 TowerDetector의 포인터를 SpawningDetector에 저장
					}

					Detector->FinishSpawning(TargetTransform); // 스폰
				}
			}
		}
	}
	else {
		// 다시 3인칭 뷰로 돌아올 때 스폰된 SpawningDetector를 제거해준다.
		if (SpawningDetector) {
			SpawningDetector->Destroy();
			SpawningDetector = nullptr;
		}

		if (SpawnedSelectedTowerChecker) {
			SpawnedSelectedTowerChecker->Destroy();
			SpawnedSelectedTowerChecker = nullptr;
		}
	}
}

// 모드 전환
void AClientPlayerController::ChangeMode(FTowerControlMode NewMode, int32 SelectedTowerIndex)
{
	if (SpawningDetector) {
		SpawningDetector->Destroy();
		SpawningDetector = nullptr;
	}

	UTowerSelectWidget* TowerSelectWidget = Cast<UTowerSelectWidget>(TowerSelectWidgetComp->GetWidget());
	UTowerLevelUpOrSellWidget* TowerLevelUpOrSellWidget = Cast<UTowerLevelUpOrSellWidget>(TowerLevelUpOrSellWidgetComp->GetWidget());
	UBarricadeBuildWidget* BarricadeBuildWidget = Cast<UBarricadeBuildWidget>(BarricadeBuildWidgetComp->GetWidget());

	// 현재 상태에 맞춰서 UI 변경
	switch (NewMode)
	{
	case FTowerControlMode::OFF:
		TowerSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
		TowerLevelUpOrSellWidget->SetVisibility(ESlateVisibility::Collapsed);
		BarricadeBuildWidget->SetVisibility(ESlateVisibility::Collapsed);
		SetInputMode(FInputModeGameOnly());
		break;
	case FTowerControlMode::INIT:
		TowerSelectWidget->SetVisibility(ESlateVisibility::Visible);
		TowerLevelUpOrSellWidget->SetVisibility(ESlateVisibility::Collapsed);
		BarricadeBuildWidget->SetVisibility(ESlateVisibility::Collapsed);
		SetInputMode(FInputModeGameAndUI());
		break;
	case FTowerControlMode::BUILD:
		TowerSelectWidget->SetVisibility(ESlateVisibility::Visible);
		TowerLevelUpOrSellWidget->SetVisibility(ESlateVisibility::Collapsed);
		BarricadeBuildWidget->SetVisibility(ESlateVisibility::Collapsed);
		SetInputMode(FInputModeGameAndUI());
		break;
	case FTowerControlMode::SELECTED:
		TowerSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
		TowerLevelUpOrSellWidget->SetVisibility(ESlateVisibility::Visible);
		BarricadeBuildWidget->SetVisibility(ESlateVisibility::Collapsed);
		SetInputMode(FInputModeGameAndUI());
		break;
	case FTowerControlMode::BARRICADE:
		TowerSelectWidget->SetVisibility(ESlateVisibility::Collapsed);
		TowerLevelUpOrSellWidget->SetVisibility(ESlateVisibility::Collapsed);
		BarricadeBuildWidget->SetVisibility(ESlateVisibility::Visible);
		SetInputMode(FInputModeGameAndUI());
		break;
	default:
		break;
	}

	if (NewMode == FTowerControlMode::BUILD) {
		TowerSelectWidget->GetReturnInitText()->SetVisibility(ESlateVisibility::Visible);
		TowerSelectWidget->GetTowerButtonsGridPanel()->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		TowerSelectWidget->GetReturnInitText()->SetVisibility(ESlateVisibility::Collapsed);
		TowerSelectWidget->GetTowerButtonsGridPanel()->SetVisibility(ESlateVisibility::Visible);
	}

	// TowerControlMode와 SelectedIndex 최신화
	TowerControlMode = NewMode;
	SelectedIndex = SelectedTowerIndex;
}

void AClientPlayerController::ReturnInit()
{
	if (TowerControlMode == FTowerControlMode::BUILD) {
		ChangeMode(FTowerControlMode::INIT);
	}
}


void AClientPlayerController::ClickLeft()
{
	if (IsActiveTowerControl()) {
		ClickAction();
	}
	else {
		APawn* CurrentPawn = GetPawn();
		AMagicianCharacter* MagicianCharacter = Cast<AMagicianCharacter>(CurrentPawn);
		if (MagicianCharacter) {
			MagicianCharacter->AttackLeft();
		}
	}
}

// 클릭시 모드에 따라 로직 수행
void AClientPlayerController::ClickAction()
{
	if (TowerControlMode == FTowerControlMode::BUILD) { // 빌드 모드면 타워를 짓는다.
		// 감지된 타워가 없으면 타워를 짓는다.
		if (!SpawningDetector->IsDetectedNexus() && !SpawningDetector->GetDetectedTowerNum()) {
			SpawnTower();
		}
		else {
			Cast<UMessageWidget>(MessageWidgetComp->GetWidget())->ShowMessage(TEXT("본진 또는 다른 타워들과 가까운 위치에 새로운 타워를 지을 수 없습니다."), 3.f);
		}
	}
	else { // 빌드 모드가 아니면
		if (SpawningDetector->IsDetectedNexus()) { // 넥서스가 감지된 상태라면
			ChangeMode(FTowerControlMode::BARRICADE);
		}
		else if (SpawningDetector->GetDetectedTowerNum()) { // 감지된 타워가 있으면 해당 타워를 선택
			// 감지된 타워를 받은 후 SelectedTower에 할당
			ATower* Tower = SpawningDetector->GetOverlappedTowerOnce();
			SelectedTower = Tower;

			// 선택된 타워를 알 수 있게 표시
			if (SpawnedSelectedTowerChecker) {
				SpawnedSelectedTowerChecker->SetActorLocation(Tower->GetActorLocation());
			}
			else {	
				SpawnedSelectedTowerChecker = GetWorld()->SpawnActor<ASelectedTowerChecker>(SelectedTowerChecker, Tower->GetActorLocation(), FRotator::ZeroRotator);
			}

			// 선택된 타워를 토대로 UI 갱신
			Cast<UTowerLevelUpOrSellWidget>(TowerLevelUpOrSellWidgetComp->GetWidget())->BindTower(Tower);
			ChangeMode(FTowerControlMode::SELECTED);
		}
		else { // 아니면 선택 모드를 해제한다.
			SelectedTower = nullptr;

			if (SpawnedSelectedTowerChecker) {
				SpawnedSelectedTowerChecker->Destroy();
				SpawnedSelectedTowerChecker = nullptr;
			}

			ChangeMode(FTowerControlMode::INIT);
		}
	}
}

// 타워 스폰
void AClientPlayerController::SpawnTower() {
	// 현재 마우스 커서의 위치를 획득
	float MouseLocationX, MouseLocationY;
	bool bInScreen = GetMousePosition(MouseLocationX, MouseLocationY);

	// 마우스 커서가 게임 화면 안에 있으면 해당 위치에 타워를 소환
	if (bInScreen) {
		// World Coordinate 기준 위치와 방향
		FVector WorldLocation, WorldDirection;
		bool bSucceed = DeprojectScreenPositionToWorld(
			MouseLocationX, MouseLocationY, WorldLocation, WorldDirection
		);

		// 좌표 변환에 성공하면 스폰 위치 계산 후 탑을 스폰
		if (bSucceed) {
			// 스폰 위치 계산
			float Weight = (100.f - WorldLocation.Z) / WorldDirection.Z;
			FVector SpawnLoc = WorldLocation + Weight * WorldDirection;

			// 계산된 스폰 위치에 탑 생성 아직 스폰은 보류
			ATower* Tower = GetWorld()->SpawnActorDeferred<ATower>(TowerClasses[SelectedIndex], FTransform(SpawnLoc), this);
			
			// 돈 감소 시도
			auto ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			int32 Cost = ClientGameInstance->GetTowerStatData(Tower->GetTowerId(), 1)->Cost;
			AMagicianCharacter* MagicianCharacter = Cast<AMagicianCharacter>(GetPawn());
			bool bCostDec = MagicianCharacter->ChangeMoney(-Cost);

			// 돈 감소 결과에 따라 로직 수행
			if (bCostDec) {
				Tower->FinishSpawning(FTransform(SpawnLoc));
			}
			else {
				Tower->Destroy();
				Cast<UMessageWidget>(MessageWidgetComp->GetWidget())->ShowMessage(TEXT("돈이 부족합니다."), 3.f);
			}
		}
	}

	// 스폰이 종료되면 모드 전환으로 빌드 모드 해제
	ChangeMode(FTowerControlMode::INIT);
}

// 타워 강화 기능 테스트 용
void AClientPlayerController::TowerLevelUp()
{
	UE_LOG(LogTemp, Log, TEXT("Tower Level Up"));

	// 돈 감소
	auto ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	int32 Cost = ClientGameInstance->GetTowerStatData(SelectedTower->GetTowerId(), SelectedTower->GetTowerStatComponent()->GetLevel() + 1)->Cost;
	AMagicianCharacter* MagicianCharacter = Cast<AMagicianCharacter>(GetPawn());
	bool bResult = MagicianCharacter->ChangeMoney(-Cost);

	// 돈 감소 결과에 따라 로직 수행
	if (bResult) {
		// 선택된 타워를 강화
		SelectedTower->LevelUp();
	}
	else {
		Cast<UMessageWidget>(MessageWidgetComp->GetWidget())->ShowMessage(TEXT("돈이 부족합니다."), 3.f);
	}

	// 타워 선택을 해제하고 INIT 상태로 돌린다.
	SelectedTower = nullptr;
	SpawnedSelectedTowerChecker->Destroy();
	SpawnedSelectedTowerChecker = nullptr;
	ChangeMode(FTowerControlMode::INIT);
}

void AClientPlayerController::SellTower()
{
	UE_LOG(LogTemp, Log, TEXT("Tower Sell"));

	// 돈 증가
	int32 SellCost = SelectedTower->GetTowerStatComponent()->SellCost();
	AMagicianCharacter* MagicianCharacter = Cast<AMagicianCharacter>(GetPawn());
	MagicianCharacter->ChangeMoney(SellCost);

	// 선택된 타워를 판매
	SelectedTower->Sell();

	// 타워 선택을 해제하고 INIT 상태로 돌린다.
	SelectedTower = nullptr;
	SpawnedSelectedTowerChecker->Destroy();
	SpawnedSelectedTowerChecker = nullptr;
	ChangeMode(FTowerControlMode::INIT);
}

void AClientPlayerController::BuildBarricadeUp()
{
	FTransform SpawnTransform = FTransform(FVector(1150.f, -300.f, 100.f) + FVector(0.f, -300.f, 0.f));
	FRotator SpawnRot = FRotator(0.f, 180.f, 0.f);
	
	// 스폰 준비
	ABarricade* Barricade = GetWorld()->SpawnActorDeferred<ABarricade>(BarricadeClass, SpawnTransform);
	
	// 돈 감소
	AMagicianCharacter * MagicianCharacter = Cast<AMagicianCharacter>(GetPawn());
	bool bResult = MagicianCharacter->ChangeMoney(-(Barricade->GetCost()));

	// 감소에 성공하면 바리케이드 설치
	if (bResult) {
		Barricade->SetBarricadeDirection(FBarricadeDirection::UP);
		Barricade->SetActorRotation(SpawnRot);
		Barricade->SetActorScale3D(FVector(1.f, 1.f, 1.5f));
		Barricade->OnDestroyBarricade.AddUObject(this, &AClientPlayerController::OnDestroyBarricade);
		Barricade->FinishSpawning(SpawnTransform);

		UBarricadeBuildWidget* BarricadeBuildWidget = Cast<UBarricadeBuildWidget>(BarricadeBuildWidgetComp->GetWidget());
		BarricadeBuildWidget->GetBuildUpButtonWidget()->GetButton()->SetIsEnabled(false);
	}
	else { // 실패하면 스폰을 중지
		Barricade->Destroy();
		Cast<UMessageWidget>(MessageWidgetComp->GetWidget())->ShowMessage(TEXT("돈이 부족합니다."), 3.f);
	}

	ChangeMode(FTowerControlMode::INIT);
}

void AClientPlayerController::BuildBarricadeDown()
{
	FTransform SpawnTransform = FTransform(FVector(1150.f, -300.f, 100.f) + FVector(0.f, 900.f, 0.f));
	FRotator SpawnRot = FRotator(0.f, 0.f, 0.f);

	// 스폰 준비
	ABarricade* Barricade = GetWorld()->SpawnActorDeferred<ABarricade>(BarricadeClass, SpawnTransform);

	// 돈 감소
	AMagicianCharacter* MagicianCharacter = Cast<AMagicianCharacter>(GetPawn());
	bool bResult = MagicianCharacter->ChangeMoney(-(Barricade->GetCost()));

	// 감소에 성공하면 바리케이드 설치
	if (bResult) {
		Barricade->SetBarricadeDirection(FBarricadeDirection::DOWN);
		Barricade->SetActorRotation(SpawnRot);
		Barricade->SetActorScale3D(FVector(1.f, 1.f, 1.5f));
		Barricade->OnDestroyBarricade.AddUObject(this, &AClientPlayerController::OnDestroyBarricade);
		Barricade->FinishSpawning(SpawnTransform);

		UBarricadeBuildWidget* BarricadeBuildWidget = Cast<UBarricadeBuildWidget>(BarricadeBuildWidgetComp->GetWidget());
		BarricadeBuildWidget->GetBuildDownButtonWidget()->GetButton()->SetIsEnabled(false);
	}
	else { // 실패하면 스폰을 중지
		Barricade->Destroy();
		Cast<UMessageWidget>(MessageWidgetComp->GetWidget())->ShowMessage(TEXT("돈이 부족합니다."), 3.f);
	}

	ChangeMode(FTowerControlMode::INIT);
}

void AClientPlayerController::BuildBarricadeLeft()
{
	FTransform SpawnTransform = FTransform(FVector(1150.f, -300.f, 100.f) + FVector(-600.f, 300.f, 0.f));
	FRotator SpawnRot = FRotator(0.f, 90.f, 0.f);

	// 스폰 준비
	ABarricade* Barricade = GetWorld()->SpawnActorDeferred<ABarricade>(BarricadeClass, SpawnTransform);

	// 돈 감소
	AMagicianCharacter* MagicianCharacter = Cast<AMagicianCharacter>(GetPawn());
	bool bResult = MagicianCharacter->ChangeMoney(-(Barricade->GetCost()));

	// 감소에 성공하면 바리케이드 설치
	if (bResult) {
		Barricade->SetBarricadeDirection(FBarricadeDirection::LEFT);
		Barricade->SetActorRotation(SpawnRot);
		Barricade->SetActorScale3D(FVector(1.f, 1.f, 1.5f));
		Barricade->OnDestroyBarricade.AddUObject(this, &AClientPlayerController::OnDestroyBarricade);
		Barricade->FinishSpawning(SpawnTransform);

		UBarricadeBuildWidget* BarricadeBuildWidget = Cast<UBarricadeBuildWidget>(BarricadeBuildWidgetComp->GetWidget());
		BarricadeBuildWidget->GetBuildLeftButtonWidget()->GetButton()->SetIsEnabled(false);
	}
	else { // 실패하면 스폰을 중지
		Barricade->Destroy();
		Cast<UMessageWidget>(MessageWidgetComp->GetWidget())->ShowMessage(TEXT("돈이 부족합니다."), 3.f);
	}

	ChangeMode(FTowerControlMode::INIT);
}

void AClientPlayerController::BuildBarricadeRight()
{
	FTransform SpawnTransform = FTransform(FVector(1150.f, -300.f, 100.f) + FVector(600.f, 300.f, 0.f));
	FRotator SpawnRot = FRotator(0.f, 270.f, 0.f);
	
	// 스폰 준비
	ABarricade* Barricade = GetWorld()->SpawnActorDeferred<ABarricade>(BarricadeClass, SpawnTransform);

	// 돈 감소
	AMagicianCharacter* MagicianCharacter = Cast<AMagicianCharacter>(GetPawn());
	bool bResult = MagicianCharacter->ChangeMoney(-(Barricade->GetCost()));

	// 감소에 성공하면 바리케이드 설치
	if (bResult) {
		Barricade->SetBarricadeDirection(FBarricadeDirection::RIGHT);
		Barricade->SetActorRotation(SpawnRot);
		Barricade->SetActorScale3D(FVector(1.f, 1.f, 1.5f));
		Barricade->OnDestroyBarricade.AddUObject(this, &AClientPlayerController::OnDestroyBarricade);
		Barricade->FinishSpawning(SpawnTransform);

		UBarricadeBuildWidget* BarricadeBuildWidget = Cast<UBarricadeBuildWidget>(BarricadeBuildWidgetComp->GetWidget());
		BarricadeBuildWidget->GetBuildRightButtonWidget()->GetButton()->SetIsEnabled(false);
	}
	else { // 실패하면 스폰을 중지
		Barricade->Destroy();
		Cast<UMessageWidget>(MessageWidgetComp->GetWidget())->ShowMessage(TEXT("돈이 부족합니다."), 3.f);
	}

	ChangeMode(FTowerControlMode::INIT);
}

// 현재 선택중인 타워로 프리뷰를 띄운다.
void AClientPlayerController::SetPreviewStaticMesh(int32 SelectedTowerIndex)
{
	if (bShowMouseCursor) {
		ATower* TowerObj = NewObject<ATower>(this, TowerClasses[SelectedTowerIndex]);
		UStaticMesh* PreviewTowerMesh = Cast<UStaticMeshComponent>(TowerObj->GetComponentByClass(UStaticMeshComponent::StaticClass()))->GetStaticMesh();
		SpawningDetector->TowerPreviewComponent->SetStaticMesh(PreviewTowerMesh);
	}
}

// 바리케이드 파괴 이벤트
void AClientPlayerController::OnDestroyBarricade(FBarricadeDirection BarricadeDirection)
{
	UE_LOG(LogTemp, Log, TEXT("Barricade Destroy : %d"), BarricadeDirection)
	UBarricadeBuildWidget* BarricadeBuildWidget = Cast<UBarricadeBuildWidget>(BarricadeBuildWidgetComp->GetWidget());

	switch (BarricadeDirection)
	{
	case FBarricadeDirection::DOWN:
		BarricadeBuildWidget->GetBuildDownButtonWidget()->GetButton()->SetIsEnabled(true);
		break;
	case FBarricadeDirection::RIGHT:
		BarricadeBuildWidget->GetBuildRightButtonWidget()->GetButton()->SetIsEnabled(true);
		break;
	case FBarricadeDirection::UP:
		BarricadeBuildWidget->GetBuildUpButtonWidget()->GetButton()->SetIsEnabled(true);
		break;
	case FBarricadeDirection::LEFT:
		BarricadeBuildWidget->GetBuildLeftButtonWidget()->GetButton()->SetIsEnabled(true);
		break;
	default:
		break;
	}
}
