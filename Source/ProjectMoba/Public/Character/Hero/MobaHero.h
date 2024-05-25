// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MobaCharacter.h"
#include "MobaHero.generated.h"

UCLASS()
class PROJECTMOBA_API AMobaHero : public AMobaCharacter
{
	GENERATED_BODY()

public:
	AMobaHero();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void InitCharacter() override;

	UFUNCTION(NetMulticast, Unreliable)
	void MultiCastStatusBar_PlayerName(const FString& PlayerName);

	UFUNCTION(NetMulticast, Unreliable)
	void MultiCastStatusBar_Level(const int32 Level);
	
	/** 技能攻击 */
	void SkillAttack(ESkillKey SkillKey, TWeakObjectPtr<AMobaHero> InTarget);
	
	UAnimMontage* GetCurrentSkillMontage(ESkillKey SkillKey) const;

};
