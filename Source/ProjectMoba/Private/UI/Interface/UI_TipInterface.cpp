// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Interface/UI_TipInterface.h"

#include "Common/MethodUnit.h"
#include "Game/MobaPlayerState.h"
#include "Table/SlotAsset.h"
#include "UI/Tip/UI_Tip.h"


// Add default functionality here for any IUI_TipInterface functions that are not pure virtual.
bool IUI_TipInterface::VerifyTip(AMobaPlayerState* MobaPlayerState, UUI_Tip* Tip, int32 SlotID, FSlotData* ClientSlotData)
{
	if(MobaPlayerState && Tip && SlotID != INDEX_NONE)
	{
		if(ClientSlotData==nullptr)
		{
			ClientSlotData = MobaPlayerState->GetSlotData(SlotID);
		}

		if(ClientSlotData)
		{
			if(ClientSlotData->DataID != INDEX_NONE)
			{
				if(FSlotAttribute* SlotAttribute = MobaPlayerState->GetSlotAttributeFromDataID(ClientSlotData->DataID))
				{
					if(const FSlotAsset* SlotAsset = MobaPlayerState->GetSlotAssetFromDataID(ClientSlotData->DataID))
					{
						MethodUnit::SetToolTip(Tip, SlotAttribute, SlotAsset);
						return true;
					}
				}
			}
		}
	}
	return false;
}
