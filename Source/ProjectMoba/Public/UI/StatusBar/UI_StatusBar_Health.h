// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_StatusBar_Health.generated.h"

class UUI_BuffBar;
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

	virtual void BindDelegate() override;
	virtual void RemoveDelegate() override;

	FORCEINLINE void SetPlayerID(int64 InPlayerID) {PlayerID = InPlayerID;}
	FORCEINLINE int64 GetPlayerID() const {return PlayerID;}

private:

	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(meta= (BindWidget))
	TObjectPtr<UUI_BuffBar> BuffBar;

protected:
	int64 PlayerID = INDEX_NONE;

private:
	FDelegateHandle UpdateBuffBarDelegateHandle;
	
};
