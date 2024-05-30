// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BarricadeBuildWidget.generated.h"

/**
 * 
 */
UCLASS()
class CLIENT_API UBarricadeBuildWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

private:
	// 호버 이벤트
	UFUNCTION() void HoveredBuildUpButton();
	UFUNCTION() void HoveredBuildDownButton();
	UFUNCTION() void HoveredBuildLeftButton();
	UFUNCTION() void HoveredBuildRightButton();

	// 호버 해제 이벤트
	UFUNCTION() void UnhoveredButton();

public:
	// 바리케이드 설치버튼 반환
	class UTowerButtonWidget* GetBuildUpButtonWidget() { return BuildUp; }
	class UTowerButtonWidget* GetBuildDownButtonWidget() { return BuildDown; }
	class UTowerButtonWidget* GetBuildLeftButtonWidget() { return BuildLeft; }
	class UTowerButtonWidget* GetBuildRightButtonWidget() { return BuildRight; }
	void SetBarricadeCost(int32 Cost);

private:
	UPROPERTY(meta = (BindWidget)) class UTowerButtonWidget* BuildUp;
	UPROPERTY(meta = (BindWidget)) class UTowerButtonWidget* BuildDown;
	UPROPERTY(meta = (BindWidget)) class UTowerButtonWidget* BuildLeft;
	UPROPERTY(meta = (BindWidget)) class UTowerButtonWidget* BuildRight;
	UPROPERTY(meta = (BindWidget)) class UDetailPanel* DetailPanel;

public:
	// 텍스쳐
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	UTexture2D* BuildUpTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	UTexture2D* BuildDownTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	UTexture2D* BuildLeftTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	UTexture2D* BuildRightTexture;

	// 디테일 패널 타이틀
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTitle)
	FString BuildUpTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTitle)
	FString BuildDownTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTitle)
	FString BuildLeftTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTitle)
	FString BuildRightTitle;

	// 디테일 페널 설명
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonDescription)
	FString BuildUpDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonDescription)
	FString BuildDownDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonDescription)
	FString BuildLeftDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonDescription)
	FString BuildRightDescription;
};
