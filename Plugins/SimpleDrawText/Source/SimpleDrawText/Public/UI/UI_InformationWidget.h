// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SimpleDrawTextType.h"
#include "UI_InformationWidget.generated.h"

class UWidgetAnimation;
class UDataTable;
struct FSimpleDrawTextStyle;
/**
 * 
 */
UCLASS()
class SIMPLEDRAWTEXT_API UUI_InformationWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextWidget;

	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(meta = (BindWidget))
	class UImage* BackgroundIcon;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	UDataTable* StyleTable;

public:	
	UUI_InformationWidget(const FObjectInitializer& ObjectInitializer);

	void SetTextBlockStyle(const FString &InText, const FLinearColor &Color,float Percentage,EInfoAnimType Type = EInfoAnimType::TYPE_DEFAULT, int32 PrefixStyleID = INDEX_NONE,bool bBackground = false);

private:
	TArray<FSimpleDrawTextStyle*>* GetDrawTextStyle();

protected:
	UWidgetAnimation* GetNameWidgetAnimation(const FString& WidgetAnimationName) const;

private:
	TArray<FSimpleDrawTextStyle*> SimpleDrawTextStyleCache;
};
