#include "UI/StatusBar/UI_StatusBar_Health.h"

#include "ThreadManage.h"
#include "Components/ProgressBar.h"
#include "Game/MobaGameState.h"
#include "Game/MobaPlayerState.h"
#include "UI/Buff/UI_BuffBar.h"

void UUI_StatusBar_Health::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_StatusBar_Health::SetHealthPercent(const float InPercent) const
{
	HealthBar->SetPercent(InPercent);
}

void UUI_StatusBar_Health::SetColor(const FLinearColor& InColor) const
{
	HealthBar->SetFillColorAndOpacity(InColor);
}

void UUI_StatusBar_Health::BindDelegate()
{
	Super::BindDelegate();

	//绑定BuffBar更新委托
	if(MobaGameState)
	{
		UpdateBuffBarDelegateHandle = MobaGameState->UpdateBuffDelegate.AddLambda([&](int64 InPlayerID, int32 DataID, float CD){
			BuffBar->UpdateCD(InPlayerID, DataID, CD);
		});

		//ue只会渲染同屏，当屏幕外玩家buff结束时，buff会被移除。当该玩家进入屏幕时，会发现buff仍显示cd，所以需要用一个委托强制移除
		EndBuffDelegateHandle = MobaGameState->EndBuffDelegate.AddLambda([&](int64 InPlayerID, int32 DataID){
			BuffBar->RemoveCD(InPlayerID, DataID);
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

void UUI_StatusBar_Health::RemoveDelegate()
{
	Super::RemoveDelegate();

	if(MobaGameState)
	{
		MobaGameState->UpdateBuffDelegate.Remove(UpdateBuffBarDelegateHandle);
		MobaGameState->EndBuffDelegate.Remove(EndBuffDelegateHandle);
	}
}

void UUI_StatusBar_Health::SetPlayerID(int64 InPlayerID)
{
	Super::SetPlayerID(InPlayerID);

	BuffBar->SetPlayerID(InPlayerID);
}

