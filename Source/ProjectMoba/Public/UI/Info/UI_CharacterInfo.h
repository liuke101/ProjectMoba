// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_CharacterInfo.generated.h"

enum class ECharacterAttributeType : uint8;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_CharacterInfo : public UMobaUIBase
{
	GENERATED_BODY()
public:
	UUI_CharacterInfo();
	
	virtual void NativeConstruct() override;

	/** 请求更新属性 */
	void RequestUpdateSlot(const ECharacterAttributeType CharacterAttributeType);

	/** 响应更新属性 */
	void ResponseUpdateSlot(int64 InPlayerID, const ECharacterAttributeType CharacterAttributeType, float Value);

	FORCEINLINE void SetPlayerID(const int64& InPlayerID) { PlayerID = InPlayerID; }
private:
	int64 PlayerID;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CharacterIcon;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PhysicalAttack; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Armor;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PhysicalPenetration;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttackSpeed;

	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MagicAttack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MagicResistance;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MagicPenetration;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WalkSpeed;

	

};
