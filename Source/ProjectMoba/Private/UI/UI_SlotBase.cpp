// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_SlotBase.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UUI_SlotBase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_SlotBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_SlotBase::ResetSlot()
{
	SlotIcon->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_SlotBase::UpdateIcon(UTexture2D* IconTexture) const
{
	if (SlotIcon)
	{
		if (IconTexture)
		{
			SlotIcon->SetBrushFromTexture(IconTexture);
			SlotIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			SlotIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UUI_SlotBase::SetTextNumber(UTextBlock* TextNumberBlock, float TextNumber)
{
	if (TextNumber <= 0.f)
	{
		TextNumberBlock->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		//0.0~1.0 保留一位小数
		if (TextNumber > 0.f && TextNumber <= 1.0f)
		{
			TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), TextNumber)));
		}
		// >1.0 只保留整数
		else
		{
			TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%02d"), static_cast<int32>(TextNumber))));
		}
		
		TextNumberBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUI_SlotBase::SetTextNumber(UTextBlock* TextNumberBlock, int32 TextNumber)
{
	if (TextNumber < 1)
	{
		TextNumberBlock->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%02d"), TextNumber)));

		TextNumberBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

