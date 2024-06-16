// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Skill/UI_SkillSlot.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/DragDropOperation.h"
#include "Character/MobaPlayerController.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Game/MobaPlayerState.h"
#include "ProjectMoba/MobaType.h"
#include "UI/Tip/UI_Tip.h"


static int32 SkillSlotKeyIndex = 0;

void UUI_SkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	//初始隐藏升级按钮
	UpdateLevelButton->SetVisibility(ESlateVisibility::Hidden);
	//关闭技能按钮
	GetClickButton()->SetIsEnabled(false);

	//绑定委托
	UpdateLevelButton->OnClicked.AddDynamic(this, &UUI_SkillSlot::OnClickedUpdateLevelButton);
	
	//绑定输入
	if(AMobaPlayerController* MobaPlayerController = Cast<AMobaPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(MobaPlayerController->InputComponent))
		{
			
			UInputAction* SkillAction = nullptr;
			
			ESlotKey SkillKey = static_cast<ESlotKey>(SkillSlotKeyIndex); 
			switch (SkillKey)
			{
				case ESlotKey::ESK_W:
					SkillAction = MobaPlayerController->W_Action;
					SetKeyName("W"); //设置SlotKey
					break;
				case ESlotKey::ESK_E:
					SkillAction = MobaPlayerController->E_Action;
					SetKeyName("E");
					break;	
				case ESlotKey::ESK_R:
					SkillAction = MobaPlayerController->R_Action;
					SetKeyName("R");
					break;
				case ESlotKey::ESK_F:
					SkillAction = MobaPlayerController->F_Action;
					SetKeyName("F");
					break;
				default:
					break;
			}
			
			EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Started, this, &UUI_SkillSlot::OnClickedWidget);
			EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Completed, this, &UUI_SkillSlot::OnReleasedWidget);
		}
	}

	//0-3 共四个键位
	SkillSlotKeyIndex++;
	if (SkillSlotKeyIndex > 3)
	{
		SkillSlotKeyIndex = 0;
	}
}

void UUI_SkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_SkillSlot::OnClickedWidget()
{
	if(GetClickButton()->GetIsEnabled())
	{
		//如果当前Slot不在CD中
		if(MobaPlayerState)
		{
			if(MobaPlayerState->IsCDValid(GetSlotID()))
			{
				//通知服务器使用技能
				MobaPlayerState->Server_Use(GetSlotID());
			}
		}
	}
}

void UUI_SkillSlot::OnReleasedWidget()
{
	if(MobaPlayerState)
	{
		MobaPlayerState->Server_ReleaseSkillKey();
	}
}


void UUI_SkillSlot::BindToolTip()
{
	GetClickButton()->SetToolTip(GetTip());
	GetClickButton()->SetCursor(EMouseCursor::Hand);
}

void UUI_SkillSlot::OnClickedUpdateLevelButton()
{
	if(UpdateLevelButton)
	{
		if(MobaPlayerState)
		{
			MobaPlayerState->Server_UpdateSkillLevel(GetSlotID());
		}
	}

	//BUG:第二次点击升级按钮，没有反应
}

bool UUI_SkillSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                 UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	bool bDrop = false;
	// 当把其他Slot拖拽到SkillSlot时，更新被拖拽的Slot, 即拖拽失败重新显示图标
	if (UDragDropOperation* InDragDropOperation = Cast<UDragDropOperation>(InOperation))
	{
		if (UUI_Slot* PayloadSlot = Cast<UUI_Slot>(InDragDropOperation->Payload))
		{
			PayloadSlot->UpdateSlot();

			bDrop = true;
		}
	}

	return bDrop;
}
