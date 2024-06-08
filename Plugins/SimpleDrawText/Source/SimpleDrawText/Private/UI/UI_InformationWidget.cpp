// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "UI/UI_InformationWidget.h"
#include "Components/TextBlock.h"
#include "Animation/WidgetAnimation.h"
#include "SimpleDrawText.h"
#include "Style/SimpleDrawTextStyle.h"
#include "Components/Image.h"

UUI_InformationWidget::UUI_InformationWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	//static ConstructorHelpers::FObjectFinder<UDataTable> DrawTexttyle_Table(*FSimpleDrawTextHelp::GetStylePaths());
	//StyleTable = DrawTexttyle_Table.Object;
}

UWidgetAnimation* UUI_InformationWidget::GetNameWidgetAnimation(const FString& WidgetAnimationName) const
{
	if (UWidgetBlueprintGeneratedClass* WidgetBlueprintGenerated = Cast<UWidgetBlueprintGeneratedClass>(GetClass()))
	{
		TArray<UWidgetAnimation*> TArrayAnimations = WidgetBlueprintGenerated->Animations;
		UWidgetAnimation** MyTempAnimation = TArrayAnimations.FindByPredicate([&](const UWidgetAnimation* OurAnimation) {return OurAnimation->GetFName().ToString() == (WidgetAnimationName + FString("_INST")); });
		return *MyTempAnimation;
	}

	return nullptr;
}

void UUI_InformationWidget::SetTextBlockStyle(
	const FString &InText,
	const FLinearColor &Color, 
	float Percentage, 
	EInfoAnimType Type, 
	int32 PrefixStyleID,
	bool bBackground)
{
	int32 MaxFontSize = 50;
	int32 MinFontSize = 16;
	int32 CurrentFontSize = MinFontSize + (MaxFontSize - MinFontSize) *Percentage;
	TextWidget->SetText(FText::FromString(InText));
	TextWidget->SetColorAndOpacity(FSlateColor(Color));

	//??????
	UWidgetAnimation* InWidgetAnim = NULL;
	switch (Type)
	{
	case EInfoAnimType::TYPE_DEFAULT:
		InWidgetAnim = GetNameWidgetAnimation(TEXT("UpdateFontSize"));
		break;
	case EInfoAnimType::TYPE_GRADUALCHANGE:
		InWidgetAnim = GetNameWidgetAnimation(TEXT("UpdateGradualChange"));
		break;
	case EInfoAnimType::TYPE_CRIT:
		InWidgetAnim = GetNameWidgetAnimation(TEXT("UpdateFontSizeExplosion"));
		break;
	case EInfoAnimType::TYPE_EXPLOSION:
		InWidgetAnim = GetNameWidgetAnimation(TEXT("UpdateExplosion"));
		break;
	case EInfoAnimType::TYPE_GENTLE:
		InWidgetAnim = GetNameWidgetAnimation(TEXT("UpdateGentle"));
		break;
	case EInfoAnimType::TYPE_SMALLER:
		InWidgetAnim = GetNameWidgetAnimation(TEXT("UpdateSmaller"));
		break;
	case EInfoAnimType::TYPE_MAX:
		break;
	}

	if (InWidgetAnim)
	{
		PlayAnimation(InWidgetAnim);
	}

	//??ǰ׺?ͱ???
	const TArray<FSimpleDrawTextStyle*>* DrawTextStyles = GetDrawTextStyle();
	if (DrawTextStyles)
	{
		if (PrefixStyleID != INDEX_NONE)
		{
			for (const auto& Tmp : *DrawTextStyles)
			{
				if (Tmp->ID == PrefixStyleID)
				{
					Icon->SetVisibility(ESlateVisibility::Visible);
					Icon->SetBrushFromTexture(Tmp->Icon);
					Icon->SetDesiredSizeOverride(FVector2D(32.f, 32.f));
				}
			}
		}

		TArray<FSimpleDrawTextStyle*> Arrays;
		if (bBackground)
		{
			for (const auto& Tmp : *DrawTextStyles)
			{
				if (Tmp->ID >= 1440)
				{
					Arrays.Add(Tmp);
				}
			}
		}

		if (Arrays.Num())
		{
			int32 NewIndex = FMath::RandRange(0, Arrays.Num() - 1);

			BackgroundIcon->SetVisibility(ESlateVisibility::Visible);
			BackgroundIcon->SetBrushFromTexture(Arrays[NewIndex]->Icon);
			BackgroundIcon->SetDesiredSizeOverride(FVector2D(156.f, 156.f));
		}
	}
}

TArray<FSimpleDrawTextStyle*> *UUI_InformationWidget::GetDrawTextStyle()
{
	if (!SimpleDrawTextStyleCache.Num())
	{
		if (StyleTable)
		{
			StyleTable->GetAllRows(TEXT("Draw Text Style"), SimpleDrawTextStyleCache);
		}
	}

	return &SimpleDrawTextStyleCache;
}
