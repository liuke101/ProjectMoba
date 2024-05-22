// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Table/Config/ClientGlobalConfig.h"
#include "MobaHUD.generated.h"

class UDataTable;
class UMobaMainScreenUI;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API AMobaHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMobaHUD();

	virtual void BeginPlay() override;

public:
	TArray<FClientGlobalConfig*>* GetClientGlobalConfigs();
	const FClientGlobalConfig* GetClientGlobalConfig();
private:
	UPROPERTY(EditDefaultsOnly, Category = "Moba|UI")
	TSubclassOf<UMobaMainScreenUI> MainScreenClass;
	
	TObjectPtr<UMobaMainScreenUI> UI_MainScreen;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|Config")
	TObjectPtr<UDataTable> DT_ClientGlobalConfig;

	TArray<FClientGlobalConfig*> CacheClientGlobalConfigs;
};
