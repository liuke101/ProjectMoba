// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectMoba/MobaType.h"
#include "MobaCharacter.generated.h"

class UWidgetComponent;
struct FCharacterAttribute;

UCLASS(Blueprintable)
class PROJECTMOBA_API AMobaCharacter : public ACharacter
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

	UFUNCTION(NetMulticast, Unreliable)
	void MultiCastStatusBar(float HealthPercent, float ManaPercent);
	
	/** 将PlayerID和CharacterID注册到Map结构（GameState中即服务器上）*/
	void RegisterCharacterOnServer(const int64 InPlayerID, const int32 InCharacterID, const ETeamType InTeamType);
	void InitCharacter();
	
	FORCEINLINE void SetPlayerID(const int64 InPlayerID) { PlayerID = InPlayerID; } 
	FORCEINLINE int64 GetPlayerID() const { return PlayerID; }
	
	const FCharacterAttribute* GetCharacterAttribute() const;
	
	bool IsDie() const;
	
	FORCEINLINE void SetTeamType(ETeamType InTeamType) { TeamType = InTeamType; }
	FORCEINLINE ETeamType GetTeamType() const { return TeamType; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|UI")
	TObjectPtr<UWidgetComponent> StatusBarComponent;
private:
	bool bAttacking;
	uint8 AttackCount; //攻击计数
	int64 PlayerID;
	ETeamType TeamType;

	FTimerHandle InitTimerHandle;
	
};

