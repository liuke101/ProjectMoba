// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaStatusBarUI_Health.h"
#include "MobaStatusBarUI.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UMobaStatusBarUI : public UMobaStatusBarUI_Health
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetTitle(const FString& InTitle) const;
	void SetManaPercent(const float InPercent) const;

private:
	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UTextBlock> Title;

	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UProgressBar> ManaBar;
};
