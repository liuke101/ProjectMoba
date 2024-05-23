// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectMoba/MobaType.h"
#include "PlayerDataComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTMOBA_API UPlayerDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerDataComponent();

protected:
	virtual void BeginPlay() override;
	
	/** 复制 */
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 只需要同步一次: COND_InitialOnly
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|PlayerData")
	FName PlayerName = "DefaultName";

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|PlayerData")
	ETeamType TeamType = ETeamType::ETT_Neutral;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|PlayerData")
	int64 PlayerID; //来自DB服务器
	
};
