// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class PROJECTMOBA_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	ABullet();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE void SetRangeCheck(bool bCheck) { bSingleCheck = bCheck; }
	
protected:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bSingleCheck = false; //如果为true则为单体检测，否则为范围检测
private:
	UPROPERTY()
	TObjectPtr<USceneComponent> RootBullet;
	
	//碰撞盒, 不要将其设置为RootComponent，否则会导致无法调整大小
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> BoxDamage; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
};
