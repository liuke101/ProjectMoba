// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_IconDragDrop.generated.h"

class UImage;

UCLASS()
class PROJECTMOBA_API UUI_IconDragDrop : public UMobaUIBase
{
	GENERATED_BODY()

	// 拖拽时显示的Icon
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Icon;

public:

	void DrawIcon(UTexture2D *InIcon) const;
};
