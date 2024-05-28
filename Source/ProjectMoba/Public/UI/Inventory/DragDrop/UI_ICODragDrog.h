// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_ICODragDrog.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_ICODragDrog : public UMobaUIBase
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Icon;

public:

	void DrawIcon(UTexture2D *InIcon) const;
};
