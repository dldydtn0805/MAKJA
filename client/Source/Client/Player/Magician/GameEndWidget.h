// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "GameEndWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UGameEndWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ExitRoom();
	// HP ����
	void BindRankingList();
	void UpdateRankingList(int32 StatusCode, FString Message, int32 GameCnt, int32 PageNumber, TArray<struct FGameplayData> BestScore, TArray<struct FGameplayData> GameInfo);
	void PostRankingListResponse(int32 StatusCode, FString Message, int32 GameId, int32 Score);
	void GetRankingList();

	void PostRankingList();

	class AHttpActor* HttpActor;
protected:
	// ���� ���� �� ȣ��Ǵ� �Լ�
	// virtual void NativeConstruct() override;

public:
	class UMagicianStatComponent* CurrentStatComp;
	class UClientGameInstance* ClientGameInstance;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UScrollBox* RankingList;

	// ������ ��ư
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ExitBtn;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TextWidget;


	UFUNCTION()
	void OnExitBtnClicked();

	//UPROPERTY(EditAnywhere, meta = (BindWidget))
	//UTextBlock* RankingOne;
	//UPROPERTY(EditAnywhere, meta = (BindWidget))
	//UTextBlock* RankingTwo;
	//UPROPERTY(EditAnywhere, meta = (BindWidget))
	//UTextBlock* RankingThree;
	//UPROPERTY(EditAnywhere, meta = (BindWidget))
	//UTextBlock* RankingFour;
	//UPROPERTY(EditAnywhere, meta = (BindWidget))
	//UTextBlock* RankingFive;
	//UPROPERTY(EditAnywhere, meta = (BindWidget))
	//UTextBlock* RankingSix;

};
