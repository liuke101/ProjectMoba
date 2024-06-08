// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameInfo/UI_KillMsg.h"

#include "ThreadManage.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Game/MobaPlayerState.h"

void UUI_KillMsg::NativeConstruct()
{
	Super::NativeConstruct();
	
	SetVisibility(ESlateVisibility::Hidden);
}

void UUI_KillMsg::BindDelegate()
{
	if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
	{
		MobaPlayerState->KillMessageDelegate.BindLambda([&](const FKillNetPackgae& KillNetPackgae)
		{
			switch (KillNetPackgae.KillType)
			{
			case EKillType::EKT_NormalKill:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("击杀"))); //要加TEXT(), 否则乱码
				break;
			case EKillType::EKT_FirstBlood:
				UpdateSlot(KillNetPackgae, FText::FromString(TEXT("敌将讨伐")));
				break;
			case EKillType::EKT_DoubleKill:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("双杀")));
				break;
			case EKillType::EKT_TripleKill:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("三杀")));
				break;
			case EKillType::EKT_QuadraKill:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("四杀")));
				break;
			case EKillType::EKT_PentaKill:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("五杀")));
				break;
			case EKillType::EKT_DaShaTeSha:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("大杀特杀")));
				break;
			case EKillType::EKT_SuoXiangPiMi:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("所向披靡")));
				break;
			case EKillType::EKT_HunShenShiDan:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("浑身是胆")));
				break;
			case EKillType::EKT_YongGuanSanJun:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("勇冠三军")));
				break;
			case EKillType::EKT_YiJiDangQian:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("一骑当千")));
				break;
			case EKillType::EKT_WanFuMoDi:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("万夫莫敌")));
				break;
			case EKillType::EKT_JuShiWuShuang:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("举世无双")));
				break;
			case EKillType::EKT_TianXiaWuDi:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("天下无敌")));
				break;
			case EKillType::EKT_TeamDeath:
				UpdateSlot(KillNetPackgae,FText::FromString(TEXT("团灭")));
				break;
			}
		});
	}
	else
	{
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			BindDelegate();
		});
	}
}



void UUI_KillMsg::UpdateSlot(const FKillNetPackgae& KillNetPackgae, const FText& KillType)
{
	//显示
	SetVisibility(ESlateVisibility::HitTestInvisible);

	//更新UI
	KillTypeText->SetText(KillType); 
	KillerNameText->SetText(FText::FromName(KillNetPackgae.KillerName));
	KillerIcon->SetBrushFromTexture(KillNetPackgae.KillerIcon);
	KilledNameText->SetText(FText::FromName(KillNetPackgae.KilledName));
	KilledIcon->SetBrushFromTexture(KillNetPackgae.KilledIcon);

	//播放UI动画
	if (UWidgetAnimation* InWidgetAnim = GetNameWidgetAnimation(TEXT("KillTargetAnim")))
	{
		PlayAnimation(InWidgetAnim);
	}
}
