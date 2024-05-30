// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAnimInstance.h"
#include "EnemyAIController.h"
#include "../Player/Magician/MagicianCharacter.h"
#include "Components/WidgetComponent.h"
#include "EnemyWidget.h"
#include "../ClientGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AEnemyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Stat = CreateDefaultSubobject<UEnemyStatComponent>(TEXT("STAT"));

	EnemyUI = CreateDefaultSubobject<UWidgetComponent>(TEXT("ENEMY_UI"));
	EnemyUI->SetupAttachment(GetMesh());
	EnemyUI->SetRelativeLocation(FVector(0.f, 0.f, 200.f)); // Monster Mesh 크기에 따라 바뀌도록 수정해야함
	EnemyUI->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/Enemy/WBP_Enemy.WBP_Enemy_C'"));
	if (UW.Succeeded()) {
		EnemyUI->SetWidgetClass(UW.Class);
		EnemyUI->SetDrawSize(FVector2D(100.f, 25.f)); // Camera 상태에 따라 바뀌도록 수정해야함
	}
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance) {
		AnimInstance->OnMontageEnded.AddDynamic(this, &AEnemy::OnEnemyMontageEnded);
		AnimInstance->OnEnemyAttackHit.AddUObject(this, &AEnemy::AttackCheck);
	}

	EnemyUI->InitWidget();
	// TODO - Enemy UI binding
	auto EnemyWidget = Cast<UEnemyWidget>(EnemyUI->GetUserWidgetObject());
	if (EnemyWidget)
		EnemyWidget->BindStat(Stat);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::Attack()
{
	if (IsAttacking || IsBeingHit)
		return;

	int32 RandomNumber = FMath::RandRange(0, 2);
	
	AnimInstance->PlayAttackMontage();
	AnimInstance->JumpToSection(0, RandomNumber);
	//AnimInstance->JumpToSection(0, AttackIdx);
	//AttackIdx = (AttackIdx + 1) % 3;

	IsAttacking = true;
}


void AEnemy::AttackCheck() {
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackDamage = Stat->GetAttackDmg();
	float AttackRange = 300.f;
	float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel9,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);
	
	if (bResult && HitResult.Actor.IsValid()) {
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
}


void AEnemy::Hit()
{
	if (IsBeingHit)
		return;
	
	int32 RandomNumber = FMath::RandRange(0, 3);
	
	AnimInstance->PlayHitMontage();
	AnimInstance->JumpToSection(1, RandomNumber);

	IsBeingHit = true;
}

void AEnemy::Die()
{
	if (IsDying)
		return;

	int32 RandomNumber = FMath::RandRange(0, 1);
	
	AnimInstance->PlayDeathMontage();
	AnimInstance->JumpToSection(2, RandomNumber);

	IsDying = true;
}

void AEnemy::OnEnemyMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	FString EnemyName = Stat->GetName();
	FString MontageString = (Montage->GetFName()).ToString();
	if (MontageString == EnemyName + "_AttackMontage") {
		IsAttacking = false;
		OnAttackEnd.Broadcast();
	}
	else if (MontageString == EnemyName + "_HitMontage") {
		IsBeingHit = false;
	}
	else if (MontageString == EnemyName + "_DeathMontage") {
		UE_LOG(LogTemp, Log, TEXT("Enemy Die!"));
		AClientGameMode* GM = Cast<AClientGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		if (GM) GM->DecreaseEnemy();
		Destroy();
	}
	else {
		UE_LOG(LogTemp, Log, TEXT("No Matching Montage Name!"));
		UE_LOG(LogTemp, Log, TEXT("Target Montage Name: %s"), *MontageString);
		FString temp_string = EnemyName + "AttackMontage";
		UE_LOG(LogTemp, Log, TEXT("Real Montage Name: %s"), *temp_string);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Stat->OnAttacked(DamageAmount);
	if (Stat->GetHp() == 0) {
		UClientGameInstance* ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		if (ClientGameInstance)
		{
			int32 Round = ClientGameInstance->GetCurrentStage();
			APawn* DefaultPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
			if (DefaultPawn)
			{
				// DefaultPawn이 ACharacter 클래스 또는 해당 클래스에서 파생된 클래스의 인스턴스인지 확인
				AMagicianCharacter* Character = Cast<AMagicianCharacter>(DefaultPawn);
				if (Character)
				{
					// ACharacter 클래스 또는 해당 클래스에서 파생된 클래스의 인스턴스인 경우에만 실행됨

					// ACharacter 클래스에 정의된 메서드 호출 예시
					Character->ChangeMoney(1000 * Round);
				}
				else
				{
					Character->ChangeMoney(1000);
				}
			}
		}
		Die();		
	}
	else {
		Hit();
		if (DamageCauser)
		{
			// DamageCauser의 기본 콜리전 컴포넌트 가져오기
			UPrimitiveComponent* DamageCauserComponent = DamageCauser->FindComponentByClass<UPrimitiveComponent>();
			if (DamageCauserComponent)
			{
				FName CollisionProfileName = DamageCauserComponent->GetCollisionProfileName();
				if (CollisionProfileName == TEXT("Projectile"))
				{
					UE_LOG(LogTemp, Log, TEXT("DamageCauser is a Tower Projectile, skipping additional functionality."));
				}
				else if (CollisionProfileName == TEXT("Tower")) {
					UE_LOG(LogTemp, Log, TEXT("DamageCauser is a AOE Tower, skipping additional functionality."));
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("DamageCauser is : %s"), *DamageCauser->GetName());
					// AIController의 Blackboard 변수 변경
					AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());
					if (EnemyAIController)
					{
						ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
						if (PlayerCharacter)
						{
							EnemyAIController->OnPlayerHit(PlayerCharacter);
						}
					}
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("DamageCauser has no primitive component."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("DamageCauser is None"));
		}
	}
	return DamageAmount;
}