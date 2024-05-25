#include "UI/StatusBar/MobaStatusBarUI_Health.h"

#include "Components/ProgressBar.h"

void UMobaStatusBarUI_Health::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMobaStatusBarUI_Health::SetHealthPercent(const float InPercent) const
{
	HealthBar->SetPercent(InPercent);
}
