// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MobaHUD.generated.h"

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

private:
	UPROPERTY(EditDefaultsOnly, Category = "Moba|UI")
	TSubclassOf<UMobaMainScreenUI> MainScreenClass;
	
	TObjectPtr<UMobaMainScreenUI> UI_MainScreen;
};
