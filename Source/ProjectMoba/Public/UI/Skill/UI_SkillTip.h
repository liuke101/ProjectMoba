// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_SkillTip.generated.h"

class URichTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_SkillTip : public UMobaUIBase
{
	GENERATED_BODY()

public:
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URichTextBlock> RichText;
};
