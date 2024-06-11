// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MobaUIBase.h"

#include "ThreadManage.h"
#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Game/MobaPlayerState.h"
#include "Game/MobaGameState.h"
#include "UI/MobaHUD.h"

void UMobaUIBase::NativeConstruct()
{
	Super::NativeConstruct();

	InitMobaGameState();
	InitMobaPlayerState();
	
	BindDelegate();
}

AMobaHUD* UMobaUIBase::GetMobaHUD() const
{
	return GetWorld()->GetFirstPlayerController()->GetHUD<AMobaHUD>();
}

void UMobaUIBase::InitMobaPlayerState()
{
	MobaPlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<AMobaPlayerState>();

	//保证获取
	if(MobaPlayerState == nullptr)
	{
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			InitMobaPlayerState();
		});	
	}
}

void UMobaUIBase::InitMobaGameState() 
{
	MobaGameState = GetWorld()->GetGameState<AMobaGameState>();
	
	//保证获取
	if(MobaGameState == nullptr)
	{
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			InitMobaGameState();
		});	
	}
}

UWidgetAnimation* UMobaUIBase::GetNameWidgetAnimation(const FString& WidgetAnimationName) const
{
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
	{
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;
		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate([&](const UWidgetAnimation* OurAnimation) {return OurAnimation->GetFName().ToString() == (WidgetAnimationName + FString("_INST")); });
		return *MyTempAnimation;
	}

	return nullptr;
}

void UMobaUIBase::BindDelegate()
{
}

void UMobaUIBase::RemoveDelegate()
{
}
