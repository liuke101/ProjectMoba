// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaUIBase.h"
#include "MobaMainScreenUI.generated.h"

class UMobaMiniMapUI;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UMobaMainScreenUI : public UMobaUIBase
{
	GENERATED_BODY()

public:

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UMobaMiniMapUI> UI_MiniMap;

public:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
