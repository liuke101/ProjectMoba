// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI_CD.h"
#include "UI_BuffSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_BuffSlot : public UUI_CD
{
	GENERATED_BODY()
	
public:
	UUI_BuffSlot();

	virtual void NativeConstruct() override;

	void UpdateCD(int32 InSlotID,float InMaxCD);
	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	float CurrentBuffCD;
};
