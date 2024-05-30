// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Http.h"
#include "HttpActor.generated.h"

// ���� �÷��� ������
USTRUCT()
struct FGameplayData {
	GENERATED_BODY()

	int32 GameId; // ���� ���̵�
	FString Mode; // ���� ��� (���̵�)
	int32 Score; // ����
	FString StartDate; // ���� ���� �ð�
	int32 Playtime; // �÷��� Ÿ��
	int32 MemberId; // ��� ���̵�
	FString Nickname; // �г���
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
	// ������ �޾��� �� ������ �ݹ� �Լ���
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
	// API Request�� ���� �Լ� ��
	void SignUp(FString Nickname, FString Password); // ȸ�� ����
	void Reissue(FString RefreshToken, FString Nickname); // ��ū ��߱�
	void SignIn(FString Nickname, FString Password); // �α���
	void NicknameCheck(FString Nickname); // �г��� �ߺ� Ȯ��
	void SignOut(); // �α׾ƿ�
	void CreateNewGame(FString ModeString); // ���ο� ���� ����
	void SaveGameRecord(int32 GameId, int32 Score); // ���� ��� ����
	void GameExit(int32 GameId); // ���� �� ������
	void InquiryGameRecord(int32 MemberId, int32 PageNumber); // ���� ��� ��ȸ
	void InquiryRanking(FString GameMode, int32 PageNumber); // ��ŷ ��ȸ

private:
	const FString TestAccessToken = TEXT("");

public:
	// �ܺο��� ����� Delegate
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
