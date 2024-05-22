// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectMoba/MobaType.h"
#include "MobaCharacter.generated.h"

struct FCharacterAttribute;

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
	/** 普通攻击 */
	void NormalAttack(TWeakObjectPtr<AMobaCharacter> InTarget);

	/** 技能攻击 */
	void SkillAttack(ESkillKey SkillKey, TWeakObjectPtr<AMobaCharacter> InTarget);
	UAnimMontage* GetCurrentSkillMontage(ESkillKey SkillKey) const;

	/** 动画广播 */
	UFUNCTION(NetMulticast, Unreliable)
	void MultiCastPlayerAnimMontage(UAnimMontage* InAnimMontage, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);

	/** 将PlayerID和CharacterID注册到Map结构（GameState中即服务器上）*/
	void RegisterCharacterOnServer(const int64 InPlayerID, const int32 InCharacterID);
	
	FORCEINLINE void SetPlayerID(const int64 InPlayerID) { PlayerID = InPlayerID; } 
	FORCEINLINE int64 GetPlayerID() const { return PlayerID; }
	const FCharacterAttribute* GetCharacterAttribute() const;
	bool IsDie() const;
private:
	bool bAttacking;
	uint8 AttackCount; //攻击计数
	int64 PlayerID;
};

