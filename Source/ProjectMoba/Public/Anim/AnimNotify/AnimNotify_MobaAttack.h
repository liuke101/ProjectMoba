// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_MobaAttack.generated.h"

class ADamageBox;
class ABullet;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UAnimNotify_MobaAttack : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_MobaAttack();

	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify",meta = (ExposeOnSpawn = true))
	TSubclassOf<ADamageBox> DamageBoxClass;

	//编辑器无法获取FirePoint组件位置, Runtime可以获取。为了表现一致，编辑器通过 Socket 获取位置
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	FName SocketName = TEXT("OpenFire");

	//是否为范围检测
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	bool bSingleCheck = false;

	//子弹生命周期
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	float LifeSpan = 4.0f;
};
