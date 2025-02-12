// Fill out your copyright notice in the Description page of Project Settings.
#include "MagicianCharacter.h"
#include "GameFrameWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "MagicianQProjectile.h"
#include "MagicianEProjectile.h"
#include "MagicianRProjectile.h"
#include "MagicianRightProjectile.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "MagicianAnimInstance.h"
#include "MagicianStatComponent.h"
#include "Components/WidgetComponent.h"
#include "NiagaraComponent.h"

#include "GameEndWidget.h"
#include "../../ClientGameInstance.h"
#include "../../Enemy/Enemy.h"


// Sets default values
AMagicianCharacter::AMagicianCharacter()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(GetMesh(), "head"); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FPSCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/EssentialAnimation/MagicStaff/Demo/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'")); // 情軒杖 殖遭拭辞 亜閃臣 神崎詮闘研 差紫背辞 ""拭 隔澗陥
	if (SM.Succeeded()) {
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	FName WeaponSocket(TEXT("weapon_r"));
	if (GetMesh()->DoesSocketExist(WeaponSocket))
	{
		Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>SW(TEXT("StaticMesh'/Game/EssentialAnimation/MagicStaff/Demo/Mannequin/Weapon/SM_Staff.SM_Staff'"));
		if (SW.Succeeded()) {
			Weapon->SetStaticMesh(SW.Object);
		}

		Weapon->SetupAttachment(GetMesh(), WeaponSocket);
	}

	FName WeaponEffectSocket(TEXT("SwordAura_R"));
	if (GetMesh()->DoesSocketExist(WeaponEffectSocket))
	{
		WeaponEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WEAPONEFFECT"));
		static ConstructorHelpers::FObjectFinder<UNiagaraSystem>WE(TEXT("NiagaraSystem'/Game/sA_SwordVfx/Fx/NS_SwordAura_1.NS_SwordAura_1'"));
		if (WE.Succeeded())
		{
			WeaponEffect->SetAsset(WE.Object);
		}

		WeaponEffect->SetupAttachment(GetMesh(), WeaponEffectSocket);

		// Y逐 闘沓什廿 -20 竺舛
		WeaponEffect->SetRelativeLocation(FVector(0.0f, -20.0f, 0.0f));

		// Z逐 噺穿 90亀 竺舛 (舘是: 虞巨照)
		WeaponEffect->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

		// X逐 掩戚 1.5壕, Y逐 掩戚 1.0壕, Z逐 掩戚 1.0壕稽 竺舛
		WeaponEffect->SetRelativeScale3D(FVector(1.5f, 1.0f, 1.0f)); // 滴奄 竺舛

		FLinearColor DefaultColor(0.0f, 0.0f, 0.0f, 0.0f);
		WeaponEffect->SetNiagaraVariableLinearColor("Color01", DefaultColor);
		WeaponEffect->ReinitializeSystem();
	}

	TeleportParticleSystem = LoadObject<UParticleSystem>(nullptr, TEXT("ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_hit1.P_ky_hit1'"));

	MagicianStat = CreateDefaultSubobject<UMagicianStatComponent>(TEXT("MagicianStat"));

	if (MagicianStat)
	{
		MagicianStat->OnNoLifeChanged.AddUObject(this, &AMagicianCharacter::CallGameEndWidget);
	}

	ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/InGameWidgets/Player/WBP_ScreenWidget.WBP_ScreenWidget_C'"));
	if (UW.Succeeded())
	{
		ScreenWidget = CreateWidget<UUserWidget>(GetWorld(), UW.Class);
		if (ScreenWidget)
		{
			ScreenWidget->AddToViewport(); // 是銭聖 鉢檎拭 蓄亜
		}
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UWS(TEXT("WidgetBlueprint'/Game/UI/InGameWidgets/Player/WBP_WeaponLevelUpSuccessWidget.WBP_WeaponLevelUpSuccessWidget_C'"));
	if (UWS.Succeeded())
	{
		WeaponLevelUpSuccessWidget = CreateWidget<UUserWidget>(GetWorld(), UWS.Class);
		if (WeaponLevelUpSuccessWidget)
		{
			WeaponLevelUpSuccessWidget->AddToViewport();
			WeaponLevelUpSuccessWidget->SetVisibility(ESlateVisibility::Hidden);

		}
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UWF(TEXT("WidgetBlueprint'/Game/UI/InGameWidgets/Player/WBP_WeaponLevelUpFailWidget.WBP_WeaponLevelUpFailWidget_C'"));
	if (UWF.Succeeded())
	{
		WeaponLevelUpFailWidget = CreateWidget<UUserWidget>(GetWorld(), UWF.Class);
		if (WeaponLevelUpFailWidget)
		{
			WeaponLevelUpFailWidget->AddToViewport();
			WeaponLevelUpFailWidget->SetVisibility(ESlateVisibility::Hidden);

		}
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> UWBF(TEXT("WidgetBlueprint'/Game/UI/InGameWidgets/Player/WBP_WeaponLevelUpBigFailWidget.WBP_WeaponLevelUpBigFailWidget_C'"));
	if (UWBF.Succeeded())
	{
		WeaponLevelUpBigFailWidget = CreateWidget<UUserWidget>(GetWorld(), UWBF.Class);
		if (WeaponLevelUpBigFailWidget)
		{
			WeaponLevelUpBigFailWidget->AddToViewport();
			WeaponLevelUpBigFailWidget->SetVisibility(ESlateVisibility::Hidden);

		}
	}


	static ConstructorHelpers::FClassFinder<UUserWidget> ULS(TEXT("WidgetBlueprint'/Game/UI/InGameWidgets/Player/WBP_LevelUpSuccess.WBP_LevelUpSuccess_C'"));
	if (ULS.Succeeded())
	{
		LevelUpSuccessWidget = CreateWidget<UUserWidget>(GetWorld(), ULS.Class);
		if (LevelUpSuccessWidget)
		{
			LevelUpSuccessWidget->AddToViewport();
			LevelUpSuccessWidget->SetVisibility(ESlateVisibility::Hidden);

		}
	}

	static ConstructorHelpers::FClassFinder<UGameEndWidget> UGE(TEXT("WidgetBlueprint'/Game/UI/InGameWidgets/Player/WBP_GameEndWidget.WBP_GameEndWidget_C'"));
	if (UGE.Succeeded())
	{
		GameEndWidget = CreateWidget<UGameEndWidget>(GetWorld(), UGE.Class);
		if (GameEndWidget)
		{
			GameEndWidget->AddToViewport();
			GameEndWidget->SetVisibility(ESlateVisibility::Hidden);

		}
	}


	//HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	//HpBar->SetupAttachment(GetMesh());
	//HpBar->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
	//HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	//static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_HPBar.WBP_HPBar_C'"));
	//if (UW.Succeeded())
	//{
	//	HpBar->SetWidgetClass(UW.Class);
	//	HpBar->SetDrawSize(FVector2D(200.f, 50.f));
	//}


}

// Called when the game starts or when spawned
void AMagicianCharacter::BeginPlay()
{
	Super::BeginPlay();

	

}

void AMagicianCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AnimInstance = Cast<UMagicianAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMagicianCharacter::OnAnimMontageEnded);
		AnimInstance->OnAttackQHit.AddUObject(this, &AMagicianCharacter::AttackQCheck);
		AnimInstance->OnAttackEHit.AddUObject(this, &AMagicianCharacter::AttackECheck);
		AnimInstance->OnAttackRHit.AddUObject(this, &AMagicianCharacter::AttackRCheck);
		AnimInstance->OnAttackRightHit.AddUObject(this, &AMagicianCharacter::AttackRightCheck);
		AnimInstance->OnAttackLeftHit.AddUObject(this, &AMagicianCharacter::AttackLeftCheck);
	}

	// ScreenWidget->InitWidget();

	auto SW = Cast<UScreenWidget>(ScreenWidget);
	if (SW)
	{
		SW->BindHp(MagicianStat);
		SW->BindPlayerLevel(MagicianStat);
		SW->BindWeaponLevel(MagicianStat);
		SW->BindRightCoolTime(MagicianStat);
		SW->BindShiftCoolTime(MagicianStat);
		SW->BindQCoolTime(MagicianStat);
		SW->BindECoolTime(MagicianStat);
		SW->BindRCoolTime(MagicianStat);
		SW->BindMoney(MagicianStat);
		SW->BindLife(MagicianStat);

		/*
		if (ClientGameInstance)
		{
			SW->BindMonsterKillCount(ClientGameInstance);
			SW->BindCurrentStage(ClientGameInstance);
		}
		*/
	}
	auto GEW = Cast<UGameEndWidget>(GameEndWidget);
	if (GEW)
	{
		GEW->BindRankingList();
	}

	// 蝶遣斗 什堵 亜閃神奄


}

// Called every frame
void AMagicianCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMagicianCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	// PlayerInputComponent->BindAction(TEXT("ClickLeft"), IE_Pressed, this, &AMagicianCharacter::ClickLeft);
	PlayerInputComponent->BindAction(TEXT("AttackRight"), IE_Pressed, this, &AMagicianCharacter::AttackRight);

	PlayerInputComponent->BindAction(TEXT("AttackQ"), IE_Pressed, this, &AMagicianCharacter::AttackQ);
	PlayerInputComponent->BindAction(TEXT("AttackE"), IE_Pressed, this, &AMagicianCharacter::AttackE);
	PlayerInputComponent->BindAction(TEXT("AttackR"), IE_Pressed, this, &AMagicianCharacter::AttackR);
	PlayerInputComponent->BindAction(TEXT("Teleport"), IE_Pressed, this, &AMagicianCharacter::Teleport);
	PlayerInputComponent->BindAction(TEXT("ViewChange"), IE_Pressed, this, &AMagicianCharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("WeaponLevelUp"), IE_Pressed, this, &AMagicianCharacter::WeaponLevelUp);
	PlayerInputComponent->BindAction(TEXT("LevelUp"), IE_Pressed, this, &AMagicianCharacter::LevelUp);
	PlayerInputComponent->BindAction(TEXT("ExitRoom"), IE_Pressed, this, &AMagicianCharacter::ExitRoom);


	PlayerInputComponent->BindAxis("MoveForward", this, &AMagicianCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMagicianCharacter::MoveRight);
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMagicianCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMagicianCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMagicianCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMagicianCharacter::TouchStopped);

}

void AMagicianCharacter::ExitRoom()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
}

void AMagicianCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (IsAttacking)
	{
		return;
	}
	if (IsDeath)
	{
		return;
	}
	Jump();
}


void AMagicianCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (IsAttacking)
	{
		return;
	}
	if (IsDeath)
	{
		return;
	}
	StopJumping();
}

float AMagicianCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	if (MagicianStat->GetLife() == 0)
		return 0.0f;

	if (MagicianStat->GetHp() > 0.0f)
	{
		UE_LOG(LogTemp, Error, TEXT("Take Damage - Damage Amount : %f"), DamageAmount);
		MagicianStat->OnAttacked(DamageAmount);
		if (MagicianStat->GetHp() == 0)
		{
			Die();
			MagicianStat->SetLife(MagicianStat->GetLife() - 1);
			// OnLifeStatusChanged.Broadcast();
		}
		return DamageAmount;
	}
	return 0.0f;
}

bool AMagicianCharacter::ChangeMoney(int32 Money)
{
	if (MagicianStat->GetMoney() + Money < 0)
	{
		return false;
	}

	if (Money > 0)
	{
		MagicianStat->PlusMoney(Money);
	}
	else if (Money < 0)
	{
		MagicianStat->MinusMoney(Money);
	}

	return true;
}

void AMagicianCharacter::CallGameEndWidget()
{
	if (GameEndWidget)
	{
		GameEndWidget->PostRankingList();
		// GameEndWidget->GetRankingList();
		GameEndWidget->SetVisibility(ESlateVisibility::Visible);
		
		
		AController* MyController = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetController();;

		if (MyController)
		{
			APlayerController* PlayerController = Cast<APlayerController>(MyController);
			if (PlayerController) // 蝶什特戚 失因梅澗走 溌昔
			{
				FInputModeGameAndUI InputModeData;
				InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				InputModeData.SetHideCursorDuringCapture(false);
				PlayerController->SetInputMode(InputModeData);
				PlayerController->SetShowMouseCursor(true);
				PlayerController->bShowMouseCursor = true;
			}
			else
			{
				// PlayerController 蝶什特 叔鳶 獣 坦軒拝 稽送
				UE_LOG(LogTemp, Warning, TEXT("PlayerController casting failed."));
			}
		}
		else
		{
			// MyController亜 政反馬走 省精 井酔 坦軒拝 稽送
			UE_LOG(LogTemp, Warning, TEXT("MyController is not valid."));
		}

	}
	
}

void AMagicianCharacter::WeaponEffectChange()
{
	UE_LOG(LogTemp, Log, TEXT("weapon effect changed, %d"), MagicianStat != nullptr)
		UE_LOG(LogTemp, Log, TEXT("weapon effect changed, %d"), WeaponEffect != nullptr)
		if (MagicianStat && WeaponEffect)
		{

			int32 WeaponLevel = MagicianStat->GetWeaponLevel();

			if (WeaponLevel == 2)
			{
				UE_LOG(LogTemp, Log, TEXT("Weapon effect level 2"));
				FLinearColor RedColor(1.0f, 0.0f, 0.0f, 1.0f);
				WeaponEffect->SetNiagaraVariableLinearColor("Color01", RedColor);
				WeaponEffect->ReinitializeSystem();
			}
			else if (WeaponLevel == 3)
			{
				UE_LOG(LogTemp, Log, TEXT("Weapon effect level 2"));
				FLinearColor OrangeColor(1.0f, 0.647f, 0.0f, 1.0f);
				WeaponEffect->SetNiagaraVariableLinearColor("Color01", OrangeColor);
				WeaponEffect->ReinitializeSystem();
			}
			else if (WeaponLevel == 4)
			{
				UE_LOG(LogTemp, Log, TEXT("Weapon effect level 3"));
				FLinearColor YellowColor(1.0f, 1.0f, 0.0f, 1.0f);
				WeaponEffect->SetNiagaraVariableLinearColor("Color01", YellowColor);
				WeaponEffect->ReinitializeSystem();
			}
			else if (WeaponLevel == 5)
			{
				UE_LOG(LogTemp, Log, TEXT("Weapon effect level 4"));
				FLinearColor GreenColor(0.0f, 1.0f, 0.0f, 1.0f);
				WeaponEffect->SetNiagaraVariableLinearColor("Color01", GreenColor);
				WeaponEffect->ReinitializeSystem();
			}
			else if (WeaponLevel == 6)
			{
				UE_LOG(LogTemp, Log, TEXT("Weapon effect level 5"));
				FLinearColor BlueColor(0.0f, 0.0f, 1.0f, 1.0f);
				WeaponEffect->SetNiagaraVariableLinearColor("Color01", BlueColor);
				WeaponEffect->ReinitializeSystem();
			}
			else if (WeaponLevel == 7)
			{
				UE_LOG(LogTemp, Log, TEXT("Weapon effect level 6"));
				FLinearColor IndigoColor(0.294f, 0.0f, 0.509f, 1.0f);
				WeaponEffect->SetNiagaraVariableLinearColor("Color01", IndigoColor);
				WeaponEffect->ReinitializeSystem();
			}
			else if (WeaponLevel == 8)
			{
				UE_LOG(LogTemp, Log, TEXT("Weapon effect level 7"));
				FLinearColor PurpleColor(0.5f, 0.0f, 0.5f, 1.0f);
				WeaponEffect->SetNiagaraVariableLinearColor("Color01", PurpleColor);
				WeaponEffect->ReinitializeSystem();
			}
			else if (WeaponLevel == 9)
			{
				UE_LOG(LogTemp, Log, TEXT("Weapon effect level 8"));
				FLinearColor WhiteColor(1.0f, 1.0f, 1.0f, 1.0f);
				WeaponEffect->SetNiagaraVariableLinearColor("Color01", WhiteColor);
				WeaponEffect->ReinitializeSystem();
			}
			else if (WeaponLevel == 10)
			{
				UE_LOG(LogTemp, Log, TEXT("Weapon effect level 9"));
				FLinearColor BlackColor(0.0f, 0.0f, 0.0f, 1.0f);
				WeaponEffect->SetNiagaraVariableLinearColor("Color01", BlackColor);
				WeaponEffect->ReinitializeSystem();
			}

		}
}

void AMagicianCharacter::Die()
{
	IsDeath = true;
	if (AnimInstance)
	{
		AnimInstance->PlayDeathMontage();
	}
}

void AMagicianCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMagicianCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}


void AMagicianCharacter::MoveForward(float Value)
{
	if (IsAttacking)
	{
		return;
	}
	if (IsDeath)
	{
		return;
	}
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMagicianCharacter::MoveRight(float Value)
{
	if (IsAttacking)
	{
		return;
	}
	if (IsDeath)
	{
		return;
	}
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

// 社什 督析
void AMagicianCharacter::ViewChange()
{
	if (FollowCamera->IsActive())
	{
		FollowCamera->SetActive(false);
		FPSCamera->SetActive(true);
	}
	else
	{
		FollowCamera->SetActive(true);
		FPSCamera->SetActive(false);
	}
	//bIsFirstPersonView = !bIsFirstPersonView;

	//FName HeadCameraSocket(TEXT("head_camera"));

	//if (bIsFirstPersonView)
	//{
	//	// 1昔暢 獣繊 竺舛
	//	/*FollowCamera->SetRelativeLocation(FVector(530.0f, 0.0f, 75.0f));
	//	FollowCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));*/

	//	// 奄沙 朝五虞 是帖研 戚原拭 是帖獣鉄 (琶推拭 魚虞 繕舛 亜管)
	//	if (GetMesh()->DoesSocketExist(HeadCameraSocket))
	//	{
	//		FollowCamera->SetupAttachment(GetMesh(), HeadCameraSocket);
	//		UE_LOG(LogTemp, Log, TEXT("せせせせ %s"), *FollowCamera->GetComponentLocation().ToString());
	//		UE_LOG(LogTemp, Log, TEXT("せせせせ %s"), *GetMesh()->GetSocketLocation(HeadCameraSocket).ToString());
	//	}
	//}
	//else
	//{
	//	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//	UE_LOG(LogTemp, Log, TEXT("せせせせ %s"), *FollowCamera->GetComponentLocation().ToString());
	//}
}

void AMagicianCharacter::AttackLeft()
{
	if (IsAttacking)
		return;
	if (IsDeath)
	{
		return;
	}
	if (AnimInstance)
	{
		AnimInstance->PlayAttackLeftMontage();
		AnimInstance->JumpToLeftSection(AttackLeftIndex);
		AttackLeftIndex = (AttackLeftIndex + 1) % 3;
		IsAttacking = true;
	}
}

void AMagicianCharacter::AttackLeftCheck()
{
	UE_LOG(LogTemp, Log, TEXT("left attack"));
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);
	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	//DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius, Rotation, DrawColor, false, 2.f);

	if (bResult && HitResult.Actor.IsValid()) {
		AEnemy* Enemy = Cast<AEnemy>(HitResult.Actor);
		if (Enemy)
		{
			UE_LOG(LogTemp, Log, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName())
				UE_LOG(LogTemp, Log, TEXT("left attack damgage : %f"), MagicianStat->GetLeftDamage() * MagicianStat->GetAttackCoefficient());
			FDamageEvent DamageEvent;
			Enemy->TakeDamage(MagicianStat->GetLeftDamage(), DamageEvent, GetInstigatorController(), this);
		}

	}
}
void AMagicianCharacter::AttackRight()
{
	if (IsAttacking)
		return;
	if (IsDeath)
	{
		return;
	}
	if (MagicianStat->IsSkillReady("Right") == false)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Attack Right Ready"));

	if (AnimInstance)
	{
		AnimInstance->PlayAttackRightMontage();
		AnimInstance->JumpToRightSection(AttackRightIndex);
		AttackRightIndex = (AttackRightIndex + 1) % 2;
		IsAttacking = true;
		MagicianStat->ActivateSkill("Right");
	}

}

void AMagicianCharacter::AttackRightCheck()
{
	if (ProjectileRightClass)
	{
		// 朝五虞 闘沓什廿聖 姥杯艦陥.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// MuzzleOffset 聖 朝五虞 什凪戚什拭辞 杉球 什凪戚什稽 痕発杯艦陥.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;
		// 繕層聖 鉦娃 性楕生稽 臣形掃艦陥.
		MuzzleRotation.Pitch += 10.0f;
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			// 恥姥 是帖拭 降紫端研 什肉獣典艦陥.
			AMagicianRightProjectile* ProjectileRight = World->SpawnActorDeferred<AMagicianRightProjectile>(ProjectileRightClass, FTransform(MuzzleRotation, MuzzleLocation), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			if (ProjectileRight)
			{
				ProjectileRight->DamageAmount = MagicianStat->GetRightDamage() * MagicianStat->GetAttackCoefficient();
				// 降紫 号狽聖 硝焼垣艦陥.
				FVector LaunchDirection = MuzzleRotation.Vector();
				ProjectileRight->FinishSpawning(FTransform(MuzzleRotation, MuzzleLocation));
				ProjectileRight->FireInDirection(LaunchDirection);
			}
		}
	}
}


void AMagicianCharacter::AttackQ()
{
	if (IsAttacking)
		return;
	if (IsDeath)
	{
		return;
	}
	if (MagicianStat->IsSkillReady("Q") == false)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Attack Q Ready"));
	if (AnimInstance)
	{
		AnimInstance->PlayAttackQMontage();
		// 覗稽詮展析 降紫研 獣亀杯艦陥.

		AnimInstance->JumpToQSection(AttackQIndex);
		AttackQIndex = (AttackQIndex + 1) % 2;
		IsAttacking = true;
		MagicianStat->ActivateSkill("Q");
	}
}


void AMagicianCharacter::AttackQCheck()
{
	if (ProjectileQClass)
	{
		// 朝五虞 闘沓什廿聖 姥杯艦陥.
		FVector CameraLocation;
		FRotator CameraRotation;
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		// MuzzleOffset聖 朝五虞 什凪戚什拭辞 杉球 什凪戚什稽 痕発杯艦陥.
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRotation;

		// 繕層聖 鉦娃 性楕生稽 臣形掃艦陥.
		MuzzleRotation.Pitch += 10.0f;

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			// 恥姥 是帖拭 降紫端研 什肉獣典艦陥.
			const float AngleOffset = 10.0f; // 降紫端 娃税 唖亀 託戚 (亀 舘是)

			FRotator ProjectileQ1Rotation = MuzzleRotation - FRotator(0.0f, AngleOffset, 0.0f);
			AMagicianQProjectile* ProjectileQ1 = World->SpawnActorDeferred<AMagicianQProjectile>(ProjectileQClass, FTransform(ProjectileQ1Rotation, MuzzleLocation), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

			FRotator ProjectileQ2Rotation = MuzzleRotation;
			AMagicianQProjectile* ProjectileQ2 = World->SpawnActorDeferred<AMagicianQProjectile>(ProjectileQClass, FTransform(ProjectileQ2Rotation, MuzzleLocation), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

			FRotator ProjectileQ3Rotation = MuzzleRotation + FRotator(0.0f, AngleOffset, 0.0f);
			AMagicianQProjectile* ProjectileQ3 = World->SpawnActorDeferred<AMagicianQProjectile>(ProjectileQClass, FTransform(ProjectileQ3Rotation, MuzzleLocation), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

			if (ProjectileQ1)
			{
				ProjectileQ1->DamageAmount = MagicianStat->GetQDamage() * MagicianStat->GetAttackCoefficient();
				FVector LaunchDirection = ProjectileQ1Rotation.Vector();
				ProjectileQ1->FinishSpawning(FTransform(ProjectileQ1Rotation, MuzzleLocation));
				ProjectileQ1->FireInDirection(LaunchDirection);
			}

			if (ProjectileQ2)
			{
				ProjectileQ2->DamageAmount = MagicianStat->GetQDamage() * MagicianStat->GetAttackCoefficient();
				FVector LaunchDirection = ProjectileQ2Rotation.Vector();
				ProjectileQ2->FinishSpawning(FTransform(ProjectileQ2Rotation, MuzzleLocation));
				ProjectileQ2->FireInDirection(LaunchDirection);
			}

			if (ProjectileQ3)
			{
				ProjectileQ3->DamageAmount = MagicianStat->GetQDamage() * MagicianStat->GetAttackCoefficient();
				FVector LaunchDirection = ProjectileQ3Rotation.Vector();
				ProjectileQ3->FinishSpawning(FTransform(ProjectileQ3Rotation, MuzzleLocation));
				ProjectileQ3->FireInDirection(LaunchDirection);
			}
		}
	}
}

void AMagicianCharacter::AttackE()
{
	if (IsAttacking)
		return;
	if (IsDeath)
	{
		return;
	}
	if (MagicianStat->IsSkillReady("E") == false)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Attack E Ready"));
	if (AnimInstance)
	{
		AnimInstance->PlayAttackEMontage();
		IsAttacking = true;
		MagicianStat->ActivateSkill("E");
	}

}


void AMagicianCharacter::AttackECheck()
{
	if (ProjectileEClass)
	{
		// Actor 闘沓什廿
		FVector ActorLocation = GetActorLocation();
		FRotator ActorRotation = GetActorRotation();

		FVector MuzzleLocation = ActorLocation + ActorRotation.Vector() * 1500;
		MuzzleLocation.Z = 1000;

		FRotator MuzzleRotation = FRotator(0, 0, 0);

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AMagicianEProjectile* ProjectileE = World->SpawnActorDeferred<AMagicianEProjectile>(ProjectileEClass, FTransform(MuzzleRotation, MuzzleLocation), this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			if (ProjectileE)
			{
				// 覗稽詮展析拭 DamageAmount 竺舛
				ProjectileE->DamageAmount = MagicianStat->GetEDamage() * MagicianStat->GetAttackCoefficient();

				// 降紫端税 噺穿聖 竺舛杯艦陥.
				FRotator NewRotation = MuzzleRotation;
				NewRotation.Pitch = 0.0f; // X 逐 噺穿聖 0生稽 竺舛馬食 呪汝生稽 幻旧艦陥.
				ProjectileE->SetActorRotation(NewRotation);

				// FinishSpawning聖 硲窒馬食 降紫端研 杉球拭 蓄亜杯艦陥.
				ProjectileE->FinishSpawning(FTransform(MuzzleRotation, MuzzleLocation));

				// 降紫端税 是帖研 竺舛杯艦陥.
				FHitResult HitResult;
				FVector StartTrace = ProjectileE->GetActorLocation();
				FVector EndTrace = StartTrace - FVector(0.f, 0.f, 1000.f); // 降紫端 焼掘稽 1000税 暗軒幻鏑 傾戚研 助艦陥.

				FCollisionQueryParams CollisionParams;
				CollisionParams.bReturnPhysicalMaterial = false;

				if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldStatic, CollisionParams))
				{
					// 降紫端研 傾戚人 中宜廃 走繊生稽 戚疑獣典艦陥.
					ProjectileE->SetActorLocation(HitResult.ImpactPoint);
				}
			}
		}
	}
}
//
//void AMagicianCharacter::AttackECheck()
//{
//	if (ProjectileEClass)
//	{
//		// Actor 闘沓什廿
//		FVector ActorLocation = GetActorLocation();
//		FRotator ActorRotation = GetActorRotation();
//		
//		FVector MuzzleLocation = ActorLocation + ActorRotation.Vector() * 1500;
//		MuzzleLocation.Z = 1000;
//
//		FRotator MuzzleRotation = FRotator(0, 0, 0);
//
//		UWorld* World = GetWorld();
//		if (World)
//		{
//			FActorSpawnParameters SpawnParams;
//			SpawnParams.Owner = this;
//			AMagicianEProjectile* ProjectileE = World->SpawnActor<AMagicianEProjectile>(ProjectileEClass, MuzzleLocation, MuzzleRotation, SpawnParams);
//
//			if (ProjectileE)
//			{
//				// 降紫端税 噺穿聖 竺舛杯艦陥.
//				FRotator NewRotation = ProjectileE->GetActorRotation();
//
//				NewRotation.Pitch = 0.0f; // X 逐 噺穿聖 0生稽 竺舛馬食 呪汝生稽 幻旧艦陥.
//				ProjectileE->SetActorRotation(NewRotation);
//				// 降紫端税 是帖研 竺舛杯艦陥.
//				FHitResult HitResult;
//				FVector StartTrace = ProjectileE->GetActorLocation();
//				FVector EndTrace = StartTrace - FVector(0.f, 0.f, 1000.f); // 降紫端 焼掘稽 1000税 暗軒幻鏑 傾戚研 助艦陥.
//
//				FCollisionQueryParams CollisionParams;
//				CollisionParams.bReturnPhysicalMaterial = false;
//
//				if (GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_WorldStatic, CollisionParams))
//				{
//					// 降紫端研 傾戚人 中宜廃 走繊生稽 戚疑獣典艦陥.
//					ProjectileE->SetActorLocation(HitResult.ImpactPoint);
//				}
//				//什倒 竺舛 背醤敗
//				//PlayerStat->ActivateSkill("E");
//			}
//		}
//	}
//}
void AMagicianCharacter::AttackR()
{
	if (IsAttacking)
		return;
	if (IsDeath)
	{
		return;
	}
	if (MagicianStat->IsSkillReady("R") == false)
		return;

	UE_LOG(LogTemp, Warning, TEXT("Attack R Ready"));
	if (AnimInstance)
	{
		AnimInstance->PlayAttackRMontage();
		IsAttacking = true;
		MagicianStat->ActivateSkill("R");
	}

}

void AMagicianCharacter::WeaponLevelUp()
{

	if (MagicianStat)
	{
		bool WeaponLevelUpAvailable = MagicianStat->GetWeaponLevelUpAvailable();
		int32 Money = MagicianStat->GetMoney();
		int32 WeaponLevelUpCost = MagicianStat->GetWeaponLevelUpCost();

		if (WeaponLevelUpAvailable && Money - WeaponLevelUpCost >= 0)
		{
			MagicianStat->MinusMoney(-WeaponLevelUpCost);
			UE_LOG(LogTemp, Error, TEXT("MinusMoneyResult : %d"), MagicianStat->GetMoney());
			float WeaponLevelUpPercentage = MagicianStat->GetWeaponLevelUpPercentage();
			float RandomFloat = FMath::RandRange(0.0f, 1.0f);
			if (RandomFloat <= WeaponLevelUpPercentage) // 失因績
			{
				MagicianStat->SetWeaponLevelUp();
				UE_LOG(LogTemp, Log, TEXT("weapon level up!"));
				if (WeaponLevelUpSuccessWidget)
				{
					LevelUpSuccessWidget->SetVisibility(ESlateVisibility::Hidden);
					WeaponLevelUpFailWidget->SetVisibility(ESlateVisibility::Hidden);
					WeaponLevelUpBigFailWidget->SetVisibility(ESlateVisibility::Hidden);

					WeaponLevelUpSuccessWidget->SetVisibility(ESlateVisibility::Visible);
					// 展戚袴 輩級 痕呪
					FTimerHandle TimerHandle;

					// 2段 板拭 叔楳吃 寓陥 敗呪
					auto HideLambda = [&]()
					{
						// 是銭 需奄奄
						WeaponLevelUpSuccessWidget->SetVisibility(ESlateVisibility::Hidden);
					};

					// 2段 板拭 寓陥 敗呪 叔楳
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, HideLambda, 2.0f, false);
					WeaponEffectChange();
				}
			}
			else // 叔鳶績
			{
				UE_LOG(LogTemp, Log, TEXT("fail, %f"), RandomFloat);
				float WeaponLevelDownPercentage = 0.1f;
				float RandomFloat2 = FMath::RandRange(0.0f, 1.0f);
				// 企企企
				if (RandomFloat2 <= WeaponLevelDownPercentage)
				{
					MagicianStat->SetWeaponLevelDown();
					if (WeaponLevelUpFailWidget)
					{
						LevelUpSuccessWidget->SetVisibility(ESlateVisibility::Hidden);
						WeaponLevelUpSuccessWidget->SetVisibility(ESlateVisibility::Hidden);
						WeaponLevelUpFailWidget->SetVisibility(ESlateVisibility::Hidden);

						WeaponLevelUpBigFailWidget->SetVisibility(ESlateVisibility::Visible);
						// 展戚袴 輩級 痕呪
						FTimerHandle TimerHandle;

						// 2段 板拭 叔楳吃 寓陥 敗呪
						auto HideLambda = [&]()
						{
							// 是銭 需奄奄
							WeaponLevelUpBigFailWidget->SetVisibility(ESlateVisibility::Hidden);
						};

						// 2段 板拭 寓陥 敗呪 叔楳
						GetWorld()->GetTimerManager().SetTimer(TimerHandle, HideLambda, 2.0f, false);
						WeaponEffectChange();
					}
				}
				else
				{
					if (WeaponLevelUpFailWidget)
					{
						LevelUpSuccessWidget->SetVisibility(ESlateVisibility::Hidden);
						WeaponLevelUpSuccessWidget->SetVisibility(ESlateVisibility::Hidden);
						WeaponLevelUpBigFailWidget->SetVisibility(ESlateVisibility::Hidden);

						WeaponLevelUpFailWidget->SetVisibility(ESlateVisibility::Visible);

						// 展戚袴 輩級 痕呪
						FTimerHandle TimerHandle;

						// 2段 板拭 叔楳吃 寓陥 敗呪
						auto HideLambda = [&]()
						{
							// 是銭 需奄奄
							WeaponLevelUpFailWidget->SetVisibility(ESlateVisibility::Hidden);
						};

						// 2段 板拭 寓陥 敗呪 叔楳
						GetWorld()->GetTimerManager().SetTimer(TimerHandle, HideLambda, 2.0f, false);

					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NO MONEY TO WEAPONLEVELUP"));
		}
	}
}

void AMagicianCharacter::LevelUp()
{
	if (IsDeath)
	{
		return;
	}
	if (MagicianStat)
	{
		bool LevelUpAvailable = MagicianStat->GetLevelUpAvailable();
		int32 Money = MagicianStat->GetMoney();
		int32 LevelUpCost = MagicianStat->GetLevelUpCost();
		if (LevelUpAvailable && Money - LevelUpCost >= 0)
		{
			MagicianStat->SetLevel();
			FVector CharacterLocation = GetActorLocation();
			CharacterLocation.Z -= 77.f;
			UParticleSystemComponent* ParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LevelUpParticleSystem, CharacterLocation, FRotator::ZeroRotator, true);
			if (ParticleSystem)
			{
				ParticleSystem->SetRelativeScale3D(FVector(1.3f, 1.3f, 1.6f));
				FTimerHandle TimerHandle;
				float DestroyDelay = 0.77f; // 3段 走尻

				GetWorldTimerManager().SetTimer(TimerHandle, [ParticleSystem]()
					{
						ParticleSystem->DestroyComponent();
					}, DestroyDelay, false);
			}

			UE_LOG(LogTemp, Log, TEXT("level up!"));
			if (LevelUpSuccessWidget)
			{
				WeaponLevelUpSuccessWidget->SetVisibility(ESlateVisibility::Hidden);
				WeaponLevelUpFailWidget->SetVisibility(ESlateVisibility::Hidden);
				WeaponLevelUpBigFailWidget->SetVisibility(ESlateVisibility::Hidden);

				LevelUpSuccessWidget->SetVisibility(ESlateVisibility::Visible);
				// 展戚袴 輩級 痕呪
				FTimerHandle TimerHandle;

				// 2段 板拭 叔楳吃 寓陥 敗呪
				auto HideLambda = [&]()
				{
					// 是銭 需奄奄
					LevelUpSuccessWidget->SetVisibility(ESlateVisibility::Hidden);
				};

				// 2段 板拭 寓陥 敗呪 叔楳
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, HideLambda, 2.0f, false);
			}
		}
	}
}

void AMagicianCharacter::AttackRCheck()
{
	if (ProjectileRClass)
	{
		FVector ActorLocation = GetActorLocation();
		FRotator ActorRotation = GetActorRotation();

		FVector MuzzleLocation = ActorLocation + ActorRotation.Vector() * 2000;
		MuzzleLocation.Z = 4000;

		FRotator MuzzleRotation = FRotator(0, 0, 0);

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			AMagicianRProjectile* ProjectileR = World->SpawnActorDeferred<AMagicianRProjectile>(ProjectileRClass, FTransform(MuzzleRotation, MuzzleLocation));
			if (ProjectileR)
			{
				ProjectileR->DamageAmount = MagicianStat->GetRDamage() * MagicianStat->GetAttackCoefficient();
				// 陥製 覗傾績拭辞 硲窒鞠亀系 走尻吉 什肉 刃戟

				ProjectileR->FinishSpawning(FTransform(MuzzleRotation, MuzzleLocation));
				FVector LaunchDirection = FVector(0, 0, -1);
				ProjectileR->FireDirection(LaunchDirection);
			}
		}
	}
}
//void AMagicianCharacter::AttackRCheck()
//{
//	if (ProjectileRClass)
//	{
//		FVector ActorLocation = GetActorLocation();
//		FRotator ActorRotation = GetActorRotation();
//
//		FVector MuzzleLocation = ActorLocation + ActorRotation.Vector() * 2000;
//		MuzzleLocation.Z = 4000;
//
//		FRotator MuzzleRotation = FRotator(0, 0, 0);
//
//		UWorld* World = GetWorld();
//		if (World)
//		{
//			FActorSpawnParameters SpawnParams;
//			SpawnParams.Owner = this;
//			AMagicianRProjectile* ProjectileR = World->SpawnActor<AMagicianRProjectile>(ProjectileRClass, MuzzleLocation, MuzzleRotation, SpawnParams);
//			if (ProjectileR)
//			{
//				FVector LaunchDirection = FVector(0, 0, -1);
//				ProjectileR->FireDirection(LaunchDirection);
//				
//			}
//		}
//	}
//}
void AMagicianCharacter::OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	FName MontageName = Montage->GetFName();
	if (MontageName.ToString() == "MagicianDeathMontage")
	{
		OnDieMontageEnded();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Animation Montage ended: %s"), *MontageName.ToString());
		OnAttackMontageEnded();
	}
	/*if (MontageName.ToString() == "MagicianAttackNormalAnimMontage" || MontageName.ToString() == "MagicianAttackRightAnimMontage" || MontageName.ToString() == "MagicianAttackQAnimMontage" || MontageName.ToString() == "MagicianAttackEAnimMontage" || MontageName.ToString() == "MagicianAttackRAnimMontage")
	{
		OnAttackMontageEnded();
	}*/
	// 功展爽税 戚硯聖 稽益稽 窒径
}

void AMagicianCharacter::OnDieMontageEnded()
{
	if (MagicianStat->GetLife() > 0)
	{
		IsDeath = false;
		MagicianStat->SetHp(MagicianStat->GetMaxHp());
	}
	UE_LOG(LogTemp, Error, TEXT("zlakjdfklasdnkt"));
}


void AMagicianCharacter::OnAttackMontageEnded()
{
	IsAttacking = false;
}

void AMagicianCharacter::Teleport()
{
	/*
	if (ClientGameInstance)
	{
		ClientGameInstance->SetMonsterKillCount();
		ClientGameInstance->SetCurrentStage();
	}*/
	if (IsDeath)
	{
		return;
	}
	// Teleport 害精 悌展績 溌昔
	if (MagicianStat->IsSkillReady("Shift") == true)
	{
		UE_LOG(LogTemp, Warning, TEXT("Teleport Ready"));
		// 蝶遣斗税 薄仙 是帖
		FVector CurrentLocation = GetActorLocation();

		// 蝶遣斗亜 郊虞左澗 号狽 困斗
		FVector ForwardVector = GetActorForwardVector();

		// 授娃戚疑拝 是帖 (郊虞左澗 号狽生稽 析舛 暗軒 戚疑)
		FVector TeleportLocation = CurrentLocation + ForwardVector * 777.f;

		// 亀鐸 走繊拭辞 郊韓聖 達焼 是帖 繕舛
		FHitResult HitResult;
		FHitResult CheckResult;

		FVector StartTrace = TeleportLocation + FVector(0.f, 0.f, 777.f); // 鉦娃 是拭辞 闘傾戚縮 獣拙
		FVector EndTrace = TeleportLocation - FVector(0.f, 0.f, 777.f); // 鉦娃 焼掘稽 闘傾戚縮
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this); // 切奄 切重精 巷獣



		bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_Visibility, QueryParams);

		if (bHit)
		{
			// 郊韓聖 達精 井酔, 是帖 繕舛
			TeleportLocation = HitResult.Location + FVector(0.f, 0.f, 77.f);
			// 授娃戚疑拝 噺穿葵
			FRotator TeleportRotation = GetActorRotation();
			if (AnimInstance)
			{
				AnimInstance->PlayTeleportMontage();
				TeleportTo(TeleportLocation, TeleportRotation, false, true);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportParticleSystem, TeleportLocation, FRotator::ZeroRotator, true);
				USoundBase* ExplosionSound = LoadObject<USoundBase>(nullptr, TEXT("SoundWave'/Game/Sounds/Player/Magician/EffectSound/Teleport.Teleport'"));
				UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetOwner()->GetActorLocation());
			}
			// TeleportTo 敗呪 硲窒

			// 亀鐸 是帖拭辞 督銅適 獣什奴 什肉
			// Teleport 悌展績 段奄鉢
			MagicianStat->ActivateSkill("Shift");
		}

	}

	else
	{

	}


}
