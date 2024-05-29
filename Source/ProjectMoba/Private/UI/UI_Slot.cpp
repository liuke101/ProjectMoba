// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_Slot.h"

#include "Materials/MaterialInstanceDynamic.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Game/MobaPlayerState.h"

void UUI_Slot::NativeConstruct()
{
	Super::NativeConstruct();

	ClickButton->OnClicked.AddDynamic(this, &UUI_Slot::OnLeftClickedWidget);

	CDMaterialDynamic = UMaterialInstanceDynamic::Create(CDMaterialParent, this); //创建动态材质实例
	SlotCD->SetBrushFromMaterial(CDMaterialDynamic); //设置图片为动态材质
}

void UUI_Slot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//客户端更新CD
	if(FSlotData* SlotData = GetMobaPlayerState()->GetInventorySlotData(GetSlotID()))
	{
		if(SlotData->CD > 0.0f)
		{
			SlotData->CD -= InDeltaTime;
			DrawSlotCDMat(SlotData->CD / BuildSlot.MaxCD); //注意这里要除MaxCD
			DrawSlotCDText(SlotData->CD);
			
			if(SlotData->CD <= 0.0f)
			{
				DrawSlotCDMat(0.0f);
				DrawSlotCDText(0.0f);
			}
		}
	}
}

void UUI_Slot::ResetSlot()
{
	SlotIcon->SetVisibility(ESlateVisibility::Hidden);
	SlotCD->SetVisibility(ESlateVisibility::Hidden);
	SlotCDValue->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_Slot::UpdateSlot()
{
	if(FSlotData* SlotData = GetMobaPlayerState()->GetInventorySlotData(GetSlotID()))
	{
		if(SlotData->SlotID != INDEX_NONE)
		{
			UpdateIcon(SlotData->SlotIcon);
			DrawSlotCDMat(SlotData->CD);
			DrawSlotCDText(SlotData->CD);
		}
		else if(SlotData->SlotID == INDEX_NONE)
		{
			ResetSlot(); //交换时，清空Slot
		}
	}
}

void UUI_Slot::StartUpdateCD()
{
	if(FSlotData* SlotData = GetMobaPlayerState()->GetInventorySlotData(GetSlotID()))
	{
		BuildSlot.MaxCD = SlotData->CD;
	}
}

void UUI_Slot::EndUpdateCD()
{
	if(FSlotData* SlotData = GetMobaPlayerState()->GetInventorySlotData(GetSlotID()))
	{
		UpdateIcon(SlotData->SlotIcon);
		DrawSlotCDMat(SlotData->CD);
		DrawSlotCDText(SlotData->CD);
	}
}

void UUI_Slot::SetKeyName(const FString& NewKeyName) const
{
	SlotKey->SetText(FText::FromString(NewKeyName));
}

void UUI_Slot::DrawSlotCDMat(float InSlotCD) const
{
	if (CDMaterialDynamic)
	{
		if (InSlotCD > 0.0f && InSlotCD < 1.0f)
		{
			CDMaterialDynamic->SetScalarParameterValue(SlotClearValueName, true);
			SlotCD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			CDMaterialDynamic->SetScalarParameterValue(SlotClearValueName, false);
			SlotCD->SetVisibility(ESlateVisibility::Hidden);
		}

		CDMaterialDynamic->SetScalarParameterValue(SlotMatCDName, InSlotCD);
	}
}

void UUI_Slot::DrawSlotCDText(float InSlotCD) const
{
	DisplayNumber(SlotCDValue, InSlotCD);
}

void UUI_Slot::UpdateIcon(UTexture2D* InIcon) const
{
	if (SlotIcon)
	{
		if (InIcon)
		{
			SlotIcon->SetBrushFromTexture(InIcon);
			SlotIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			SlotIcon->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UUI_Slot::OnLeftClickedWidget()
{
	
}

void UUI_Slot::DisplayNumber(UTextBlock* TextNumberBlock, float TextNumber)
{
	if (TextNumber <= 0.f)
	{
		TextNumberBlock->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (TextNumber > 0.f && TextNumber <= 1.0f)
		{
			TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%.1f"), TextNumber)));
		}
		else
		{
			TextNumberBlock->SetText(FText::FromString(FString::Printf(TEXT("%02d"), static_cast<int32>(TextNumber))));
		}
		
		TextNumberBlock->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}

void UUI_Slot::DisplayNumber(UTextBlock* TextNumberBlock, int32 TextNumber)
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
