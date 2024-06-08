#include "UI/StatusBar/UI_StatusBar_Health.h"

#include "Components/ProgressBar.h"

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
