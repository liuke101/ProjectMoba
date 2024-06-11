// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_CD.h"
#include "UI_SlotBase.h"
#include "UI_Slot.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UMaterialInstanceDynamic;

/**
 *  Slot基类
 */
UCLASS()
class PROJECTMOBA_API UUI_Slot : public UUI_CD
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// 隐藏Slot
	virtual void ResetSlot() override;
	// 更新Slot
	virtual void UpdateSlot();
	// 开始进入更新CD的状态
	virtual void StartUpdateCD();
	//结束更新CD
	virtual void EndUpdateCD();

#pragma region UI组件
	// 键位设置
	void SetKeyName(const FString& NewKeyName) const;
	
	// 绘制CD文本
	void DrawSlotCDText(float CD) const;

	//点击事件回调
	UFUNCTION(BlueprintCallable)
	virtual void OnClickedWidget();

	UFUNCTION(BlueprintCallable)
	virtual void OnReleasedWidget();
#pragma endregion
	
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotCDValue;
	
	//键位
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotKey;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ClickButton;
};
