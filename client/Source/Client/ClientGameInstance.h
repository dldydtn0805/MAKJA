// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"

#include "EnumTypes.h"
#include "Enemy/Enemy.h"
#include "Enemy/EnemyAnimInstance.h"

#include "ClientGameInstance.generated.h"
/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnMonsterKillCountChanged);
DECLARE_MULTICAST_DELEGATE(FOnCurrentStageChanged);

// 1. 캐릭터 스탯 데이터
USTRUCT()
struct FCharacterStatData : public FTableRowBase
{
	GENERATED_BODY()

	// 플레이어 레벨
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;								
	// 플레이어 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHp;

	// 현재 Level에서 다음 Level로 레벨업 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LevelUpAvailable;

	// 레벨업을 위해 드는 가격
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelUpCost;

};


// 2. 캐릭터 스킬 관련 데이터
USTRUCT()
struct FCharacterSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level; // 플레이어 레벨

	// 좌클릭
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LeftDamage;								// 플레이어 Left 스킬 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LeftCoolTime;								// 플레이어 Left 스킬 쿨타임
	
	// 우클릭
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RightDamage;								// 플레이어 Left 스킬 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RightCoolTime;							// 플레이어 Left 스킬 쿨타임
	
	// Shift 키 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShiftDamage;								// 플레이어 Shift 스킬 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShiftCoolTime;							// 플레이어 Shift 스킬 쿨타임

	// Q 키
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float QDamage;									// 플레이어 Q 스킬 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float QCoolTime;								// 플레이어 Q 스킬 쿨타임

	// E 키
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EDamage;									// 플레이어 E 스킬 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ECoolTime;								// 플레이어 E 스킬 쿨타임
	
	// R 키
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RDamage;									// 플레이어 R 스킬 데미지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RCoolTime;								// 플레이어 R 스킬 쿨타임
	
};

// 2. 캐릭터 무기 관련 데이터
USTRUCT()
struct FCharacterWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponLevel;									// 현재 무기 레벨
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoefficient;							// 현재 무기 공격력 계수
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WeaponLevelUpAvailable;						// 다음 무기 레벨업에 가능 여부

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponLevelUpCost;							// 다음 무기 레벨업에 필요한 돈

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponLevelUpPercentage;						// 다음 무기 레벨업 성공 확률
};

// 3. Enemy 스탯 데이터

USTRUCT()
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Enemy ID"))
	int32 EnemyID; // 고유 ID

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Enemy KR Name"))
	FString EnemyKrName; // Monster name(KR)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Enemy EN Name"))
	FString EnemyEnName; // Monster name(EN)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EnemyType type; // Monster type

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemy> BP_Enemy; // Enemy Skeletal Mesh Blueprint

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HpRatio; // Hp Ratio in float

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ARRatio; // AD Resist Ratio in float

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MRRatio; // Magic Resist Ratio in float

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed; // Speed in float

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DmgRatio; // DMG Ratio in float

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRange; // Attack range in float

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeed; // Attack speed in float
};

// 4. Tower 스텟 데이터
USTRUCT()
struct FTowerStat : public FTableRowBase
{
	GENERATED_BODY()

	// 레벨
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Level;

	// 공격력
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Attack;

	// 공격 범위
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 AttackRange;

	// 공격 속도
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float AttackSpeed;

	// 비용
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Cost;

	// 안정치
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Stable;
};

// 5. Wave Enemy 데이터
USTRUCT()
struct FSpawnData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnPoint;
};


USTRUCT()
struct FWaveData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSpawnData> SpawnData;
};

UCLASS()
class CLIENT_API UClientGameInstance : public UGameInstance
{
	GENERATED_BODY()


public:
	UClientGameInstance();

	virtual void Init() override;

	// 캐릭터 레벨에 따른 스탯 데이터 가져오기
	FCharacterStatData* GetCharacterStatData(int32 CharacterLevel);
	// 캐릭터 레벨에 따른 스킬 데이터 가져오기
	FCharacterSkillData* GetCharacterSkillData(int32 CharacterLevel);
	// 캐릭터 레벨에 따른 스킬 데이터 가져오기
	FCharacterWeaponData* GetCharacterWeaponData(int32 WeaponLevel);

	// 레벨에 해당하는 타워의 스텟 반환
	FTowerStat* GetTowerStatData(FString TowerId, int32 Level);
	// 몬스터 고유 아이디에 해당하는 적의 스탯 반환
	FEnemyData* GetEnemyStatData(int32 EnemyID);

	// 웨이브 번호에 해당하는 wave 데이터 가져오기
	FWaveData* GetWaveData(int32 Wave);

	void SetAccessToken(FString NewAccessToken);
	void SetMemberId(int32 NewMemberId);
	void SetNickname(FString NewNickname);
	void SetGameId(int32 NewGameId);
	void SetMode(FString NewMode);
	void SetStartDate(FString NewStartDate);
	void SetPlaytime(int32 NewPlaytime);
	void SetLife(int32 NewLife);
	void SetMonsterKillCount();
	void SetCurrentStage();
	void SetMoney(int32 NewMoney);
	void SetScore(int32 AddToScore);

	FString GetAccessToken() { return AccessToken; }
	int32 GetMemberId() { return  MemberId; }
	FString GetNickname() { return Nickname; }
	int32 GetGameId() { return GameId; }
	FString GetMode() { return Mode; }
	FString GetStartDate() { return StartDate; }
	int32 GetPlaytime() { return Playtime; }
	int32 GetLife() { return Life; }
	int32 GetMonsterKillCount() { return MonsterKillCount; }
	int32 GetCurrentStage() { return CurrentStage; }
	int32 GetMoney() { return Money; }
	int32 GetScore()  { return MonsterKillCount * 100; }

private:
	UPROPERTY()
	class UDataTable* CharacterStatData;
	UPROPERTY()
	class UDataTable* CharacterSkillData;
	UPROPERTY()
	class UDataTable* CharacterWeaponData;
	UPROPERTY()
	class UDataTable* EnemyStatData;
	UPROPERTY()
	class UDataTable* TowerStats;
	UPROPERTY()
	class UDataTable* WaveData;

	FString AccessToken;
	int32 MemberId; // 멤버 아이디
	FString Nickname; // 닉네임
	int32 GameId; // 게임 아이디
	FString Mode; // 게임 모드 (난이도)
	FString StartDate; // 게임 시작 시각
	int32 Playtime; // 플레이 타임
	int32 Life = 3;
	int32 MonsterKillCount = 0; // 처치한 몬스터 수
	int32 CurrentStage = 1; // 현재 스테이지 수
	int32 Money = 0; // 보유 금액
	int32 Score = 0; // 점수

public:
	FOnMonsterKillCountChanged OnMonsterKillCountChanged;
	FOnCurrentStageChanged OnCurrentStageChanged;
};

