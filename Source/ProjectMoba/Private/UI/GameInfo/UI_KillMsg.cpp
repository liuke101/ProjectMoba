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

	GThread::GetCoroutines().BindLambda(0.3f, [&]()
	{
		if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
		{
			MobaPlayerState->KillMessageDelegate.BindLambda([&](const FKillNetPackgae& KillNetPackgae)
			{
				switch (KillNetPackgae.KillType)
				{
				case EKillType::EKT_NormalKill:
					NormalKill(KillNetPackgae,FText::FromString("击杀"));
					break;
				case EKillType::EKT_FirstBlood:
					ContinuousKill(KillNetPackgae, FText::FromString("敌将讨伐"));
					break;
				case EKillType::EKT_DoubleKill:
					ContinuousKill(KillNetPackgae,FText::FromString("双杀"));
					break;
				case EKillType::EKT_TripleKill:
					ContinuousKill(KillNetPackgae,FText::FromString("三杀"));
					break;
				case EKillType::EKT_QuadraKill:
					ContinuousKill(KillNetPackgae,FText::FromString("四杀"));
					break;
				case EKillType::EKT_PentaKill:
					ContinuousKill(KillNetPackgae,FText::FromString("五杀"));
					break;
				case EKillType::EKT_DaShaTeSha:
					AccumulatedKill(KillNetPackgae,FText::FromString("大杀特杀"));
					break;
				case EKillType::EKT_SuoXiangPiMi:
					AccumulatedKill(KillNetPackgae,FText::FromString("所向披靡"));
					break;
				case EKillType::EKT_HunShenShiDan:
					AccumulatedKill(KillNetPackgae,FText::FromString("浑身是胆"));
					break;
				case EKillType::EKT_YongGuanSanJun:
					AccumulatedKill(KillNetPackgae,FText::FromString("勇冠三军"));
					break;
				case EKillType::EKT_YiJiDangQian:
					AccumulatedKill(KillNetPackgae,FText::FromString("一骑当千"));
					break;
				case EKillType::EKT_WanFuMoDi:
					AccumulatedKill(KillNetPackgae,FText::FromString("万夫莫敌"));
					break;
				case EKillType::EKT_JuShiWuShuang:
					AccumulatedKill(KillNetPackgae,FText::FromString("举世无双"));
					break;
				case EKillType::EKT_TianXiaWuDi:
					AccumulatedKill(KillNetPackgae,FText::FromString("天下无敌"));
					break;
				}
			});
		}
	});
}

void UUI_KillMsg::UpdateSlot()
{
	if (UWidgetAnimation* InWidgetAnim = GetNameWidgetAnimation(TEXT("KillTargetAnim")))
	{
		PlayAnimation(InWidgetAnim);
	}
}

void UUI_KillMsg::NormalKill(const FKillNetPackgae& KillNetPackgae, const FText& KillType)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	KillTypeText->SetText(KillType); 
	KillerNameText->SetText(FText::FromName(KillNetPackgae.KillerName));
	KillerIcon->SetBrushFromTexture(KillNetPackgae.KillerIcon);
	KilledNameText->SetText(FText::FromName(KillNetPackgae.KilledName));
	KilledIcon->SetBrushFromTexture(KillNetPackgae.KilledIcon);
}

void UUI_KillMsg::ContinuousKill(const FKillNetPackgae& KillNetPackgae, const FText& KillType)
{
	//TODO: 根据需要显隐UI
	SetVisibility(ESlateVisibility::HitTestInvisible);
	KillTypeText->SetText(KillType);
	KillerNameText->SetText(FText::FromName(KillNetPackgae.KillerName));
	KillerIcon->SetBrushFromTexture(KillNetPackgae.KillerIcon);
	KilledNameText->SetText(FText::FromName(KillNetPackgae.KilledName));
	KilledIcon->SetBrushFromTexture(KillNetPackgae.KilledIcon);
}

void UUI_KillMsg::AccumulatedKill(const FKillNetPackgae& KillNetPackgae, const FText& KillType)
{
	SetVisibility(ESlateVisibility::HitTestInvisible);
	KillTypeText->SetText(KillType);
	KillerNameText->SetText(FText::FromName(KillNetPackgae.KillerName));
	KillerIcon->SetBrushFromTexture(KillNetPackgae.KillerIcon);
	KilledNameText->SetText(FText::FromName(KillNetPackgae.KilledName));
	KilledIcon->SetBrushFromTexture(KillNetPackgae.KilledIcon);
}
