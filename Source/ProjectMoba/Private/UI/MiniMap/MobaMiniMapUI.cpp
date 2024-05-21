// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MiniMap/MobaMiniMapUI.h"

#include "Common/RenderingUtils.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Engine/SceneCapture2D.h"

void UMobaMiniMapUI::NativeConstruct()
{
	Super::NativeConstruct();

	if(ASceneCapture2D* SceneCapture2D = RenderingUtils::SpawnSceneCapture2D(GetWorld(), SceneCapture2DClass, MapSize, 0.03f))
    {
		//MiniMapImage->SetBrushFromMaterial((MiniMapMaterial.GetObject()));   
    }

	if(MiniMapImage)
	{
		if(UCanvasPanelSlot* ImagePanelSlot = Cast<UCanvasPanelSlot>(MiniMapImage->Slot))
		{
			ImagePanelSlot->SetSize(FVector2d(1024,1024));
		}
	}
}

void UMobaMiniMapUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}
