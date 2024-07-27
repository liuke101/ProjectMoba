// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaUIBase.h"
#include "UI_SlotBase.generated.h"

class UTextBlock;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_SlotBase : public UMobaUIBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// 隐藏Slot
	virtual void ResetSlot();
	
	//设置唯一ID, SlotID指的是格子的ID，而不是用于获取DataTable数据的dtID
	FORCEINLINE void SetSlotID(const int32 InID) { SlotID = InID; }
	FORCEINLINE int32 GetSlotID() const { return SlotID; }
	
#pragma region UI组件
	/// 更新Icon
	/// @param IconTexture 如果为nullptr则隐藏Icon，否则显示Icon
	void UpdateIcon(UTexture2D* IconTexture) const;

protected:
	//显示 float字体
	static void SetTextNumber(UTextBlock* TextNumberBlock, float TextNumber);
	//显示 int字体
	static void SetTextNumber(UTextBlock* TextNumberBlock, int32 TextNumber);
#pragma endregion
	
protected:
	//Slot数据
	float MaxCD = 1.f;
	int32 SlotID = INDEX_NONE;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SlotIcon;

};
