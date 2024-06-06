// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameInfo/UI_TeamInfo.h"

#include "EnhancedInputComponent.h"
#include "ThreadManage.h"
#include "Character/MobaPlayerController.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Game/MobaPlayerState.h"
#include "UI/GameInfo/UI_PlayersInfo.h"

void UUI_TeamInfo::NativeConstruct()
{
	Super::NativeConstruct();

	SetVisibility(ESlateVisibility::Hidden);

	//绑定输入 Tab开关
	if(AMobaPlayerController* MobaPlayerController = Cast<AMobaPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(MobaPlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(MobaPlayerController->Tab_Action, ETriggerEvent::Started, this, &UUI_TeamInfo::Show);
		}
	}

	GThread::GetCoroutines().BindLambda(0.3f, [&]()
	{
		if(AMobaPlayerState* MobaPlayerState = Cast<AMobaPlayerState>(GetOwningPlayerState()))
		{
			// 绑定委托
			MobaPlayerState->TeamInfoDelegate.BindUObject(this, &UUI_TeamInfo::SpawnPlayerInfo);
		}
	});
	
	
}

void UUI_TeamInfo::SpawnPlayerInfo(bool bEnemy, const int64& InPlayerID, const TArray<int32>& InSlotsID) const
{
	// if (UVerticalBox* InArray = (bEnemy ? EnemyTeam : FriendlyTeam))
	// {
	// 	if (UUI_PlayersInfo* SlotWidget = CreateWidget<UUI_PlayersInfo>(GetWorld(), PlayersInfoClass))
	// 	{
	// 		if (UVerticalBoxSlot* InBoxSlot = InArray->AddChildToVerticalBox(SlotWidget))
	// 		{
	// 			InBoxSlot->SetPadding(1.0f);
	// 			SlotWidget->UpdateSlot(InPlayerID,InSlotsID);
	// 		}
	// 	}
	// }
}

void UUI_TeamInfo::SpawnPlayerInfo(const TArray<FPlayerTeamNetPackage>& PlayerTeamNetPackages) const
{
	for(auto& Tmp : PlayerTeamNetPackages)
	{
		if(UUI_PlayersInfo* PlayerInfo = CreateWidget<UUI_PlayersInfo>(GetWorld(), PlayersInfoClass))
		{
			for(int i = 0; i<PlayerTeamNetPackages.Num();i++)
			{
				UVerticalBoxSlot* GridSlot = nullptr;
				//目前还没区分队伍
				if(i % 2 == 0) //偶数在左列
				{
					GridSlot = EnemyTeam->AddChildToVerticalBox(PlayerInfo);
				}
				else //奇数在右列
				{
					GridSlot = FriendlyTeam->AddChildToVerticalBox(PlayerInfo);
				}
						
				GridSlot->SetPadding(1.0f);
				GridSlot->SetHorizontalAlignment(HAlign_Fill);
				GridSlot->SetVerticalAlignment(VAlign_Fill);

				//更新
				PlayerInfo->UpdateSlot(Tmp);
			}
		}
	}
}

void UUI_TeamInfo::Show()
{
	if(GetVisibility() == ESlateVisibility::Hidden)
	{
		SetVisibility(ESlateVisibility::Visible);

		//清空Slot
		EnemyTeam->ClearChildren();
		FriendlyTeam->ClearChildren();

		//请求队伍信息
		if(AMobaPlayerState* MobaPlayerState = Cast<AMobaPlayerState>(GetOwningPlayerState()))
		{
			MobaPlayerState->Server_RequestAllPlayerTeamInfos();
		}
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}
