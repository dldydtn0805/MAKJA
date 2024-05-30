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
    // StatusCode ���
    UE_LOG(LogTemp, Log, TEXT("StatusCode: %d"), StatusCode);
    // Message ���
    UE_LOG(LogTemp, Log, TEXT("Message: %s"), *Message);
    // GameCnt ���
    UE_LOG(LogTemp, Log, TEXT("GameCnt: %d"), GameCnt);
    // PageNumber ���
    UE_LOG(LogTemp, Log, TEXT("PageNumber: %d"), PageNumber);

    int32 Index = 0;
    for (const FGameplayData& Data : GameInfo)
    {
        Index++;

        if (RankingList)
        {
            // UHorizontalBox ����
            UHorizontalBox* HorizontalBox = NewObject<UHorizontalBox>(this);
            RankingList->AddChild(HorizontalBox);

            // ���� ���̻� ����
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

            // �г��� �ؽ�Ʈ ��� ���� �� HorizontalBox�� �߰�
            UTextBlock* NicknameText = NewObject<UTextBlock>(this);
            // �г����� �ؽ�Ʈ�� ����
            NicknameText->SetText(FText::FromString(FString::Printf(TEXT("%d%s %s"), Index, *SuffixString, *Data.Nickname)));
            UHorizontalBoxSlot* NicknameSlot = HorizontalBox->AddChildToHorizontalBox(NicknameText);
            NicknameSlot->SetHorizontalAlignment(HAlign_Left);
            NicknameSlot->SetPadding(FMargin(10, 0));  // ���� �е� 10, ���� �е� 0
            NicknameSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

            // ���� �ؽ�Ʈ ��� ���� �� HorizontalBox�� �߰�
            UTextBlock* ScoreText = NewObject<UTextBlock>(this);
            ScoreText->SetText(FText::FromString(FString::Printf(TEXT("%d"), Data.Score)));
            UHorizontalBoxSlot* ScoreSlot = HorizontalBox->AddChildToHorizontalBox(ScoreText);
            ScoreSlot->SetHorizontalAlignment(HAlign_Right);
            ScoreSlot->SetPadding(FMargin(10, 0));  // ������ �е� 10, ���� �е� 0
            ScoreSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

            // HorizontalBox ����
            HorizontalBox->SetVisibility(ESlateVisibility::Visible);
        }

        // �α� ���
        UE_LOG(LogTemp, Log, TEXT("GameInfo : %s = %d"), *Data.Nickname, Data.Score);
    }

}


void UGameEndWidget::GetRankingList()
{
    // �� ������ �� ȣ�� �ϸ� 
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
