// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UI_TipInterface.generated.h"

struct FSlotData;
class UUI_Tip;
class AMobaPlayerState;
// This class does not need to be modified.
UINTERFACE()
class UUI_TipInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTMOBA_API IUI_TipInterface
{
	GENERATED_BODY()

public:
	bool VerifyTip(AMobaPlayerState* MobaPlayerState, UUI_Tip* Tip, int32 SlotID, FSlotData* ClientSlotData = nullptr);
};
