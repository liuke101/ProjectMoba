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
#include "ProjectMoba/MobaType.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

static int32 InventorySlotKeyIndex = 0;

void UUI_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	//绑定输入
	if(AMobaPlayerController* MobaPlayerController = Cast<AMobaPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(MobaPlayerController->InputComponent))
		{
			UInputAction* SkillAction = nullptr;
			
			ESlotKey SkillKey = static_cast<ESlotKey>(InventorySlotKeyIndex + 4);  //从 Enum 第4个元素开始
			switch (SkillKey)
			{
			case ESlotKey::ESK_Space:
				SkillAction = MobaPlayerController->Space_Action;
				SetKeyName("Space"); //设置SlotKey
				break;
			case ESlotKey::ESK_Q:
				SkillAction = MobaPlayerController->Q_Action;
				SetKeyName("Q");
				break;
			case ESlotKey::ESK_1:
				SkillAction = MobaPlayerController->One_Action;
				SetKeyName("1");
				break;
			case ESlotKey::ESK_2:
				SkillAction = MobaPlayerController->Two_Action;
				SetKeyName("2");
				break;
			case ESlotKey::ESK_3:
				SkillAction = MobaPlayerController->Three_Action;
				SetKeyName("3");
				break;
			case ESlotKey::ESK_4:
				SkillAction = MobaPlayerController->Four_Action;
				SetKeyName("4");
				break;
			default:
				break;
			}
			
			EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Started, this, &UUI_InventorySlot::OnClickedWidget);
			EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Completed, this, &UUI_InventorySlot::OnReleasedWidget);
		}
	}
	
	// 0-5 共六个键位
	InventorySlotKeyIndex++;
	if (InventorySlotKeyIndex > 5)
	{
		InventorySlotKeyIndex = 0;
	}
}

void UUI_InventorySlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!bDrag)
	{
		Super::NativeTick(MyGeometry, InDeltaTime);
	}
}

void UUI_InventorySlot::OnClickedWidget()
{
	//左键点击使用物品
	//如果当前Slot有物品, 且不在CD中
	if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
	{
		if(MobaPlayerState->IsValidInventorySlot(GetSlotID()) && MobaPlayerState->IsCDValid(GetSlotID()))
		{
			//通知服务器使用物品
			MobaPlayerState->Server_Use(GetSlotID());
		}
	}
}

void UUI_InventorySlot::UpdateNumber() const
{
	if(const FSlotData* SlotData = GetMobaPlayerState()->GetInventorySlotData(GetSlotID()))
	{
		SetTextNumber(SlotNumber, SlotData->Number);
	}
}

void UUI_InventorySlot::UpdateSlot()
{
	Super::UpdateSlot();
	bDrag = false;

	UpdateNumber();
}

void UUI_InventorySlot::StartUpdateCD()
{
	Super::StartUpdateCD();
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

int32 UUI_InventorySlot::GetSlotNumber() const
{
	if(const FSlotData* SlotData = GetMobaPlayerState()->GetInventorySlotData(GetSlotID()))
	{
		return SlotData->Number;
	}

	return INDEX_NONE;
}

UWidget * UUI_InventorySlot::GetInventoryTip()
{
	return nullptr;
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
			// 只有Slot不为空时才能拖拽
			if(GetSlotID() != INDEX_NONE)
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
				if(TargetSlotData->DataID == INDEX_NONE)
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