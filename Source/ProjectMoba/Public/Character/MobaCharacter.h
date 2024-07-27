// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectMoba/MobaType.h"
#include "MobaCharacter.generated.h"

class ABullet;
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma region "战斗"
public:
	// 普通攻击，由AIController驱动，仅运行在服务器
	void NormalAttack(TWeakObjectPtr<AMobaCharacter> InTarget);
	
	virtual void Die();
	bool IsDead() const;
	

	//添加子弹弱引用
	void AddBulletPtr(ABullet* InBullet);

	//呼叫所有子弹停止射击
	void CallBulletEndFire();

	// 锁定旋转
	void LockRotate(bool InbLockRotate);
	void ResetRotation();
	
protected:
	/** 接收伤害 */
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	
#pragma endregion
	
#pragma region "RPC"
public:
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_RegisterCharacter(int64 InPlayerID);
	
	/** 动画广播 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayerAnimMontage(UAnimMontage* InAnimMontage, float InPlayRate = 1.0f, FName StartSectionName = NAME_None);

	/** 状态栏广播 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_StatusBar(float HealthPercent, float ManaPercent);
	
	void InitHealthBarColorfromTeamType(); //跟据队伍类型初始化血条颜色
	
	/** 重生 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_Reborn();

	/** 显示伤害字体 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SpwanDrawDamageText(float Value, float Percent, const FLinearColor& Color, const FVector& Location);

	/** 命中特效 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_SpawnHitVFX(const FVector& HitLocation, UParticleSystem* HitVFX);

#pragma endregion
	
	/** 将PlayerID和CharacterID注册到Map结构（GameState中即服务器上）*/
	UFUNCTION()
	void RegisterCharacterOnServer(const int64 InPlayerID, const int32 InCharacterID, const ETeamType InTeamType, const ECharacterType InCharacterType);

	UFUNCTION()
	virtual void InitCharacter();

	/** 广播UI信息 */
	void InitWidgetInfo();


#pragma region "Getter/Setter"
public:
	FORCEINLINE void SetPlayerID(const int64 InPlayerID) { PlayerID = InPlayerID; } 
	FORCEINLINE int64 GetPlayerID() const { return PlayerID; }

	FCharacterAttribute* GetCharacterAttribute() const;
	
	FORCEINLINE void SetTeamType(ETeamType InTeamType) { TeamType = InTeamType; }
	FORCEINLINE ETeamType GetTeamType() const { return TeamType; }
	FORCEINLINE void SetCharacterType(ECharacterType InCharacterType) { CharacterType = InCharacterType; }
	FORCEINLINE ECharacterType GetCharacterType() const { return CharacterType; }
	void ResetSpeed(float InSpeed);
	
	FORCEINLINE UArrowComponent* GetFirePointComponent() const { return FirePointComponent; }
	FVector GetFirePointLocation() const;
	FRotator GetFirePointRotation() const;

	void SetHealthPercent(float HealthPercent) const;
	void SetManaPercent(float ManaPercent) const;
	void SetLevel(int32 Level) const;

#pragma endregion

protected:
	/** 状态栏 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|UI")
	TObjectPtr<UWidgetComponent> StatusBarComponent;

	/** 远程攻击 开火点 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|UI")
	TObjectPtr<UArrowComponent> FirePointComponent;

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

	// 对角色射出的子弹的引用
	TArray<TWeakObjectPtr<ABullet>> BulletPtrs;
	
	bool bLockRotate; //锁定旋转
	FRotator CurrentRotation; //当前旋转
};

