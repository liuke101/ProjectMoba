// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MobaCharacter.h"
#include "System/MobaAssistSystem.h"
#include "MobaTurretCharacter.generated.h"

struct FMobaAssitSystem;

UCLASS()
class PROJECTMOBA_API AMobaTurretCharacter : public AMobaCharacter
{
	GENERATED_BODY()

public:
	AMobaTurretCharacter();
	
	virtual void Tick(float DeltaSeconds) override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:

	/** 助攻 */
	FMobaAssitSystem* GetMobaAssistSystem() { return &MobaAssistSystem; }
	FORCEINLINE TArray<FAssistPlayer> GetAssistPlayers() const;
	void AddAssistPlayer(const int64& InPlayerID);
	const FAssistPlayer* GetLastAssistPlayer(); //获取最后一个助攻玩家

	UPROPERTY(Replicated, BlueprintReadOnly)
	FVector2D TurretRotator = FVector2D::Zero();  //我们只需要Pitch和Yaw，不需要Roll，为了节省带宽使用FVector2D而不是FRotator

private:
	FMobaAssitSystem MobaAssistSystem;
};
