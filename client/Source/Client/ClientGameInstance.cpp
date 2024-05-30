// Fill out your copyright notice in the Description page of Project Settings.


#include "ClientGameInstance.h"

UClientGameInstance::UClientGameInstance()
{
	// ĳ���� ���� ������ �ҷ�����
	static ConstructorHelpers::FObjectFinder<UDataTable> CHARACTERSTATDATA(TEXT("DataTable'/Game/DataTables/Character/Magician/MagicianStatDataTable.MagicianStatDataTable'"));
	if (CHARACTERSTATDATA.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Stat Data Succeeded!"));
		CharacterStatData = CHARACTERSTATDATA.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Stat Data Failed"))
	}
	// ĳ���� ��ų ������ �ҷ�����
	static ConstructorHelpers::FObjectFinder<UDataTable> CHARACTERSKILLDATA(TEXT("DataTable'/Game/DataTables/Character/Magician/MagicianSkillDataTable.MagicianSkillDataTable'"));
	if (CHARACTERSKILLDATA.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Skill Data Succeeded!"));
		CharacterSkillData = CHARACTERSKILLDATA.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Skill Data Failed"))
	}

	// ���� ���� ������ �ҷ�����
	static ConstructorHelpers::FObjectFinder<UDataTable> CHARACTERWEAPONDATA(TEXT("DataTable'/Game/DataTables/Character/Magician/MagicianWeaponDataTable.MagicianWeaponDataTable'"));
	if (CHARACTERWEAPONDATA.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Weapon Data Succeeded!"));
		CharacterWeaponData = CHARACTERWEAPONDATA.Object;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Character Stat Data Failed"))
	}
		
	// Ÿ�� ���� ������ �ҷ�����
	static ConstructorHelpers::FObjectFinder<UDataTable> TOWER_STAT_DATA(TEXT("DataTable'/Game/DataTables/Tower/TowerStatTable.TowerStatTable'"));
	if (TOWER_STAT_DATA.Succeeded()) {
		TowerStats = TOWER_STAT_DATA.Object;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Tower Stat Data Failed"))
	}

	// Enemy ���� ������ �ҷ�����
	static ConstructorHelpers::FObjectFinder<UDataTable> ENEMY_STAT_DATA(TEXT("DataTable'/Game/DataTables/Enemy/EnemyStatData.EnemyStatData'"));
	if (ENEMY_STAT_DATA.Succeeded()) {
		UE_LOG(LogTemp, Warning, TEXT("Enemy Stat Data Succeeded!"))
		EnemyStatData = ENEMY_STAT_DATA.Object;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Enemy Stat Data Failed"))
	}

	// Wave ��ü ���� �ҷ�����
	static ConstructorHelpers::FObjectFinder<UDataTable> WAVE_DATA(TEXT("DataTable'/Game/DataTables/Wave/WaveData.WaveData'"));
	if (WAVE_DATA.Succeeded()) {
		WaveData = WAVE_DATA.Object;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Waves Data Failed"))
	}
}

void UClientGameInstance::Init()
{
	Super::PostInitProperties();
}

FCharacterStatData* UClientGameInstance::GetCharacterStatData(int32 Level)
{
	auto CSD = CharacterStatData->FindRow<FCharacterStatData>(*FString::FromInt(Level), TEXT("Level"));
	if (CSD != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success to Find Character Stat Data Row by Key Value : Level"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Fail to Find Character Stat Data Row by Key Value : Level"));

	}
	return CSD;
}

FCharacterSkillData* UClientGameInstance::GetCharacterSkillData(int32 Level)
{
	auto CSD = CharacterSkillData->FindRow<FCharacterSkillData>(*FString::FromInt(Level), TEXT(""));
	if (CSD != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success to Find Character Skill Data Row by Key Value : Level"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Fail to Find Character Skill Data Row by Key Value : Level"));

	}
	return CSD;
}

FCharacterWeaponData* UClientGameInstance::GetCharacterWeaponData(int32 WeaponLevel)
{	
	auto CWD = CharacterWeaponData->FindRow<FCharacterWeaponData>(*FString::FromInt(WeaponLevel), TEXT(""));
	if (CWD != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Success to Find Character Weapon Data Row by Key Value : WeaponLevel"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Fail to Find Character Weapon Data Row by Key Value : Level"));

	}
	return CWD;
}

// ������ �ش��ϴ� Ÿ���� ���� ��ȯ
FTowerStat* UClientGameInstance::GetTowerStatData(FString TowerId, int32 Level)
{
	if (TowerStats != nullptr) {
		FString RowString = TowerId + TEXT("_") + FString::FromInt(Level);
		return TowerStats->FindRow<FTowerStat>(*RowString, TEXT(""));
	}
	return nullptr;
}

// ���� ���� ID �� �ش��ϴ� ���� ���� ��ȯ
FEnemyData* UClientGameInstance::GetEnemyStatData(int32 EnemyID)
{
	if (EnemyStatData != nullptr) {
		return EnemyStatData->FindRow<FEnemyData>(*FString::FromInt(EnemyID), TEXT(""));
	}
	return nullptr;
}


// ���̺� ��ȣ�� �ش��ϴ� ���̺� ������ ��ȯ
FWaveData* UClientGameInstance::GetWaveData(int32 Wave)
{
	if (WaveData != nullptr) {
		return WaveData->FindRow<FWaveData>(*FString::FromInt(Wave), TEXT(""));
	}
	return nullptr;
}

void UClientGameInstance::SetGameId(int32 NewGameId)
{
	GameId = NewGameId;
}

void UClientGameInstance::SetMode(FString NewMode)
{
	Mode = NewMode;
}

void UClientGameInstance::SetMoney(int32 NewMoney)
{
	Money = NewMoney;
}

void UClientGameInstance::SetScore(int32 AddToScore)
{
	Score += AddToScore;

}

void UClientGameInstance::SetStartDate(FString NewStartDate)
{
	StartDate = NewStartDate;

}

void UClientGameInstance::SetPlaytime(int32 NewPlaytime)
{
	Playtime = NewPlaytime;
}

void UClientGameInstance::SetMemberId(int32 NewMemberId)
{
	MemberId = NewMemberId;
}

void UClientGameInstance::SetNickname(FString NewNickname)
{
	Nickname = NewNickname;
}

void UClientGameInstance::SetLife(int32 NewLife)
{
	Life = NewLife;
}


void UClientGameInstance::SetMonsterKillCount()
{
	MonsterKillCount += 1;
	OnMonsterKillCountChanged.Broadcast();
}

void UClientGameInstance::SetCurrentStage()
{
	CurrentStage += 1;
	OnCurrentStageChanged.Broadcast();
}

void UClientGameInstance::SetAccessToken(FString NewAccessToken)
{
	AccessToken = NewAccessToken;
}
