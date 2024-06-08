// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_StatusBar_Health.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_StatusBar_Health : public UMobaUIBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetHealthPercent(const float InPercent) const;

	void SetColor(const FLinearColor& InColor) const;

private:

	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	
};
