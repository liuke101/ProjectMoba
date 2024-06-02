// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaUIBase.h"
#include "Components/PanelWidget.h"
#include "UI_Info.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_Info : public UMobaUIBase
{
	GENERATED_BODY()

public:
	// 初始化Slot布局
	virtual void InitSlotLayout();
	
	virtual UPanelWidget* GetSlotLayoutPanel();
protected:
	/** 呼叫所有的Slot */
	template<class T>
	void CallAllSlot(TFunction<bool(T*)> InventorySlotLamada);
};

template <class T>
void UUI_Info::CallAllSlot(TFunction<bool(T*)> InventorySlotLamada)
{
	if (GetSlotLayoutPanel())
	{
		for (auto& TmpSlot : GetSlotLayoutPanel()->GetAllChildren())
		{
			if (T* InventorySlot = Cast<T>(TmpSlot))
			{
				//如果Lamada返回false，停止Call
				if (!InventorySlotLamada(InventorySlot))
				{
					break; 
				}
			}
		}
	}
}

