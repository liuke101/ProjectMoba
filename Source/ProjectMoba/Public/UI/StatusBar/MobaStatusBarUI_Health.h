// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "MobaStatusBarUI_Health.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UMobaStatusBarUI_Health : public UMobaUIBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetHealthPercent(const float InPercent) const;

private:

	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
};
