// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MobaCharacter.h"
#include "MobaHeroCharacter.generated.h"

UCLASS()
class PROJECTMOBA_API AMobaHeroCharacter : public AMobaCharacter
{
	GENERATED_BODY()

public:
	AMobaHeroCharacter();

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
	void SkillAttack(ESkillKey SkillKey, TWeakObjectPtr<AMobaHeroCharacter> InTarget);
	
	UAnimMontage* GetCurrentSkillMontage(ESkillKey SkillKey) const;

};
