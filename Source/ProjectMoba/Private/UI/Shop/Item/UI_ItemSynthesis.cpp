#include "UI/Shop/Item/UI_ItemSynthesis.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "UI/Shop/Item/UI_ItemSlot.h"

void FDrawSlot::FSlot::SetPosition(const FVector2D& InPosition)
{
}

FVector2D FDrawSlot::FSlot::GetSize() const
{
	return FVector2D();
}

UUI_ItemSynthesis::UUI_ItemSynthesis()
	: StartPositon(204.0f, 28.0f)
	, IconSize(32.0, 32.0)
	, LineTexture()
{
}

void UUI_ItemSynthesis::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_ItemSynthesis::UpdateSlot(const FSlotAsset* SlotAsset)
{
	//清空
	ItemCanvasPanel->ClearChildren();

	// 生成实例
	if(SlotAsset)
	{
		RecursiveUpdateSlot(SlotAsset, StartPositon, 3);
	}
}

void UUI_ItemSynthesis::RecursiveUpdateSlot(const FSlotAsset* SlotAsset, const FVector2D& StartPosition,
	int8 SlotLayer)
{
	if(SlotLayer < 0)
	{
		SlotLayer = 0;
	}

	int32 NumberChildrens = SlotAsset->ChildrensDataID.Num();

	FDrawSlot DrawSlot;

	//生成这一层所有的Slot
	//生成合成线
	auto SpawnLine = [&](TArray<FDrawSlot::FSlot>& PanelSlots, UTexture2D* Texture, int32 Layer =1)
	{
		if(UImage* Image = NewObject<UImage>(this))
		{
			if(UCanvasPanelSlot* PanelSlot = ItemCanvasPanel->AddChildToCanvas(Image)) 
			{
				PanelSlot->SetSize(FVector2D(IconSize.X * Layer, IconSize.Y)); //X轴方向的线长度可做延展，Y轴方向长度保持不变
				PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f)); // 中心对称
				PanelSlots.Add(FDrawSlot::FSlot(PanelSlot)); //收集
			}
		}
	};

	//生成物品
	auto SpawnItem = [&](TArray<FDrawSlot::FAsset>& PanelSlots, UTexture2D* Texture, int32 Layer =1)
	{
		if(UUI_ItemSlot* ItemSlot = CreateWidget<UUI_ItemSlot>(GetWorld(), ItemSlotClass))
		{
			if(UCanvasPanelSlot* PanelSlot = ItemCanvasPanel->AddChildToCanvas(ItemSlot))
			{
				PanelSlot->SetSize(IconSize); 
				PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
				PanelSlots.Add(FDrawSlot::FAsset(PanelSlot, SlotAsset)); 
			}
		}
	};
	
}
