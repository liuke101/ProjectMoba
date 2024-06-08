// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameInfo/UI_GameInfo.h"

#include "ThreadManage.h"
#include "Components/TextBlock.h"
#include "Game/MobaPlayerState.h"
#include "UI/GameInfo/UI_KDAInfo.h"

void UUI_GameInfo::NativeConstruct()
{
	Super::NativeConstruct();

	GThread::GetCoroutines().BindLambda(0.3f, [&]()
	{
		if(AMobaPlayerState* MobaPlayerState = Cast<AMobaPlayerState>(GetOwningPlayerState()))
		{
			// 绑定委托
			MobaPlayerState->KDAInfoDelegate.BindUObject(KDAInfo, &UUI_KDAInfo::UpdateSlot);
			MobaPlayerState->TeamKillCountDelegate.BindUObject(this, &UUI_GameInfo::UpdateTeamKillCount);
		}
	});
}

void UUI_GameInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 更新时间
	TimeText->SetText(FText::FromString(GetCurrentCount(GetWorld()->GetRealTimeSeconds())));
}

FString UUI_GameInfo::GetCurrentCount(float NewTimeCount)
{
	const int32 NewOurTime = FMath::Max(0, FMath::TruncToInt(NewTimeCount));
	const int32 NewMinutes = NewOurTime / 60;
	const int32 NewSeconds = NewOurTime % 60;
	const FString TimeDesc = FString::Printf(TEXT("%02d:%02d"), NewMinutes, NewSeconds);
	return TimeDesc;
}

void UUI_GameInfo::UpdateTeamKillCount(int32 InFriendlyKillCount, int32 InEnemyKillCount) const
{
	FriendlyKillCount->SetText(FText::FromString(FString::Printf(TEXT("%2d"), InFriendlyKillCount)));
	EnemyKillCount->SetText(FText::FromString(FString::Printf(TEXT("%2d"), InEnemyKillCount)));
}
