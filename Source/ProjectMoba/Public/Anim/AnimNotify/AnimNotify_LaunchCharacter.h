// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_LaunchCharacter.generated.h"

/**
 * 角色前冲
 */
UCLASS()
class PROJECTMOBA_API UAnimNotify_LaunchCharacter : public UAnimNotify
{
	GENERATED_BODY()
public:

	UAnimNotify_LaunchCharacter();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	float LaunchDistance = 200.0f;
};
