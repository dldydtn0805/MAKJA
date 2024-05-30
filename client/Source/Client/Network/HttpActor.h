// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "HttpActor.generated.h"

// 게임 플레이 데이터
USTRUCT()
struct FGameplayData {
	GENERATED_BODY()

	int32 GameId; // 게임 아이디
	FString Mode; // 게임 모드 (난이도)
	int32 Score; // 점수
	FString StartDate; // 게임 시작 시각
	int32 Playtime; // 플레이 타임
	int32 MemberId; // 멤버 아이디
	FString Nickname; // 닉네임
};

DECLARE_MULTICAST_DELEGATE_FourParams(FOnCompleteSignUp, int32, FString, int32, FString)
DECLARE_MULTICAST_DELEGATE_SixParams(FOnCompleteReissue, int32, FString, FString, FString, FString, int32)
DECLARE_MULTICAST_DELEGATE_SixParams(FOnCompleteSignIn, int32, FString, FString, FString, FString, int32)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnCompleteNicknameCheck, int32, FString, bool)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnCompleteSignOut, int32, FString, FString)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnCompleteCreateNewGame, int32, FString, int32)
DECLARE_MULTICAST_DELEGATE_FourParams(FOnCompleteSaveGameRecord, int32, FString, int32, int32)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnCompleteGameExit, int32, FString, FString)
DECLARE_MULTICAST_DELEGATE_SixParams(FOnCompleteInquiryGameRecord, int32, FString, int32, int32, TArray<FGameplayData>, TArray<FGameplayData>)
DECLARE_MULTICAST_DELEGATE_SixParams(FOnCompleteInquiryRanking, int32, FString, int32, int32, TArray<FGameplayData>, TArray<FGameplayData>)
UCLASS()
class CLIENT_API AHttpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHttpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FString GetAccessToken();

private:
	// 응답을 받았을 때 실행할 콜백 함수들
	void ResponseSignUp(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void ResponseReissue(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void ResponseSignIn(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void ResponseNicknameCheck(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void ResponseSignOut(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void ResponseCreateNewGame(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void ResponseSaveGameRecord(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void ResponseGameExit(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void ResponseInquiryGameRecord(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	void ResponseInquiryRanking(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
	
public:
	// API Request를 보낼 함수 들
	void SignUp(FString Nickname, FString Password); // 회원 가입
	void Reissue(FString RefreshToken, FString Nickname); // 토큰 재발급
	void SignIn(FString Nickname, FString Password); // 로그인
	void NicknameCheck(FString Nickname); // 닉네임 중복 확인
	void SignOut(); // 로그아웃
	void CreateNewGame(FString ModeString); // 새로운 게임 생성
	void SaveGameRecord(int32 GameId, int32 Score); // 게임 기록 저장
	void GameExit(int32 GameId); // 게임 중 나가기
	void InquiryGameRecord(int32 MemberId, int32 PageNumber); // 게임 기록 조회
	void InquiryRanking(FString GameMode, int32 PageNumber); // 랭킹 조회

private:
	const FString TestAccessToken = TEXT("");

public:
	// 외부에서 사용할 Delegate
	FOnCompleteSignUp OnCompleteSignUp;
	FOnCompleteReissue OnCompleteReissue;
	FOnCompleteSignIn OnCompleteSignIn;
	FOnCompleteNicknameCheck OnCompleteNicknameCheck;
	FOnCompleteSignOut OnCompleteSignOut;
	FOnCompleteCreateNewGame OnCompleteCreateNewGame;
	FOnCompleteSaveGameRecord OnCompleteSaveGameRecord;
	FOnCompleteGameExit OnCompleteGameExit;
	FOnCompleteInquiryGameRecord OnCompleteInquiryGameRecord;
	FOnCompleteInquiryRanking OnCompleteInquiryRanking;
};
