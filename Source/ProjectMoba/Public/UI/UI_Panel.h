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
	virtual void BindDelegate() override;

	/** 请求更新属性 */
	virtual void RequestUpdateSlot(const ECharacterAttributeType CharacterAttributeType);
	
	/** 响应协议更新 */
	UFUNCTION()
	virtual void ResponseUpdateSlot(int64 InPlayerID, const ECharacterAttributeType CharacterAttributeType);

	/** 响应整包更新 */
	UFUNCTION()
	virtual void ResponseUpdateSlots(int64 InPlayerID);
};
