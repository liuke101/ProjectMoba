// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UI_CD.h"

#include "Components/Image.h"

void UUI_CD::NativeConstruct()
{
	Super::NativeConstruct();

	CDMaterialDynamic = UMaterialInstanceDynamic::Create(CDMaterialParent, this); //创建动态材质实例
	SlotCD->SetBrushFromMaterial(CDMaterialDynamic); //设置图片为动态材质
}

void UUI_CD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}


void UUI_CD::ResetSlot()
{
	Super::ResetSlot();
	
	SlotCD->SetVisibility(ESlateVisibility::Hidden);
}

void UUI_CD::DrawSlotCDMat(float CD) const
{
	if (CDMaterialDynamic)
	{
		if (CD > 0.0f && CD < 1.0f)
		{
			CDMaterialDynamic->SetScalarParameterValue(SlotClearValueName, true);
			SlotCD->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
		{
			CDMaterialDynamic->SetScalarParameterValue(SlotClearValueName, false);
			SlotCD->SetVisibility(ESlateVisibility::Hidden);
		}

		CDMaterialDynamic->SetScalarParameterValue(SlotMatCDName, CD);
	}
}
