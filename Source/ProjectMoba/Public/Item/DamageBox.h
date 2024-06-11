// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageBox.generated.h"

struct FSlotAttribute;
class UBoxComponent;

UCLASS()
class PROJECTMOBA_API ADamageBox : public AActor
{
	GENERATED_BODY()

public:
	ADamageBox();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
protected:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
public:
	FORCEINLINE void SetRangeCheck(bool bCheck) { bSingleCheck = bCheck; }

	/** 获取命中特效 */
	UParticleSystem* GetHitVFX() const;

	void SetBoxSize(const FVector& Size);
	void SetRelativePositon(const FVector& Position);

	FORCEINLINE void SetSingleTarget(bool bSingle) { bSingleTarget = bSingle; }
	FORCEINLINE void SetSingleCheck(bool bCheck) { bSingleCheck = bCheck; }

	void SetOpenFireActive(bool bActive) const;

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_EndOpenFireVFX();

	FORCEINLINE void SetSlotAttribute(const FSlotAttribute* InSlotAttribute) { SlotAttribute = InSlotAttribute; }
	
protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> Root;
	
	//碰撞盒, 不要将其设置为RootComponent，否则会导致无法调整大小
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Moba|DamageBox")
	TObjectPtr<UBoxComponent> DamageBox;

	//开火点特效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|DamageBox")
	TObjectPtr<UParticleSystemComponent> OpenFirePointVFX;

	//开火子弹特效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|DamageBox")
	TObjectPtr<UParticleSystemComponent> OpenFireBullet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|DamageBox")
	TArray<TObjectPtr<UParticleSystem>> HitVFXs;

	bool bSingleCheck = false; //如果为true则为单体检测，否则为范围检测
	bool bSingleTarget = true; //击中目标就销毁

	const FSlotAttribute* SlotAttribute = nullptr;
};
