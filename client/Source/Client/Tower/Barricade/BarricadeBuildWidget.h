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
	// ȣ�� �̺�Ʈ
	UFUNCTION() void HoveredBuildUpButton();
	UFUNCTION() void HoveredBuildDownButton();
	UFUNCTION() void HoveredBuildLeftButton();
	UFUNCTION() void HoveredBuildRightButton();

	// ȣ�� ���� �̺�Ʈ
	UFUNCTION() void UnhoveredButton();

public:
	// �ٸ����̵� ��ġ��ư ��ȯ
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
	// �ؽ���
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	UTexture2D* BuildUpTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	UTexture2D* BuildDownTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	UTexture2D* BuildLeftTexture;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTextures)
	UTexture2D* BuildRightTexture;

	// ������ �г� Ÿ��Ʋ
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTitle)
	FString BuildUpTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTitle)
	FString BuildDownTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTitle)
	FString BuildLeftTitle;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonTitle)
	FString BuildRightTitle;

	// ������ ��� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonDescription)
	FString BuildUpDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonDescription)
	FString BuildDownDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonDescription)
	FString BuildLeftDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ButtonDescription)
	FString BuildRightDescription;
};
