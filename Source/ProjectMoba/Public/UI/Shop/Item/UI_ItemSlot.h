// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_ItemBase.h"
#include "UI_ItemSlot.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_ItemSlot : public UUI_ItemBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	virtual void OnClickedWidget() override;
};
