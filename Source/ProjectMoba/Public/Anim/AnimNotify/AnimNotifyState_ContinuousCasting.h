// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ContinuousCasting.generated.h"

class AMobaHeroCharacter;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UAnimNotifyState_ContinuousCasting : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	
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
