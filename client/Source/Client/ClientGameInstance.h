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

// 1. ĳ���� ���� ������
USTRUCT()
struct FCharacterStatData : public FTableRowBase
{
	GENERATED_BODY()

	// �÷��̾� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;								
	// �÷��̾� �ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHp;

	// ���� Level���� ���� Level�� ������ ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool LevelUpAvailable;

	// �������� ���� ��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelUpCost;

};


// 2. ĳ���� ��ų ���� ������
USTRUCT()
struct FCharacterSkillData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level; // �÷��̾� ����

	// ��Ŭ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LeftDamage;								// �÷��̾� Left ��ų ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LeftCoolTime;								// �÷��̾� Left ��ų ��Ÿ��
	
	// ��Ŭ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RightDamage;								// �÷��̾� Left ��ų ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RightCoolTime;							// �÷��̾� Left ��ų ��Ÿ��
	
	// Shift Ű 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShiftDamage;								// �÷��̾� Shift ��ų ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShiftCoolTime;							// �÷��̾� Shift ��ų ��Ÿ��

	// Q Ű
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float QDamage;									// �÷��̾� Q ��ų ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float QCoolTime;								// �÷��̾� Q ��ų ��Ÿ��

	// E Ű
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EDamage;									// �÷��̾� E ��ų ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ECoolTime;								// �÷��̾� E ��ų ��Ÿ��
	
	// R Ű
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RDamage;									// �÷��̾� R ��ų ������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RCoolTime;								// �÷��̾� R ��ų ��Ÿ��
	
};

// 2. ĳ���� ���� ���� ������
USTRUCT()
struct FCharacterWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponLevel;									// ���� ���� ����
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackCoefficient;							// ���� ���� ���ݷ� ���
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WeaponLevelUpAvailable;						// ���� ���� �������� ���� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponLevelUpCost;							// ���� ���� �������� �ʿ��� ��

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponLevelUpPercentage;						// ���� ���� ������ ���� Ȯ��
};

// 3. Enemy ���� ������

USTRUCT()
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Enemy ID"))
	int32 EnemyID; // ���� ID

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

// 4. Tower ���� ������
USTRUCT()
struct FTowerStat : public FTableRowBase
{
	GENERATED_BODY()

	// ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Level;

	// ���ݷ�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Attack;

	// ���� ����
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 AttackRange;

	// ���� �ӵ�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float AttackSpeed;

	// ���
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Cost;

	// ����ġ
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Stable;
};

// 5. Wave Enemy ������
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

	// ĳ���� ������ ���� ���� ������ ��������
	FCharacterStatData* GetCharacterStatData(int32 CharacterLevel);
	// ĳ���� ������ ���� ��ų ������ ��������
	FCharacterSkillData* GetCharacterSkillData(int32 CharacterLevel);
	// ĳ���� ������ ���� ��ų ������ ��������
	FCharacterWeaponData* GetCharacterWeaponData(int32 WeaponLevel);

	// ������ �ش��ϴ� Ÿ���� ���� ��ȯ
	FTowerStat* GetTowerStatData(FString TowerId, int32 Level);
	// ���� ���� ���̵� �ش��ϴ� ���� ���� ��ȯ
	FEnemyData* GetEnemyStatData(int32 EnemyID);

	// ���̺� ��ȣ�� �ش��ϴ� wave ������ ��������
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
	int32 MemberId; // ��� ���̵�
	FString Nickname; // �г���
	int32 GameId; // ���� ���̵�
	FString Mode; // ���� ��� (���̵�)
	FString StartDate; // ���� ���� �ð�
	int32 Playtime; // �÷��� Ÿ��
	int32 Life = 3;
	int32 MonsterKillCount = 0; // óġ�� ���� ��
	int32 CurrentStage = 1; // ���� �������� ��
	int32 Money = 0; // ���� �ݾ�
	int32 Score = 0; // ����

public:
	FOnMonsterKillCountChanged OnMonsterKillCountChanged;
	FOnCurrentStageChanged OnCurrentStageChanged;
};

