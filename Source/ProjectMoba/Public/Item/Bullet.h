// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DamageBox.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class AMobaCharacter;
class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class PROJECTMOBA_API ABullet : public ADamageBox
{
	GENERATED_BODY()

public:
	ABullet();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	//void SetHomingTarget(const AMobaCharacter* Target) const;
	
	//投射物组件，用于远程攻击
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Bullet")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

};
