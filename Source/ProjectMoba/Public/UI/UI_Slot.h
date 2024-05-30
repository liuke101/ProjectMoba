// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_Slot.generated.h"

class UImage;
class UTextBlock;
class UButton;
class UMaterialInstanceDynamic;

/**
 *  Slot基类
 */
UCLASS()
class PROJECTMOBA_API UUI_Slot : public UMobaUIBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// 隐藏Slot
	virtual void ResetSlot();
	// 更新Slot
	virtual void UpdateSlot();
	// 开始进入更新CD的状态
	virtual void StartUpdateCD();
	//结束更新CD
	virtual void EndUpdateCD();

	//设置唯一ID (注：该类中的SlotID指的是InventoryID,即背包格子的ID，而不是用于获取DataTable数据的SlotID, 命名有些冲突，后期应该修改并区分）
	FORCEINLINE void SetSlotID(const int32 InID) { BuildSlot.SlotID = InID; }
	FORCEINLINE int32 GetSlotID() const { return BuildSlot.SlotID; }

#pragma region UI组件
	// 键位设置
	void SetKeyName(const FString& NewKeyName) const;
	// 绘制CD材质
	void DrawSlotCDMat(float CD) const;
	// 绘制CD文本
	void DrawSlotCDText(float CD) const;

	/// 更新Icon
	/// @param IconTexture 如果为nullptr则隐藏Icon，否则显示Icon
	void UpdateIcon(UTexture2D* IconTexture) const;

	//点击事件回调
	UFUNCTION(BlueprintCallable)
	virtual void OnClickedWidget();

protected:
	//显示 float字体
	static void SetTextNumber(UTextBlock* TextNumberBlock, float TextNumber);
	//显示 int字体
	static void SetTextNumber(UTextBlock* TextNumberBlock, int32 TextNumber);
#pragma endregion
	
protected:
	//Slot数据结构
	struct FBuildSlot
	{
		FBuildSlot()
			:MaxCD(1.f)
			,SlotID(INDEX_NONE)
		{}
		float MaxCD;
		int32 SlotID;
	} BuildSlot;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SlotIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SlotCD;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotCDValue;
	
	//键位
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotKey;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ClickButton;

	//CD动态材质
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> CDMaterialDynamic;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TObjectPtr<UMaterialInterface> CDMaterialParent;
	
	//CD动态材质 参数名
	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName SlotMatCDName;
	
	//CD动态材质 参数名
	UPROPERTY(EditDefaultsOnly, Category = UI)
	FName SlotClearValueName;
};
