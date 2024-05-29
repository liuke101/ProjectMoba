// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/UI_InventorySlot.h"

#include "EnhancedInputComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/Inventory/DragDrop/UI_IconDragDrop.h"
#include "Blueprint/DragDropOperation.h"
#include "Character/MobaPlayerController.h"
#include "Game/MobaPlayerState.h"
#include "ProjectMoba/MiscData.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

static int32 InventoryNumber = 1;

UUI_InventorySlot::UUI_InventorySlot()
{
}

void UUI_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	const FString InventoryNumberString = FString::Printf(TEXT("InventoryKey_%i"), InventoryNumber);
	//GetWorld()->GetFirstPlayerController()->InputComponent->BindAction(*InventoryNumberString, IE_Pressed, this, &UUI_InventorySlot::OnClickedWidget);
	//改为增强输入：
	if(AMobaPlayerController* MobaPlayerController = Cast<AMobaPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(MobaPlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(MobaPlayerController->LeftClick_Action, ETriggerEvent::Started, this, &UUI_InventorySlot::OnLeftClickedWidget);
		}
	}
	
	SetKeyName(FString::FromInt(InventoryNumber));

	InventoryNumber++;
	if (InventoryNumber > 6)
	{
		InventoryNumber = 1;
	}
}

void UUI_InventorySlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!bDrag)
	{
		Super::NativeTick(MyGeometry, InDeltaTime);
	}
}

int32 UUI_InventorySlot::GetSlotNumber() const
{
	if(FSlotData* SlotData = GetMobaPlayerState()->GetInventorySlotData(GetSlotID()))
	{
		return SlotData->Number;
	}

	return INDEX_NONE;
}

void UUI_InventorySlot::OnLeftClickedWidget()
{
	//左键点击使用物品
	//如果当前Slot有物品
	if(GetMobaPlayerState()->IsValidInventorySlot(GetSlotID()))
	{
		//检查CD是否空闲
		if(GetMobaPlayerState()->IsCDValid(GetSlotID()))
		{
			//通知服务器使用物品
			GetMobaPlayerState()->Server_Use(GetSlotID());
		}
	}
}

UWidget * UUI_InventorySlot::GetInventoryTip()
{
	return nullptr;
}

void UUI_InventorySlot::UpdateSlot()
{
	Super::UpdateSlot();
	bDrag = false;

	if(FSlotData* SlotData = GetMobaPlayerState()->GetInventorySlotData(GetSlotID()))
	{
		UpdateNumber();
	}
}

void UUI_InventorySlot::StartUpdateCD()
{
	Super::StartUpdateCD();
}

void UUI_InventorySlot::UpdateNumber()
{
	if(FSlotData* SlotData = GetMobaPlayerState()->GetInventorySlotData(GetSlotID()))
	{
		DisplayNumber(SlotNumber, SlotData->Number);
	}
}

void UUI_InventorySlot::EndUpdateCD()
{
	Super::EndUpdateCD();

	UpdateNumber();
}

void UUI_InventorySlot::ResetSlot()
{
	Super::ResetSlot();

	SlotNumber->SetVisibility(ESlateVisibility::Hidden);
}

FReply UUI_InventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	// 鼠标右键触发
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{
		FReply Reply = FReply::Handled();
		TSharedPtr<SWidget> SlateWidgetDrag = GetCachedWidget();
		if (SlateWidgetDrag.IsValid())
		{
			Reply.DetectDrag(SlateWidgetDrag.ToSharedRef(), EKeys::RightMouseButton);
			return Reply;
		}
	}

	return FReply::Handled();
}

void UUI_InventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (IconDragDrogClass)
	{
		if(FSlotData* SlotData = GetMobaPlayerState()->GetInventorySlotData(GetSlotID()))
		{
			if (UUI_IconDragDrop* IconDragDrop = CreateWidget<UUI_IconDragDrop>(GetWorld(), IconDragDrogClass))
			{
				if (UDragDropOperation* InDropOperation = NewObject<UDragDropOperation>(GetTransientPackage(), UDragDropOperation::StaticClass()))
				{
					InDropOperation->SetFlags(RF_StrongRefOnFrame);
					IconDragDrop->DrawIcon(SlotData->SlotIcon);
					InDropOperation->DefaultDragVisual = IconDragDrop;
					InDropOperation->Payload = this; //设置当前拖拽的Slot作为Payload
					OutOperation = InDropOperation;

					ResetSlot();//隐藏自己

					bDrag = true;
				}
			}		
		}
	}

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UUI_InventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	bool bDrop = false;

	if (const UDragDropOperation* InDragDropOperation = Cast<UDragDropOperation>(InOperation))
	{
		if (UUI_InventorySlot* DraggedInventorySlot = Cast<UUI_InventorySlot>(InDragDropOperation->Payload)) //获取正在被拖拽的Slot
		{
			const FSlotData* TargetSlotData = GetMobaPlayerState()->GetInventorySlotData(GetSlotID()); // 准备放置的SlotData
			const FSlotData* DraggedSlotData = GetMobaPlayerState()->GetInventorySlotData(DraggedInventorySlot->GetSlotID()); //正在被拖拽的SlotData
			if(TargetSlotData && DraggedSlotData)
			{
				//如果目标Slot为空位置，直接移动
				if(TargetSlotData->SlotID == INDEX_NONE)
				{
					// 其他数据由服务器处理，客户端只关心和表现相关的数据
					BuildSlot.MaxCD = DraggedInventorySlot->BuildSlot.MaxCD;
				}
				else //如果目标Slot不为空，交换位置
				{
					// float TempCD = DraggedInventorySlot->BuildSlot.MaxCD;
					// BuildSlot.MaxCD = DraggedInventorySlot->BuildSlot.MaxCD;
					// DraggedInventorySlot->BuildSlot.MaxCD = TempCD;
					Swap(DraggedInventorySlot->BuildSlot.MaxCD,BuildSlot.MaxCD);
				}

				//通知服务器更新数据
				GetMobaPlayerState()->Server_UpdateInventory(DraggedInventorySlot->GetSlotID(), GetSlotID());
			}
			bDrop = true;
		}	
	}
	
	return bDrop;
}

void UUI_InventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

}

void UUI_InventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{

}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif