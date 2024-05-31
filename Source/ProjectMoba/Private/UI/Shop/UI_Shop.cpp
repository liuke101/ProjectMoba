// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/UI_Shop.h"

#include "EnhancedInputComponent.h"
#include "Blueprint/WidgetTree.h"
#include "Character/MobaPlayerController.h"
#include "Components/Button.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/CheckBox.h"


void UUI_Shop::NativeConstruct()
{
	Super::NativeConstruct();
	//GetWorld()->GetFirstPlayerController()->InputComponent->BindAction(TEXT("InventoryKey_P"), IE_Pressed, this, &UUI_Shop::OnNativeKey);

	//绑定输入
	if(AMobaPlayerController* MobaPlayerController = Cast<AMobaPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(MobaPlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(MobaPlayerController->F11_Action, ETriggerEvent::Started, this, &UUI_Shop::OnNativeKey);
		}
	}

	// 绑定回调
	TopButton->OnClicked.AddDynamic(this, &UUI_Shop::OnClickedWidget);

	All->OnCheckStateChanged.AddDynamic(this,&UUI_Shop::CheckBoxAll);
	Consumables->OnCheckStateChanged.AddDynamic(this, &UUI_Shop::CheckBoxConsumables);
	AuxiliaryProps->OnCheckStateChanged.AddDynamic(this, &UUI_Shop::CheckBoxAuxiliaryProps);
	PhysicalAttack->OnCheckStateChanged.AddDynamic(this, &UUI_Shop::CheckBoxPhysicalAttack);
	MagicAttack->OnCheckStateChanged.AddDynamic(this, &UUI_Shop::CheckBoxMagicAttack);
	PhysicalPenetration->OnCheckStateChanged.AddDynamic(this, &UUI_Shop::CheckBoxPhysicalPenetration);
	MagicPenetration->OnCheckStateChanged.AddDynamic(this, &UUI_Shop::CheckBoxMagicPenetration);
	Crit->OnCheckStateChanged.AddDynamic(this, &UUI_Shop::CheckBoxCrit);
	Armor->OnCheckStateChanged.AddDynamic(this, &UUI_Shop::CheckBoxArmor);
	MagicResistance->OnCheckStateChanged.AddDynamic(this, &UUI_Shop::CheckBoxMagicResistance);
	AttackSpeed->OnCheckStateChanged.AddDynamic(this, &UUI_Shop::CheckBoxAttackSpeed);
	Shoes->OnCheckStateChanged.AddDynamic(this, &UUI_Shop::CheckBoxShoes);

	CheckBoxArray.Add(All);
	CheckBoxArray.Add(Consumables);
	CheckBoxArray.Add(AuxiliaryProps);
	CheckBoxArray.Add(PhysicalAttack);
	CheckBoxArray.Add(MagicAttack);
	CheckBoxArray.Add(PhysicalPenetration);
	CheckBoxArray.Add(MagicPenetration);
	CheckBoxArray.Add(Crit);
	CheckBoxArray.Add(Armor);
	CheckBoxArray.Add(MagicResistance);
	CheckBoxArray.Add(AttackSpeed);
	CheckBoxArray.Add(Shoes);

	// 默认隐藏商城
	SetVisibility(ESlateVisibility::Hidden);
}

void UUI_Shop::OnNativeKey()
{
	if (GetVisibility() == ESlateVisibility::Hidden)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
}

void UUI_Shop::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_Shop::OnClickedWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UUI_Shop::CheckBoxAll(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	All->SetCheckedState(ECheckBoxState::Checked);
}

void UUI_Shop::CheckBoxConsumables(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	Consumables->SetCheckedState(ECheckBoxState::Checked);


}

void UUI_Shop::CheckBoxAuxiliaryProps(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	AuxiliaryProps->SetCheckedState(ECheckBoxState::Checked);


}

void UUI_Shop::CheckBoxPhysicalAttack(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	PhysicalAttack->SetCheckedState(ECheckBoxState::Checked);


}

void UUI_Shop::CheckBoxMagicAttack(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	MagicAttack->SetCheckedState(ECheckBoxState::Checked);


}

void UUI_Shop::CheckBoxPhysicalPenetration(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	PhysicalPenetration->SetCheckedState(ECheckBoxState::Checked);


}

void UUI_Shop::CheckBoxMagicPenetration(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	MagicPenetration->SetCheckedState(ECheckBoxState::Checked);


}

void UUI_Shop::CheckBoxCrit(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	Crit->SetCheckedState(ECheckBoxState::Checked);


}

void UUI_Shop::CheckBoxArmor(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	Armor->SetCheckedState(ECheckBoxState::Checked);


}

void UUI_Shop::CheckBoxMagicResistance(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	MagicResistance->SetCheckedState(ECheckBoxState::Checked);


}

void UUI_Shop::CheckBoxAttackSpeed(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	AttackSpeed->SetCheckedState(ECheckBoxState::Checked);


}

void UUI_Shop::CheckBoxShoes(bool bIsChecked)
{
	SetCheckBoxArray(ECheckBoxState::Unchecked);
	Shoes->SetCheckedState(ECheckBoxState::Checked);


}

void UUI_Shop::SetCheckBoxArray(ECheckBoxState CheckBoxState)
{
	for (auto &Tmp : CheckBoxArray)
	{
		Tmp->SetCheckedState(CheckBoxState);
	}
}

FReply UUI_Shop::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();

	if (HasMouseCapture())
	{
	 	if (UWidgetTree* InWidgetTree = Cast<UWidgetTree>(GetOuter()))
		{
			if (UWidget* InParentWidget = Cast<UWidget>(InWidgetTree->GetOuter()))
			{
				const FGeometry& InParentGeometry = InParentWidget->GetTickSpaceGeometry();
				const FVector2D& NewPostion = InMouseEvent.GetScreenSpacePosition();

				FVector2D NewOffsetA = InParentGeometry.AbsoluteToLocal(NewPostion) - Offset;
				FVector2D NewOffsetB = InParentGeometry.GetLocalSize() - GetDesiredSize();
				UCanvasPanelSlot* NewPanelSlot = Cast<UCanvasPanelSlot>(Slot);
				if (NewPanelSlot)
				{
					NewPanelSlot->SetPosition(FVector2D(FMath::Clamp(NewOffsetA.X, 0.f, NewOffsetB.X), FMath::Clamp(NewOffsetA.Y, 0.f, NewOffsetB.Y)));
					Reply.CaptureMouse(GetCachedWidget().ToSharedRef());
				}
			}
	 	}		
	}

	return Reply;
}

FReply UUI_Shop::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{
		Offset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());
		if (Offset.Y <= 80.f)
		{
			Reply.CaptureMouse(GetCachedWidget().ToSharedRef());
		}
		else
		{
			Reply.ReleaseMouseCapture();
		}
	}

	return Reply;
}

FReply UUI_Shop::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = FReply::Handled();

	Reply.ReleaseMouseCapture();
	return Reply;
}