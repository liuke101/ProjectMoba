// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageBox.generated.h"

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
	
public:
	FORCEINLINE void SetRangeCheck(bool bCheck) { bSingleCheck = bCheck; }
	
protected:
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool bSingleCheck = false; //如果为true则为单体检测，否则为范围检测
protected:
	UPROPERTY()
	TObjectPtr<USceneComponent> Root;
	
	//碰撞盒, 不要将其设置为RootComponent，否则会导致无法调整大小
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "DamageBox", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> DamageBox; 
};
