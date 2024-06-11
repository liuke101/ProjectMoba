// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MobaUIBase.generated.h"

class AMobaGameState;
class AMobaPlayerState;
class AMobaHUD;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UMobaUIBase : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	AMobaHUD* GetMobaHUD() const;
	UWidgetAnimation* GetNameWidgetAnimation(const FString& WidgetAnimationName) const;
	virtual void BindDelegate();
	virtual void RemoveDelegate();

	UPROPERTY()
	TObjectPtr<AMobaPlayerState> MobaPlayerState = nullptr;
	
	UPROPERTY()
	TObjectPtr<AMobaGameState> MobaGameState = nullptr;

private:
	void InitMobaPlayerState() ;
	void InitMobaGameState() ;
};
