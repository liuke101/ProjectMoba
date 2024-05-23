// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "MobaStatusBarUI.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UMobaStatusBarUI : public UMobaUIBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetTitle(const FString& InTitle) const;
	void SetHealthPercent(const float InPercent) const;
	void SetManaPercent(const float InPercent) const;

private:
	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UTextBlock> Title;

	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UProgressBar> ManaBar;
};
