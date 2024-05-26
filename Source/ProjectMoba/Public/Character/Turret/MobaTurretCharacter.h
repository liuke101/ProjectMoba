// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/MobaCharacter.h"
#include "MobaTurretCharacter.generated.h"

UCLASS()
class PROJECTMOBA_API AMobaTurretCharacter : public AMobaCharacter
{
	GENERATED_BODY()

public:
	AMobaTurretCharacter();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FVector2D TurretRotator = FVector2D::Zero();  //我们只需要Pitch和Yaw，不需要Roll，为了节省带宽使用FVector2D而不是FRotator
};
