// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_ContinuousCasting_Begin.generated.h"

/**
 * 在begin蒙太奇中使用，用于转换到loop蒙太奇
 */
UCLASS()
class PROJECTMOBA_API UAnimNotify_ContinuousCasting_Begin : public UAnimNotify
{
	GENERATED_BODY()
public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	UAnimMontage* LoopAnim;
};
