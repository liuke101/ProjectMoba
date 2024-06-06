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

	/** 动画广播 */
	UFUNCTION(NetMulticast, Unreliable)
	void MultiCastPlayerAnimMontage(UAnimMontage* InAnimMontage, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);

	/** 状态栏广播 */
	UFUNCTION(NetMulticast, Unreliable)
	void MultiCastStatusBar(float HealthPercent, float ManaPercent);
	UFUNCTION(NetMulticast, Unreliable)
	void MultiCastStatusBar_Health(float HealthPercent);
	UFUNCTION(NetMulticast, Unreliable)
	void MultiCastStatusBar_Mana(float ManaPercent);
	
	/** 重生 */
	UFUNCTION(NetMulticast, Unreliable)
	void MultiCastReborn();
	
	/** 将PlayerID和CharacterID注册到Map结构（GameState中即服务器上）*/
	UFUNCTION()
	void RegisterCharacterOnServer(const int64 InPlayerID, const int32 InCharacterID, const ETeamType InTeamType, const ECharacterType InCharacterType);

	UFUNCTION()
	virtual void InitCharacter();
	
	FORCEINLINE void SetPlayerID(const int64 InPlayerID) { PlayerID = InPlayerID; } 
	FORCEINLINE int64 GetPlayerID() const { return PlayerID; }

	FCharacterAttribute* GetCharacterAttribute();
	
	bool IsDead();
	
	FORCEINLINE void SetTeamType(ETeamType InTeamType) { TeamType = InTeamType; }
	FORCEINLINE ETeamType GetTeamType() const { return TeamType; }
	FORCEINLINE void SetCharacterType(ECharacterType InCharacterType) { CharacterType = InCharacterType; }
	FORCEINLINE ECharacterType GetCharacterType() const { return CharacterType; }
	void ResetSpeed(float InSpeed);
	
	FORCEINLINE UArrowComponent* GetFirePointComponent() const { return FirePointComponent; }
	FVector GetFirePointLocation() const;
	FRotator GetFirePointRotation() const;

protected:
	/** 状态栏 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|UI")
	TObjectPtr<UWidgetComponent> StatusBarComponent;

	/** 远程攻击 开火点 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|UI")
	TObjectPtr<UArrowComponent> FirePointComponent;

	/** 接收伤害 */
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	bool bAttacking;
	uint8 AttackCount; //攻击计数
	int64 PlayerID;
	ETeamType TeamType;
	ECharacterType CharacterType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moba|Character")
	float RebornTime;
};

