// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphRuntime/Public/AnimNotifies/AnimNotify_PlayMontageNotify.h"
#include "AnimNotifyWindow_ContinuousCasting.generated.h"

class AMobaHeroCharacter;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UAnimNotifyWindow_ContinuousCasting : public UAnimNotify_PlayMontageNotifyWindow
{
	GENERATED_BODY()
public:
	virtual void BranchingPointNotifyBegin(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyEnd(FBranchingPointNotifyPayload& BranchingPointPayload) override;
	virtual void BranchingPointNotifyTick(FBranchingPointNotifyPayload& BranchingPointPayload, float FrameDeltaTime) override;
	
	virtual FString GetNotifyName_Implementation() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	TObjectPtr<UAnimMontage> LoopAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Notify")
	TObjectPtr<UAnimMontage> EndAnim;

	UPROPERTY()
	TObjectPtr<AMobaHeroCharacter> MobaHero;
private:
	bool bEndContinuousAnim = false; //结束施法
};
