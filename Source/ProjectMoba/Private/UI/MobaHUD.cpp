// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MobaHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/MobaMainScreenUI.h"

AMobaHUD::AMobaHUD()
{
}

void AMobaHUD::BeginPlay()
{
	Super::BeginPlay();

	checkf(MainScreenClass, TEXT("请在 BP_MobaHUD 中设置 MainScreenClass"));
	
	if(MainScreenClass)
	{
		UI_MainScreen = CreateWidget<UMobaMainScreenUI>(GetWorld(), MainScreenClass);
		UI_MainScreen->AddToPlayerScreen(2);
	}
}
