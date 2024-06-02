// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectMoba/MobaType.h"
#include "UI/MobaUIBase.h"
#include "UI_Panel.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_Panel : public UMobaUIBase
{
	GENERATED_BODY()
public:
	UUI_Panel();
	
	virtual void NativeConstruct() override;

	FORCEINLINE int64 GetPlayerID() const {return PlayerID;}
	FORCEINLINE void SetPlayerID(const int64& InPlayerID) { PlayerID = InPlayerID; }
	
	/** 请求更新属性 */
	virtual void RequestUpdateSlot(const ECharacterAttributeType CharacterAttributeType);
protected:
	/** 响应更新属性 */
	UFUNCTION()
	virtual void ResponseUpdateSlot(int64 InPlayerID, const ECharacterAttributeType CharacterAttributeType);
	
	
	int64 PlayerID;
};
