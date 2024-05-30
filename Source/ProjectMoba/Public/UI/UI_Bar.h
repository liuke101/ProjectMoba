// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaUIBase.h"
#include "Components/PanelWidget.h"
#include "UI_Bar.generated.h"

class UUI_Slot;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_Bar : public UMobaUIBase
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	// 必须实现
	virtual UPanelWidget* GetSlotLayoutPanel() const;
	
protected:
	// 初始化Slot分布
	virtual void InitSlotLayout() const;
	
	// 绑定委托
	void BindSlotDelegate() ;

	// 呼叫所有的Slot
	template<class T>
	void CallAllSlot(TFunction<bool(T*)> SlotLamada) const;

	//行
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|UI")
	int32 Layout_Row = 1;

	//列
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|UI")
	int32 Layout_Col = 1;
};

template <class T>
void UUI_Bar::CallAllSlot(TFunction<bool(T*)> SlotLamada) const
{
	if(GetSlotLayoutPanel())
	{
		for (const auto& TmpSlot : GetSlotLayoutPanel()->GetAllChildren())
		{
			if (T* InventorySlot = Cast<T>(TmpSlot))
			{
				if (!SlotLamada(InventorySlot)) //如果Lamada返回false，停止Call
				{
					break; 
				}
			}
		}
	}
}
