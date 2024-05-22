﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MobaUIBase.generated.h"

class AMobaHUD;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UMobaUIBase : public UUserWidget
{
	GENERATED_BODY()

public:
	AMobaHUD* GetMobaHUD() const;
};
