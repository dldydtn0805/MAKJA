// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEndWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "../../Network/HttpActor.h"
#include "../../ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"


void UGameEndWidget::BindRankingList()
{
    ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
    HttpActor = GetWorld()->SpawnActor<AHttpActor>(AHttpActor::StaticClass());
    if (HttpActor)
    {
        HttpActor->OnCompleteInquiryRanking.AddUObject(this, &UGameEndWidget::UpdateRankingList);
        HttpActor->OnCompleteSaveGameRecord.AddUObject(this, &UGameEndWidget::PostRankingListResponse);
    }

    ExitBtn->OnClicked.AddDynamic(this, &UGameEndWidget::OnExitBtnClicked);
}

//int32, FString, int32, int32, TArray<FGameplayData>, TArray<FGameplayData>
void UGameEndWidget::UpdateRankingList(int32 StatusCode, FString Message, int32 GameCnt, int32 PageNumber, TArray<FGameplayData> BestScore, TArray<FGameplayData> GameInfo)
{
    // StatusCode 출력
    UE_LOG(LogTemp, Log, TEXT("StatusCode: %d"), StatusCode);
    // Message 출력
    UE_LOG(LogTemp, Log, TEXT("Message: %s"), *Message);
    // GameCnt 출력
    UE_LOG(LogTemp, Log, TEXT("GameCnt: %d"), GameCnt);
    // PageNumber 출력
    UE_LOG(LogTemp, Log, TEXT("PageNumber: %d"), PageNumber);

    int32 Index = 0;
    for (const FGameplayData& Data : GameInfo)
    {
        Index++;

        if (RankingList)
        {
            // UHorizontalBox 생성
            UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(this);
            RankingList->AddChild(HorizontalBox);

            // 순위 접미사 설정
            FString SuffixString;
            switch (Index % 10)
            {
            case 1:
                SuffixString = TEXT("st");
                break;
            case 2:
                SuffixString = TEXT("nd");
                break;
            case 3:
                SuffixString = TEXT("rd");
                break;
            default:
                SuffixString = TEXT("th");
                break;
            }

            if (Index >= 11 && Index <= 13)
            {
                SuffixString = TEXT("th");
            }

            // 닉네임 텍스트 블록 생성 및 HorizontalBox에 추가
            UTextBlock* NicknameText = NewObject<UTextBlock>(this);
            // 닉네임을 텍스트로 설정
            NicknameText->SetText(FText::FromString(FString::Printf(TEXT("%d%s %s"), Index, *SuffixString, *Data.Nickname)));
            UHorizontalBoxSlot* NicknameSlot = HorizontalBox->AddChildToHorizontalBox(NicknameText);
            NicknameSlot->SetHorizontalAlignment(HAlign_Left);
            NicknameSlot->SetPadding(FMargin(10, 0));  // 왼쪽 패딩 10, 상하 패딩 0
            NicknameSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

            // 점수 텍스트 블록 생성 및 HorizontalBox에 추가
            UTextBlock* ScoreText = NewObject<UTextBlock>(this);
            ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Data.Score)));
            UHorizontalBoxSlot* ScoreSlot = HorizontalBox->AddChildToHorizontalBox(ScoreText);
            ScoreSlot->SetHorizontalAlignment(HAlign_Right);
            ScoreSlot->SetPadding(FMargin(10, 0));  // 오른쪽 패딩 10, 상하 패딩 0
            ScoreSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

            // HorizontalBox 설정
            HorizontalBox->SetVisibility(ESlateVisibility::Visible);
        }

        // 로그 출력
        UE_LOG(LogTemp, Log, TEXT("GameInfo : %s = %d"), *Data.Nickname, Data.Score);
    }

}


void UGameEndWidget::GetRankingList()
{
    // 겜 끝났을 때 호출 하면 
    if (HttpActor)
    {
        HttpActor->InquiryRanking(TEXT("Easy"), 0);
    }
}

void UGameEndWidget::PostRankingList()
{
    if (HttpActor && ClientGameInstance)
    {
        int32 MonsterCount = ClientGameInstance->GetMonsterKillCount();
        int32 Life = ClientGameInstance->GetLife();
        int32 Money = ClientGameInstance->GetMoney();
        int32 CurrentStage = ClientGameInstance->GetCurrentStage();
        int32 Score;
        Score = int32((CurrentStage - 1) * 1000 + MonsterCount * 100 + (Life * 10000) / 3 + (Money) / 500);

        UE_LOG(LogTemp, Log, TEXT("Score : %d"), Score);
        HttpActor->SaveGameRecord(ClientGameInstance->GetGameId(), Score);
    }
}

void UGameEndWidget::PostRankingListResponse(int32 StatusCode, FString Message, int32 GameId, int32 Score)
{
    if (HttpActor)
    {
        UE_LOG(LogTemp, Log, TEXT("a;ldkfjakldfjlkasdfjlad"));
        HttpActor->InquiryRanking(TEXT("Easy"), 0);
    }
}

void UGameEndWidget::OnExitBtnClicked()
{
    UE_LOG(LogTemp, Log, TEXT("EXIT"));
    if (ExitBtn)
    {
        UGameplayStatics::OpenLevel(GetWorld(), TEXT("Start"));
    }
}
