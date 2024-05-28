// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Inventory/UI_InventorySlot.h"

#include "EnhancedInputComponent.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/Inventory/DragDrop/UI_ICODragDrog.h"
#include "Blueprint/DragDropOperation.h"
#include "Character/MobaPlayerController.h"

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

static int32 InventoryNumber = 1;

UUI_InventorySlot::UUI_InventorySlot()
{
}

void UUI_InventorySlot::NativeConstruct()
{
	Super::NativeConstruct();

	const FString InventoryNumberString = FString::Printf(TEXT("InventoryKey_%i"), InventoryNumber);
	//GetWorld()->GetFirstPlayerController()->InputComponent->BindAction(*InventoryNumberString, IE_Pressed, this, &UUI_InventorySlot::OnClickedWidget);
	//改为增强输入：
	if(AMobaPlayerController* MobaPlayerController = Cast<AMobaPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if(UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(MobaPlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(MobaPlayerController->LeftClick_Action, ETriggerEvent::Started, this, &UUI_InventorySlot::OnClickedWidget);
		}
	}
	
	SetKeyName(FString::FromInt(InventoryNumber));

	InventoryNumber++;
	if (InventoryNumber > 6)
	{
		InventoryNumber = 1;
	}
}

void UUI_InventorySlot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (!bDrag)
	{
		Super::NativeTick(MyGeometry, InDeltaTime);
	}
}

int32 UUI_InventorySlot::GetSlotNumber()
{
	return INDEX_NONE;
}

void UUI_InventorySlot::OnClickedWidget()
{

}

UWidget * UUI_InventorySlot::GetInventoryTip()
{
	return nullptr;
}

void UUI_InventorySlot::UpdateSlot()
{
	Super::UpdateSlot();

	bDrag = false;
}

void UUI_InventorySlot::StartUpdateCD()
{
	Super::StartUpdateCD();
}

void UUI_InventorySlot::UpdateNumber()
{

}

void UUI_InventorySlot::EndUpdateCD()
{
	Super::EndUpdateCD();

	UpdateNumber();
}

void UUI_InventorySlot::ResetSlot()
{
	Super::ResetSlot();

	SlotNumber->SetVisibility(ESlateVisibility::Hidden);
}

FReply UUI_InventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton || InMouseEvent.IsTouchEvent())
	{
		FReply Reply = FReply::Handled();
		TSharedPtr<SWidget> SlateWidgetDrag = GetCachedWidget();
		if (SlateWidgetDrag.IsValid())
		{
			Reply.DetectDrag(SlateWidgetDrag.ToSharedRef(), EKeys::RightMouseButton);
			return Reply;
		}
	}

	return FReply::Handled();
}

void UUI_InventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (ICODragDrogClass)
	{
		if (UUI_ICODragDrog* ICODragDrog = CreateWidget<UUI_ICODragDrog>(GetWorld(), ICODragDrogClass))
		{
			if (UDragDropOperation* InDropOperation = NewObject<UDragDropOperation>(GetTransientPackage(), UDragDropOperation::StaticClass()))
			{
				InDropOperation->SetFlags(RF_StrongRefOnFrame);
			//	ICODragDrog->DrawICON(InInventoryData->ItemICO);
				InDropOperation->DefaultDragVisual = ICODragDrog;
				InDropOperation->Payload = this;
				OutOperation = InDropOperation;

				ResetSlot();//隐藏自己

				bDrag = true;
			}
		}		
	}

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UUI_InventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	bool bDrop = false;

	if (const UDragDropOperation* InDragDropOperation = Cast<UDragDropOperation>(InOperation))
	{
		if (UUI_InventorySlot* MyInventorySlot = Cast<UUI_InventorySlot>(InDragDropOperation->Payload))
		{	
			bDrop = true;
		}	
	}
	
	return bDrop;
}

void UUI_InventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{

}

void UUI_InventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{

}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif