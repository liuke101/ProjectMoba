// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MobaUIBase.h"

#include "Animation/WidgetAnimation.h"
#include "Blueprint/WidgetBlueprintGeneratedClass.h"
#include "Game/MobaPlayerState.h"
#include "Game/MobaGameState.h"
#include "UI/MobaHUD.h"

void UMobaUIBase::NativeConstruct()
{
	Super::NativeConstruct();

	BindDelegate();
}

AMobaHUD* UMobaUIBase::GetMobaHUD() const
{
	return GetWorld()->GetFirstPlayerController()->GetHUD<AMobaHUD>();
}

AMobaPlayerState* UMobaUIBase::GetMobaPlayerState() const
{
	return GetWorld()->GetFirstPlayerController()->GetPlayerState<AMobaPlayerState>();
}

AMobaGameState* UMobaUIBase::GetMobaGameState() const
{
	return GetWorld()->GetGameState<AMobaGameState>();
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
