// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Buff/UI_BuffSlot.h"

#include "Common/MethodUnit.h"
#include "Table/SlotAsset.h"


UUI_BuffSlot::UUI_BuffSlot()
	: CurrentBuffCD(999999.0f)
{
	
}

void UUI_BuffSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_BuffSlot::UpdateCD(int32 InSlotID, float InMaxCD)
{
	BuildSlot.MaxCD = InMaxCD;
	BuildSlot.SlotID = InSlotID;
	CurrentBuffCD = InMaxCD;

	if (AMobaPlayerState *InPlayerState = MethodUnit::GetMobaPlayerState(GetWorld()))
	{
		if (const FSlotAsset* Asset = InPlayerState->GetSlotAssetFromDataID(InSlotID)) //为什么传入的是InSlotID？？？
			//(const FSlotAsset* Asset = InPlayerState->GetSlotAssetFromSlotID(InSlotID))
		{
			UpdateIcon(Asset->SlotIcon);
		}
	}
}

void UUI_BuffSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CurrentBuffCD -= InDeltaTime;
	DrawSlotCDMat(CurrentBuffCD / BuildSlot.MaxCD);
	if (CurrentBuffCD <= 0.f)
	{
		CurrentBuffCD = 0.f;

		RemoveFromParent();
	}
}

