// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI/UI_Bar.h"
#include "UI_PlayersInfo.generated.h"

struct FPlayerTeamNetPackage;
class UUI_KDAInfo;
class UHorizontalBox;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_PlayersInfo : public UUI_Bar
{
	GENERATED_BODY()
public:
	UUI_PlayersInfo();

	virtual void NativeConstruct() override;

	void UpdateSlot(const FPlayerTeamNetPackage& PlayerTeamNetPackage);
protected:
	virtual UPanelWidget* GetSlotLayoutPanel() override;

private:
	int64 PlayerID;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LevelText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> SlotLayoutPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_KDAInfo> KDAInfo;
	
};
