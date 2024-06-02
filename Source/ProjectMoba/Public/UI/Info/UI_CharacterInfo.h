// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UI_Panel.h"
#include "UI_CharacterInfo.generated.h"

enum class ECharacterAttributeType : uint8;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_CharacterInfo : public UUI_Panel
{
	GENERATED_BODY()
public:
	
	virtual void NativeConstruct() override;


protected:
	/** 响应更新属性 */
	virtual void ResponseUpdateSlot(int64 InPlayerID, const ECharacterAttributeType CharacterAttributeType) override;

private:
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
	TObjectPtr<UTextBlock> CriticalRate;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MagicAttack;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MagicResistance;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MagicPenetration;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WalkSpeed;

};
