// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectMoba/MobaType.h"
#include "MobaCharacter.generated.h"

UCLASS(Blueprintable)
class AMobaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMobaCharacter();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

public:
	void NormalAttack(TWeakObjectPtr<AMobaCharacter> InTarget);
	void SkillAttack(ESkillKey SkillKey, TWeakObjectPtr<AMobaCharacter> InTarget);
	UAnimMontage* GetCurrentSkillMontage(ESkillKey SkillKey) const;

	UFUNCTION(NetMulticast, Unreliable)
	void MultiCastPlayerAnimMontage(UAnimMontage* InAnimMontage, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);

	void InitCharacterID(const int32& InCharacterID);
	FORCEINLINE int32 GetCharacterID() const { return CharacterID; }
private:
	bool bAttacking;
	uint8 AttackCount; //攻击计数
	int32 CharacterID;
};

