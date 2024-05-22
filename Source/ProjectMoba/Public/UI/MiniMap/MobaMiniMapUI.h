// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "MobaMiniMapUI.generated.h"

class UCanvasPanelSlot;
class ASceneCapture2D;
class UImage;
class UCanvasPanel;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UMobaMiniMapUI : public UMobaUIBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	bool IsExistence(const int64 InID);
	void ResetLocation(UCanvasPanelSlot* PanelSlot, const FVector2D& MinMapPos, const FVector2D& LocalSize2D);

protected:
	// 玩家头像
	TMap<int64, TWeakObjectPtr<UCanvasPanelSlot>> CharacterID_To_PanelSlot;
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> MiniMapPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> MiniMapImage;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|MiniMap")
	TSubclassOf<ASceneCapture2D> SceneCapture2DClass;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|MiniMap")
	TScriptInterface<UMaterialInterface> MiniMapMaterial;
	
	FVector2D MapSize; // 地图大小
};
