// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_SlotBase.h"
#include "UI_CD.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_CD : public UUI_SlotBase
{
	GENERATED_BODY()
protected:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void ResetSlot() override;
public:
	// 绘制CD材质
	void DrawSlotCDMat(float CD) const;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SlotCD;

	//CD动态材质
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> CDMaterialDynamic;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TObjectPtr<UMaterialInterface> CDMaterialParent;
	
	//CD动态材质 参数名
	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName SlotMatCDName = "CDValue";
	
	//CD动态材质 参数名
	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName SlotClearValueName = "ClearCD";
};
