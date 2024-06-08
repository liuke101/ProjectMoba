// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaUIBase.h"
#include "UI_Info.h"
#include "Components/PanelWidget.h"
#include "UI_Bar.generated.h"

class UUI_Slot;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_Bar : public UUI_Info
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	// 绑定委托
	virtual void BindDelegate() override;
	//行
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|UI")
	int32 Layout_Row = 1;

	//列
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|UI")
	int32 Layout_Col = 1;
};

