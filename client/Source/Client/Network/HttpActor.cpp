// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpActor.h"
#include "Json.h"
#include "../ClientGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHttpActor::AHttpActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void AHttpActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHttpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FString AHttpActor::GetAccessToken()
{
	UClientGameInstance* ClientGameInstance = Cast<UClientGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (ClientGameInstance)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *(ClientGameInstance->GetAccessToken()))
		return ClientGameInstance->GetAccessToken();
	}
	else {
		return TEXT("");
	}
}

// 회원 가입
void AHttpActor::SignUp(FString Nickname, FString Password)
{
	UE_LOG(LogTemp, Log, TEXT("Request Sign Up"))

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	
	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("nickname", *Nickname);
	RequestObj->SetStringField("password", *Password);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ResponseSignUp);
	Request->SetURL("http://k10a208.p.ssafy.io:8080/api/signup");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

// 토큰 재발급
void AHttpActor::Reissue(FString RefreshToken, FString Nickname)
{
	UE_LOG(LogTemp, Log, TEXT("Request Reissue"))

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("refreshToken", *RefreshToken);
	RequestObj->SetStringField("nickname", *Nickname);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ResponseReissue);
	Request->SetURL("http://k10a208.p.ssafy.io:8080/api/reissue");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

// 로그인
void AHttpActor::SignIn(FString Nickname, FString Password)
{
	UE_LOG(LogTemp, Log, TEXT("Request Sign In"))

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("nickname", *Nickname);
	RequestObj->SetStringField("password", *Password);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ResponseSignIn);
	Request->SetURL("http://k10a208.p.ssafy.io:8080/api/login");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

// 닉네임 중복 확인
void AHttpActor::NicknameCheck(FString Nickname)
{
	UE_LOG(LogTemp, Log, TEXT("Request Nickname Check"));
	UE_LOG(LogTemp, Log, TEXT("NickName : %s"), *Nickname);

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ResponseNicknameCheck);
	Request->SetURL(*FString::Printf(TEXT("http://k10a208.p.ssafy.io:8080/api/nickname-check/%s"), *Nickname));
	UE_LOG(LogTemp, Log, TEXT("URL : %s"), *Request->GetURL());
	Request->SetVerb("GET");
	Request->ProcessRequest();
}

// 로그아웃
void AHttpActor::SignOut()
{
	UE_LOG(LogTemp, Log, TEXT("Request Sign Out"))

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ResponseSignOut);
	Request->SetURL("http://k10a208.p.ssafy.io:8080/api/logout");
	Request->SetVerb("GET");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("authorization", GetAccessToken());
	Request->ProcessRequest();
}

// 새로운 게임 생성
void AHttpActor::CreateNewGame(FString ModeString)
{
	UE_LOG(LogTemp, Log, TEXT("Request Create New Game"))

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetStringField("modeStr", *ModeString);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ResponseCreateNewGame);
	Request->SetURL("http://k10a208.p.ssafy.io:8080/api/game");
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("authorization", GetAccessToken());
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

// 게임 기록 저장
void AHttpActor::SaveGameRecord(int32 GameId, int32 Score)
{
	UE_LOG(LogTemp, Log, TEXT("Request Create New Game"))

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetNumberField("gameId", GameId);
	RequestObj->SetNumberField("score", Score);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ResponseSaveGameRecord);
	Request->SetURL("http://k10a208.p.ssafy.io:8080/api/game/save");
	Request->SetVerb("PATCH");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("authorization", GetAccessToken());
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

// 게임 중 나가기
void AHttpActor::GameExit(int32 GameId)
{
	UE_LOG(LogTemp, Log, TEXT("Request Game Exit"))

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	TSharedRef<FJsonObject> RequestObj = MakeShared<FJsonObject>();
	RequestObj->SetNumberField("gameId", GameId);

	FString RequestBody;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
	FJsonSerializer::Serialize(RequestObj, Writer);

	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ResponseGameExit);
	Request->SetURL("http://k10a208.p.ssafy.io:8080/api/game/exit");
	Request->SetVerb("PATCH");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetHeader("authorization", GetAccessToken());
	Request->SetContentAsString(RequestBody);
	Request->ProcessRequest();
}

// 게임 기록 조회
void AHttpActor::InquiryGameRecord(int32 MemberId, int32 PageNumber)
{
	UE_LOG(LogTemp, Log, TEXT("Request Inquiry Game Record"))

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ResponseInquiryGameRecord);
	Request->SetURL(*FString::Printf(TEXT("http://k10a208.p.ssafy.io:8080/api/game/rank/%d?pageNumber=%d"), MemberId, PageNumber));
	Request->SetVerb("GET");
	Request->SetHeader("authorization", GetAccessToken());
	Request->ProcessRequest();
}

// 랭킹 조회
void AHttpActor::InquiryRanking(FString GameMode, int32 PageNumber)
{
	UE_LOG(LogTemp, Log, TEXT("Request Inquiry Ranking"))

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::ResponseInquiryRanking);
	Request->SetURL(FString::Printf(TEXT("http://k10a208.p.ssafy.io:8080/api/game/rank/%s?pageNumber=%d"), *GameMode, PageNumber));
	Request->SetVerb("GET");
	Request->SetHeader("authorization", GetAccessToken());
	Request->ProcessRequest();
}

void AHttpActor::ResponseSignUp(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// 역직렬화
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	// 필요한 값 추출
	int32 StatusCode = ResponseObj->GetNumberField("status");
	FString Message = ResponseObj->GetStringField("message");
	int32 Id = ResponseObj->GetObjectField("result")
		->GetNumberField("id");
	FString Nickname = ResponseObj->GetObjectField("result")
		->GetStringField("nickname");
	
	OnCompleteSignUp.Broadcast(StatusCode, Message, Id, Nickname);
}

void AHttpActor::ResponseReissue(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// 역직렬화
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	// 필요한 값 추출
	int32 StatusCode = ResponseObj->GetNumberField("status");
	FString Message = ResponseObj->GetStringField("message");

	TSharedPtr<FJsonObject> Result = ResponseObj->GetObjectField("result");
	FString AccessToken = Result->GetStringField("access_token");
	FString RefreshToken = Result->GetStringField("refresh_token");
	FString Nickname = Result->GetStringField("nickname");
	int32 Id = FCString::Atoi(*(Result->GetStringField("id")));

	OnCompleteReissue.Broadcast(StatusCode, Message, AccessToken, RefreshToken, Nickname, Id);
}

void AHttpActor::ResponseSignIn(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	UE_LOG(LogTemp, Log, TEXT("Response %s"), *(Response->GetContentAsString()))
	// 역직렬화
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	// 필요한 값 추출
	int32 StatusCode = ResponseObj->GetNumberField("status");
	FString Message = ResponseObj->GetStringField("message");

	TSharedPtr<FJsonObject> User = ResponseObj->GetObjectField("result")->GetObjectField("user");
	FString AccessToken = User->GetStringField("access_token");
	FString RefreshToken = User->GetStringField("refresh_token");
	FString Nickname = User->GetStringField("nickname");
	int32 Id = FCString::Atoi(*(User->GetStringField("id")));

	OnCompleteSignIn.Broadcast(StatusCode, Message, AccessToken, RefreshToken, Nickname, Id);
}

void AHttpActor::ResponseNicknameCheck(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// 역직렬화
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	// 필요한 값 추출
	int32 StatusCode = ResponseObj->GetNumberField("status");
	FString Message = ResponseObj->GetStringField("message");
	bool Result = ResponseObj->GetBoolField("result");

	OnCompleteNicknameCheck.Broadcast(StatusCode, Message, Result);
}

void AHttpActor::ResponseSignOut(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// 역직렬화
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	// 필요한 값 추출
	int32 StatusCode = ResponseObj->GetNumberField("status");
	FString Message = ResponseObj->GetStringField("message");
	FString Result = ResponseObj->GetStringField("result");
	OnCompleteSignOut.Broadcast(StatusCode, Message, Result);
}

void AHttpActor::ResponseCreateNewGame(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// 역직렬화
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	// 필요한 값 추출
	int32 StatusCode = ResponseObj->GetNumberField("status");
	FString Message = ResponseObj->GetStringField("message");
	int32 GameId = ResponseObj->GetObjectField("result")
		->GetNumberField("gameId");

	OnCompleteCreateNewGame.Broadcast(StatusCode, Message, GameId);
}

void AHttpActor::ResponseSaveGameRecord(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// 역직렬화
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	// 필요한 값 추출
	int32 StatusCode = ResponseObj->GetNumberField("status");
	FString Message = ResponseObj->GetStringField("message");
	int32 GameId = ResponseObj->GetObjectField("result")
		->GetNumberField("gameId");
	int32 Score = ResponseObj->GetObjectField("result")
		->GetNumberField("score");

	OnCompleteSaveGameRecord.Broadcast(StatusCode, Message, GameId, Score);
}

void AHttpActor::ResponseGameExit(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// 역직렬화
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	// 필요한 값 추출
	int32 StatusCode = ResponseObj->GetNumberField("status");
	FString Message = ResponseObj->GetStringField("message");
	FString Result = ResponseObj->GetStringField("result");

	OnCompleteGameExit.Broadcast(StatusCode, Message, Result);
}

void AHttpActor::ResponseInquiryGameRecord(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// 역직렬화
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	// 필요한 값 추출
	int32 StatusCode = ResponseObj->GetNumberField("status");
	FString Message = ResponseObj->GetStringField("message");

	TSharedPtr<FJsonObject> Result = ResponseObj->GetObjectField("result");
	int32 GameCnt = Result->GetNumberField("gameCnt");
	int32 PageNumber = Result->GetNumberField("pageNumber");
	TArray<TSharedPtr<FJsonValue>> RawBestScore = Result->GetArrayField("bestScore");
	TArray<TSharedPtr<FJsonValue>> RawGameInfo = Result->GetArrayField("gameInfo");

	TArray<FGameplayData> BestScore, GameInfo;
	for (auto& Data : RawBestScore)
	{
		TSharedPtr<FJsonObject> DataObj = Data->AsObject();

		FGameplayData DataStruct;
		DataStruct.GameId = DataObj->GetNumberField("gameId");
		DataStruct.Mode = DataObj->GetStringField("mode");
		DataStruct.Score = DataObj->GetNumberField("score");
		DataStruct.StartDate = DataObj->GetStringField("startDate");
		DataStruct.Playtime = DataObj->GetNumberField("playtime");
		DataStruct.MemberId = 0;
		DataStruct.Nickname = TEXT("");

		BestScore.Add(DataStruct);
	}

	for (auto& Data : RawGameInfo)
	{
		TSharedPtr<FJsonObject> DataObj = Data->AsObject();

		FGameplayData DataStruct;
		DataStruct.GameId = DataObj->GetNumberField("gameId");
		DataStruct.Mode = DataObj->GetStringField("mode");
		DataStruct.Score = DataObj->GetNumberField("score");
		DataStruct.StartDate = DataObj->GetStringField("startDate");
		DataStruct.Playtime = DataObj->GetNumberField("playtime");
		DataStruct.MemberId = 0;
		DataStruct.Nickname = TEXT("");

		GameInfo.Add(DataStruct);
	}

	OnCompleteInquiryGameRecord.Broadcast(StatusCode, Message, GameCnt, PageNumber, BestScore, GameInfo);
}

void AHttpActor::ResponseInquiryRanking(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// 역직렬화
	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	// 필요한 값 추출
	int32 StatusCode = ResponseObj->GetNumberField("status");
	FString Message = ResponseObj->GetStringField("message");

	TSharedPtr<FJsonObject> Result = ResponseObj->GetObjectField("result");
	int32 GameCnt = Result->GetNumberField("gameCnt");
	int32 PageNumber = Result->GetNumberField("pageNumber");
	TArray<TSharedPtr<FJsonValue>> RawBestScore = Result->GetArrayField("bestScore");
	TArray<TSharedPtr<FJsonValue>> RawGameInfo = Result->GetArrayField("gameInfo");

	TArray<FGameplayData> BestScore, GameInfo;
	for (auto& Data : RawBestScore)
	{
		TSharedPtr<FJsonObject> DataObj = Data->AsObject();

		FGameplayData DataStruct;
		DataStruct.GameId = DataObj->GetNumberField("gameId");
		DataStruct.Mode = DataObj->GetStringField("mode");
		DataStruct.Score = DataObj->GetNumberField("score");
		DataStruct.StartDate = DataObj->GetStringField("startDate");
		DataStruct.Playtime = DataObj->GetNumberField("playtime");
		DataStruct.MemberId = DataObj->GetNumberField("memberId");
		DataStruct.Nickname = DataObj->GetStringField("nickname");

		BestScore.Add(DataStruct);
	}

	for (auto& Data : RawGameInfo)
	{
		TSharedPtr<FJsonObject> DataObj = Data->AsObject();

		FGameplayData DataStruct;
		DataStruct.GameId = DataObj->GetNumberField("gameId");
		DataStruct.Mode = DataObj->GetStringField("mode");
		DataStruct.Score = DataObj->GetNumberField("score");
		DataStruct.StartDate = DataObj->GetStringField("startDate");
		DataStruct.Playtime = DataObj->GetNumberField("playtime");
		DataStruct.MemberId = DataObj->GetNumberField("memberId");
		DataStruct.Nickname = DataObj->GetStringField("nickname");

		GameInfo.Add(DataStruct);
	}

	OnCompleteInquiryRanking.Broadcast(StatusCode, Message, GameCnt, PageNumber, BestScore, GameInfo);
}
