// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Skill/UI_SkillSlot.h"

#include "EnhancedInputComponent.h"
#include "Character/MobaPlayerController.h"
#include "ProjectMoba/MobaType.h"


static int32 SkillKeyIndex = 0;

void UUI_SkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	//绑定输入
	if(AMobaPlayerController* MobaPlayerController = Cast<AMobaPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(MobaPlayerController->InputComponent))
		{
			
			UInputAction* SkillAction = nullptr;
			
			ESkillKey SkillKey = static_cast<ESkillKey>(SkillKeyIndex); 
			switch (SkillKey)
			{
				case ESkillKey::ESK_W:
					SkillAction = MobaPlayerController->W_Action;
					SetKeyName("W"); //设置SlotKey
					break;
				case ESkillKey::ESK_E:
					SkillAction = MobaPlayerController->E_Action;
					SetKeyName("E");
					break;	
				case ESkillKey::ESK_R:
					SkillAction = MobaPlayerController->R_Action;
					SetKeyName("R");
					break;
				case ESkillKey::ESK_F:
					SkillAction = MobaPlayerController->F_Action;
					SetKeyName("F");
					break;
				default:
					break;
			}
			
			EnhancedInputComponent->BindAction(SkillAction, ETriggerEvent::Started, this, &UUI_SkillSlot::OnClickedWidget);
		}
	}

	SkillKeyIndex++;
	if (SkillKeyIndex > 3)
	{
		SkillKeyIndex = 0;
	}
}

void UUI_SkillSlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_SkillSlot::OnClickedWidget()
{
	
}
