// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MobaUIBase.generated.h"

class UUI_Tip;
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

public:
	AMobaHUD* GetMobaHUD() const;
	UWidgetAnimation* GetNameWidgetAnimation(const FString& WidgetAnimationName) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Moba|UI")
	virtual UUI_Tip* GetTip();  //在蓝图中绑定Tool Tip Widget

protected:
	virtual void BindDelegate();
	virtual void RemoveDelegate();

public:

	UPROPERTY()
	TObjectPtr<AMobaPlayerState> MobaPlayerState = nullptr;
	
	UPROPERTY()
	TObjectPtr<AMobaGameState> MobaGameState = nullptr;

	FORCEINLINE int64 GetPlayerID() const { return PlayerID; }
	virtual void SetPlayerID(int64 InPlayerID) { PlayerID = InPlayerID; }

protected:
	UPROPERTY()
	int64 PlayerID = INDEX_NONE;

	UPROPERTY()
	TObjectPtr<UUI_Tip>	Tip;

	//提示类
	UPROPERTY(EditDefaultsOnly, Category = "Moba|UI")
	TSubclassOf<UUI_Tip> TipClass;
	
private:
	void InitMobaPlayerState() ;
	void InitMobaGameState() ;

	
};
