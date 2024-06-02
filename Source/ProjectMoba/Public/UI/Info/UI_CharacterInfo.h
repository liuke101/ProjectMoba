// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_CharacterInfo.generated.h"

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
	virtual void NativeConstruct() override;

	void UpdateSlot();

private:
	
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
	TObjectPtr<UTextBlock> MagicDefense;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MagicPenetration;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WalkSpeed;

	UPROPERTY(meta = (BindWidget))
	UImage* CharacterIcon;
};
