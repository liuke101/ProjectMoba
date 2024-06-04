// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_TeamInfo.generated.h"

class UUI_PlayersInfo;
class UVerticalBox;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_TeamInfo : public UMobaUIBase
{
	GENERATED_BODY()
		
public:
	virtual void NativeConstruct() override;
	void SpawnPlayerInfo(bool bEnemy,const int64 &InPlayerID,const TArray<int32>& InSlotsID) const;


private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> EnemyTeam;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> FriendlyTeam;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_PlayersInfo> PlayersInfoClass;

};
