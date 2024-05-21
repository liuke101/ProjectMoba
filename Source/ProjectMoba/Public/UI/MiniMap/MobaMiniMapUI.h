// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "MobaMiniMapUI.generated.h"

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
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCanvasPanel> MiniMapPanel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> MiniMapImage;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|MiniMap")
	TSubclassOf<ASceneCapture2D> SceneCapture2DClass;

	UPROPERTY(EditDefaultsOnly, Category = "Moba|MiniMap")
	TScriptInterface<UMaterialInterface> MiniMapMaterial;

	
	FVector2D MapSize;
};
