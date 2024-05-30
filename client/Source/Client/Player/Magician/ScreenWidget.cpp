// Fill out your copyright notice in the Description page of Project Settings.


#include "ScreenWidget.h"
#include "MagicianStatComponent.h"
#include "../../ClientGameInstance.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CurrentGameInst = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (CurrentGameInst)
	{
		UE_LOG(LogTemp, Log, TEXT("Current Game Instance Exists!"));
	}
	PlayerLevelUpBtn->OnClicked.AddDynamic(this, &UScreenWidget::OnPlayerLevelUpBtnClicked);
	WeaponLevelUpBtn->OnClicked.AddDynamic(this, &UScreenWidget::OnWeaponLevelUpBtnClicked);
	}

// Player Hp
void UScreenWidget::BindHp(class UMagicianStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	StatComp->OnHpChanged.AddUObject(this, &UScreenWidget::UpdateHp);
}

void UScreenWidget::UpdateHp()
{
	if (CurrentStatComp->IsValidLowLevel())
	{
		HpBar->SetPercent(CurrentStatComp->GetHpRatio());

		FString HpNowText = FString::Printf(TEXT("%d"), CurrentStatComp->GetHp());
		FString HpMaxText = FString::Printf(TEXT("%d"), CurrentStatComp->GetMaxHp());
		HpNow->SetText(FText::FromString(HpNowText));
		HpMax->SetText(FText::FromString(HpMaxText));
	}
}



// PlayerLevel
void UScreenWidget::BindPlayerLevel(class UMagicianStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	StatComp->OnPlayerLevelChanged.AddUObject(this, &UScreenWidget::UpdatePlayerLevel);
}

void UScreenWidget::UpdatePlayerLevel()
{
	if (CurrentStatComp->IsValidLowLevel())
		// UE_LOG(LogTemp, Log, TEXT("level up clicked"));
	{
		FString LevelText = FString::Printf(TEXT("%d"), CurrentStatComp->GetLevel());
		PlayerLevel->SetText(FText::FromString(LevelText));
		FString LevelUpCostText = FString::Printf(TEXT("%d"), CurrentStatComp->GetLevelUpCost());
		LevelUpCost->SetText(FText::FromString(LevelUpCostText));
		UE_LOG(LogTemp, Log, TEXT("New CoolTime : %d"), int(CurrentStatComp->GetRightCoolTime()));
		FString NewRightCoolTime = FString::Printf(TEXT("Right(%d)"), int(CurrentStatComp->GetRightCoolTime()));
		RightCoolTime->SetText(FText::FromString(NewRightCoolTime));
		FString NewQCoolTime = FString::Printf(TEXT("Q(%d)"), int(CurrentStatComp->GetQCoolTime()));
		QCoolTime->SetText(FText::FromString(NewQCoolTime));
		FString NewECoolTime = FString::Printf(TEXT("E(%d)"), int(CurrentStatComp->GetECoolTime()));
		ECoolTime->SetText(FText::FromString(NewECoolTime));
		FString NewRCoolTime = FString::Printf(TEXT("R(%d)"), int(CurrentStatComp->GetRCoolTime()));
		RCoolTime->SetText(FText::FromString(NewRCoolTime));
	}
}

// WeaponLevel
void UScreenWidget::BindWeaponLevel(class UMagicianStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	StatComp->OnWeaponLevelChanged.AddUObject(this, &UScreenWidget::UpdateWeaponLevel);
}

void UScreenWidget::UpdateWeaponLevel()
{
	if (CurrentStatComp->IsValidLowLevel())
	{
		FString LevelText = FString::Printf(TEXT("%d"), CurrentStatComp->GetWeaponLevel());
		WeaponLevel->SetText(FText::FromString(LevelText));
		FString WeaponLevelUpCostText = FString::Printf(TEXT("%d"), CurrentStatComp->GetWeaponLevelUpCost());
		WeaponLevelUpCost->SetText(FText::FromString(WeaponLevelUpCostText));
		FString WeaponLevelUpPercentageText = FString::Printf(TEXT("%d%%"), FMath::TruncToInt((CurrentStatComp->GetWeaponLevelUpPercentage()) * 100));
		WeaponLevelUpPercentage->SetText(FText::FromString(WeaponLevelUpPercentageText));

	}
}

void UScreenWidget::BindMoney(UMagicianStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	StatComp->OnMoneyChanged.AddUObject(this, &UScreenWidget::UpdateMoney);
}


void UScreenWidget::UpdateMoney()
{
	if (CurrentStatComp->IsValidLowLevel())
	{
		FString MoneyText = FString::Printf(TEXT("%d"), CurrentStatComp->GetMoney());
		Money->SetText(FText::FromString(MoneyText));
	}
	if (CurrentGameInst)
	{
		CurrentGameInst->SetMoney(CurrentStatComp->GetMoney());
	}
}

void UScreenWidget::BindLife(UMagicianStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
    StatComp->OnLifeChanged.AddUObject(this, &UScreenWidget::UpdateLife);
}


void UScreenWidget::UpdateLife()
{
	if (CurrentStatComp->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("UpdateLife : %d"), CurrentStatComp->GetLife());
		FString LifeText = FString::Printf(TEXT("%d"), CurrentStatComp->GetLife());
		Life->SetText(FText::FromString(LifeText));
	}
	if (CurrentGameInst)
	{
		CurrentGameInst->SetLife(CurrentStatComp->GetLife());
	}
}

void UScreenWidget::BindMonsterKillCount(int32 CurrentEnemy, int32 TotalEnemy)
{
	FString MonsterKillCountText = FString::Printf(TEXT("%i / %i"), CurrentEnemy, TotalEnemy);
	MonsterKillCount->SetText(FText::FromString(MonsterKillCountText));
}

void UScreenWidget::BindCurrentStage(int32 CurrentWave)
{
	FString CurrentStageText = FString::Printf(TEXT("%d"), CurrentWave);
	CurrentStage->SetText(FText::FromString(CurrentStageText));
}


// Right CoolTime 
void UScreenWidget::BindRightCoolTime(UMagicianStatComponent* StatComp)
{

	CurrentStatComp = StatComp;
	StatComp->OnRightCoolTimeChanged.AddUObject(this, &UScreenWidget::UpdateRightCoolTime);

}

// 플레이어 레벨업 시 스킬 쿨타임 갱신
void UScreenWidget::UpdateRightCoolTime(float RemainingTime)
{
	if (CurrentStatComp->IsValidLowLevel())
	{
		int Time = int(RemainingTime);
		if (Time == 0)
		{
			RightBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.1f));
			RightImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			RightBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f)); // 회색
			// RightImage->SetOpacity(0.7f);
			RightImage->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));

		}
		// FString CoolTimeText = FString::Printf(TEXT("%d"), int(RemainingTime));
		// RightCoolTime->SetText(FText::FromString(CoolTimeText));
	}
}

// Shift CoolTime 
void UScreenWidget::BindShiftCoolTime(UMagicianStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	StatComp->OnShiftCoolTimeChanged.AddUObject(this, &UScreenWidget::UpdateShiftCoolTime);

}

void UScreenWidget::UpdateShiftCoolTime(float RemainingTime)
{
	if (CurrentStatComp->IsValidLowLevel())
	{
		int Time = int(RemainingTime);
		if (Time == 0)
		{
			RightBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.1f));
			RightImage->SetOpacity(1.0f);
		}
		else
		{
			RightBorder->SetBrushColor(FLinearColor(0.5f, 0.5f, 0.5f, 1.0f)); // 회색
			RightImage->SetOpacity(0.5f);
		}
		// FString CoolTimeText = FString::Printf(TEXT("%d"), int(RemainingTime));
		// RightCoolTime->SetText(FText::FromString(CoolTimeText));
	}
}

// Q CoolTime 
void UScreenWidget::BindQCoolTime(UMagicianStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	StatComp->OnQCoolTimeChanged.AddUObject(this, &UScreenWidget::UpdateQCoolTime);

}

void UScreenWidget::UpdateQCoolTime(float RemainingTime)
{
	if (CurrentStatComp->IsValidLowLevel())
	{
		int Time = int(RemainingTime);
		if (Time == 0)
		{
			QBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.1f));
			QImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			QBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f)); // 회색
			QImage->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));

		}
	}
}

// E CoolTime
void UScreenWidget::BindECoolTime(UMagicianStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	StatComp->OnECoolTimeChanged.AddUObject(this, &UScreenWidget::UpdateECoolTime);

}

void UScreenWidget::UpdateECoolTime(float RemainingTime)
{
	if (CurrentStatComp->IsValidLowLevel())
	{
		int Time = int(RemainingTime);
		if (Time == 0)
		{
			EBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.1f));
			EImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			EBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f)); // 회색
			EImage->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
		}
		// FString CoolTimeText = FString::Printf(TEXT("%d"), int(RemainingTime));
		// RightCoolTime->SetText(FText::FromString(CoolTimeText));
	}
}

// R CoolTime
void UScreenWidget::BindRCoolTime(UMagicianStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	StatComp->OnRCoolTimeChanged.AddUObject(this, &UScreenWidget::UpdateRCoolTime);

}

void UScreenWidget::UpdateRCoolTime(float RemainingTime)
{
	if (CurrentStatComp->IsValidLowLevel())
	{
		int Time = int(RemainingTime);
		if (Time == 0)
		{
			RBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.1f));
			RImage->SetColorAndOpacity(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{
			RBorder->SetBrushColor(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f)); // 회색
			RImage->SetColorAndOpacity(FLinearColor(0.1f, 0.1f, 0.1f, 1.0f));
		}
		// FString CoolTimeText = FString::Printf(TEXT("%d"), int(RemainingTime));
		// RightCoolTime->SetText(FText::FromString(CoolTimeText));
	}
}


//void UScreenWidget::SetupInputComponent()
//{
//	InputComponent->BindAction("LevelUp", IE_Pressed, this, &UScreenWidget::OnPlayerLevelUpBtnClicked);
//}


void UScreenWidget::OnPlayerLevelUpBtnClicked()
{
	// LevelUpBtn이 클릭되었을 때 실행되는 코드
	// PlayerLevel을 올리는 등의 작업을 수행할 수 있음
	if (CurrentStatComp && CurrentStatComp->IsValidLowLevel())
	{
		// 예시: 플레이어 레벨을 올리고, 이를 적용하기 위해 StatComp의 함수 호출
		CurrentStatComp->SetLevel();

		

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("level up clicked failed"));

	}
}

void UScreenWidget::OnWeaponLevelUpBtnClicked()
{
	// LevelUpBtn이 클릭되었을 때 실행되는 코드
	// PlayerLevel을 올리는 등의 작업을 수행할 수 있음
	if (CurrentStatComp && CurrentStatComp->IsValidLowLevel())
	{
		// 예시: 플레이어 레벨을 올리고, 이를 적용하기 위해 StatComp의 함수 호출
		CurrentStatComp->SetWeaponLevelUp();
		UE_LOG(LogTemp, Log, TEXT("Weapon up clicked"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Weapon up clicked failed"));

	}
}
