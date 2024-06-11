// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_BuffBar.generated.h"

class UUI_BuffSlot;
class UWrapBox;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_BuffBar : public UMobaUIBase
{
	GENERATED_BODY()
public:
	UUI_BuffBar();

	virtual void NativeConstruct() override;

	void UpdateCD(int32 InSlotID, float InMaxCD) const;
	void UpdateCD(int64 InPlayerID, int32 InSlotID, float InMaxCD) const;

	void Clear();

	void SetPlayerID(int64 InPlayerID) { PlayerID = InPlayerID; }

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> BuffBox;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_BuffSlot> BuffSlotClass;

private:
	int64 PlayerID = INDEX_NONE;

};
