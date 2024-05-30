// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ScreenWidget.h"
#include "MagicianCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLifeStatusChanged);

UCLASS(config = Game)
class CLIENT_API AMagicianCharacter : public ACharacter
{
	GENERATED_BODY()
		/** Camera boom positioning the camera behind the character */
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FPSCamera;

public:
	// Sets default values for this character's properties
	AMagicianCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	void Die();
	void AttackRight();
	void AttackQ();
	void AttackE();
	void AttackR();

	UFUNCTION()
		void WeaponLevelUp();

	UFUNCTION()
		void LevelUp();

	// 기존 OnAttackMontageEnded(); 자리에 추가
	UFUNCTION()
		void OnAnimMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
		void OnDieMontageEnded();
	UFUNCTION()
		void OnAttackMontageEnded();

	void ExitRoom();

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* Weapon;

	class UNiagaraComponent* WeaponEffect;

	class AHttpActor* HttpActor;

	// 카메라 위치에서의 총구 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	// 스폰시킬 프로젝타일 클래스
	UPROPERTY(EditDefaultsOnly, Category = ProjectileQ)
		TSubclassOf<class AMagicianQProjectile> ProjectileQClass;

	// 스폰시킬 프로젝타일 클래스
	UPROPERTY(EditDefaultsOnly, Category = ProjectileE)
		TSubclassOf<class AMagicianEProjectile> ProjectileEClass;

	// 스폰시킬 프로젝타일 클래스
	UPROPERTY(EditDefaultsOnly, Category = ProjectileR)
		TSubclassOf<class AMagicianRProjectile> ProjectileRClass;

	// 스폰시킬 프로젝타일 클래스
	UPROPERTY(EditDefaultsOnly, Category = ProjectileRight)
		TSubclassOf<class AMagicianRightProjectile> ProjectileRightClass;

	UPROPERTY()
		int32 AttackQIndex = 0;

	UPROPERTY()
		int32 AttackLeftIndex = 0;

	UPROPERTY()
		int32 AttackRightIndex = 0;

	UPROPERTY()
		bool IsAttacking = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDeath = false;


	UPROPERTY(VisibleAnywhere)
		class UMagicianAnimInstance* AnimInstance;

	void AttackQCheck();
	void AttackECheck();
	void AttackRCheck();
	void AttackRightCheck();
	void AttackLeftCheck();

	UFUNCTION()
		void WeaponEffectChange();

	UFUNCTION()
		void Teleport();

	UFUNCTION()
		void ViewChange();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
		bool bIsFirstPersonView = false;

	UPROPERTY(EditAnywhere, Category = "Teleport Particle")
		class UParticleSystem* TeleportParticleSystem;

	UPROPERTY(EditAnywhere, Category = "LevelUp Particle")
		class UParticleSystem* LevelUpParticleSystem;

public:
	void AttackLeft();
	bool ChangeMoney(int32 Money);
	void CallGameEndWidget();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	FORCEINLINE class UCameraComponent* GetFPSCamera() const { return FPSCamera; }

	UPROPERTY(VisibleAnywhere)
		class UMagicianStatComponent* MagicianStat;

	class UClientGameInstance* ClientGameInstance;

	// WBP_Screen 변수 선언
	UPROPERTY(BlueprintReadOnly)
		class UUserWidget* ScreenWidget;

	UPROPERTY(BlueprintReadOnly)
		class UUserWidget* WeaponLevelUpSuccessWidget;

	UPROPERTY(BlueprintReadOnly)
		class UUserWidget* WeaponLevelUpFailWidget;

	UPROPERTY(BlueprintReadOnly)
		class UUserWidget* WeaponLevelUpBigFailWidget;

	UPROPERTY(BlueprintReadOnly)
		class UUserWidget* LevelUpSuccessWidget;

	UPROPERTY(BlueprintReadOnly)
		class UGameEndWidget* GameEndWidget;

	UPROPERTY(BlueprintAssignable)
		FOnLifeStatusChanged OnLifeStatusChanged;
};
