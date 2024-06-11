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
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> BuffBox;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_BuffSlot> BuffSlotClass;
public:
	UUI_BuffBar();

	virtual void NativeConstruct() override;

	void UpdateCD(int32 InSlotID, float InMaxCD) const;

	void Clear();
};
