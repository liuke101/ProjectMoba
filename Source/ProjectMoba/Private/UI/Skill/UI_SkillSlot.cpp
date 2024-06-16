// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Skill/UI_SkillSlot.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/DragDropOperation.h"
#include "Character/MobaPlayerController.h"
#include "Common/MethodUnit.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Game/MobaGameState.h"
#include "Game/MobaPlayerState.h"
#include "ProjectMoba/MobaType.h"
#include "Table/CharacterAsset.h"
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

void UUI_SkillSlot::UpdateSlot()
{
	Super::UpdateSlot();

	InitSkillCache(); //初始化
}

UUI_Tip* UUI_SkillSlot::GetTip()
{
	if(MobaPlayerState)
	{
		if(VerifyTip(MobaPlayerState, Super::GetTip(), GetSlotID()))
		{
			return Tip;
		}
	}

	return nullptr;
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

void UUI_SkillSlot::InitSkillCache()
{
	if(MobaPlayerState)
	{
		int32 SkillDataID = MobaPlayerState->GetSkillDataIDFromSlotID(GetSlotID());
		if(SkillDataID == INDEX_NONE)
		{
			return;
		}

		if(MobaGameState)
		{
			if(const FCharacterAsset* CharacterAsset = MobaGameState->GetCharacterAssetFromSkillDataID(SkillDataID))
			{
				//lambda
				auto RegisterData = [&](const FCharacterSkill& Skill)
				{
					if(FSlotAttribute* SlotAttribute = MobaPlayerState->GetSlotAttributeFromDataID(Skill.DataID))
					{
						CacheSlotAttribute = *SlotAttribute;

						//注册属性 addlevel buff activeskill
						MethodUnit::RegisterSlotAttribute(MobaPlayerState, &CacheSlotAttribute);
					}
				};
				
				if(CharacterAsset->W_Skill.DataID == SkillDataID)
				{
					RegisterData(CharacterAsset->W_Skill);
				}
				else if(CharacterAsset->E_Skill.DataID == SkillDataID)
				{
					RegisterData(CharacterAsset->E_Skill);
				}
				else if(CharacterAsset->R_Skill.DataID == SkillDataID)
				{
					RegisterData(CharacterAsset->R_Skill);
				}
				else if(CharacterAsset->F_Skill.DataID == SkillDataID)
				{
					RegisterData(CharacterAsset->F_Skill);
				}
			}
		}
	}
	
}

void UUI_SkillSlot::CacheSlotAttributeUpdateLevel()
{
	//因为初始化后拷贝的Cache不再随着原SlotAttribute的变化而变化，所以需要手动更新
	CacheSlotAttribute.UpdateLevel();
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
