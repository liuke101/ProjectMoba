// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectMoba/MobaType.h"
#include "MobaCharacter.generated.h"

class ADrawText;
class UWidgetComponent;
struct FCharacterAttribute;

UCLASS(Blueprintable)
class PROJECTMOBA_API AMobaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMobaCharacter();
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

public:
	/** 普通攻击 */
	void NormalAttack(TWeakObjectPtr<AMobaCharacter> InTarget);

#pragma region RPC
	/** 动画广播 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayerAnimMontage(UAnimMontage* InAnimMontage, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);

	/** 状态栏广播 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_StatusBar(float HealthPercent, float ManaPercent);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_StatusBar_Health(float HealthPercent);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_StatusBar_Mana(float ManaPercent);
	
	void InitHealthBarColorfromTeamType(); //跟据队伍类型初始化血条颜色
	
	/** 重生 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_Reborn();

	/** 显示伤害字体 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SpwanDrawText(float Value, float Percent, const FLinearColor& Color, const FVector& Location);
#pragma endregion
	
	/** 将PlayerID和CharacterID注册到Map结构（GameState中即服务器上）*/
	UFUNCTION()
	void RegisterCharacterOnServer(const int64 InPlayerID, const int32 InCharacterID, const ETeamType InTeamType, const ECharacterType InCharacterType);

	UFUNCTION()
	virtual void InitCharacter();

	/** 广播UI信息 */
	void InitWidgetInfo();
	
	FORCEINLINE void SetPlayerID(const int64 InPlayerID) { PlayerID = InPlayerID; } 
	FORCEINLINE int64 GetPlayerID() const { return PlayerID; }

	FCharacterAttribute* GetCharacterAttribute() const;
	
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

	UPROPERTY(Replicated)
	ETeamType TeamType;
	
	ECharacterType CharacterType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moba|Character")
	float RebornTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moba|Character")
	TSubclassOf<ADrawText> DrawTextClass;
};

