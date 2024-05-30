// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumTypes.generated.h"
/**
 * 
 */
UENUM(BlueprintType)
enum class EnemyType : uint8
{
	GROUND UMETA(DisplayName = "Ground"),
	AIR UMETA(DisplayName = "Air"),
	BOSS UMETA(DisplayName = "Boss")
};