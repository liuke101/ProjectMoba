#include "UI/StatusBar/UI_StatusBar_Health.h"

#include "ThreadManage.h"
#include "Components/ProgressBar.h"
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
	if(AMobaPlayerState* MobaPlayerState = GetMobaPlayerState())
	{
		MobaPlayerState->UpdateBuffBarDelegate.AddLambda([&](int32 SlotID, float CD){
			BuffBar->UpdateCD(SlotID, CD);
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
