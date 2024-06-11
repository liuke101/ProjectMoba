#include "UI/Shop/Item/UI_ItemSynthesis.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Game/MobaPlayerState.h"
#include "UI/Shop/Item/UI_ItemSlot.h"

void FDrawSlot::FSlot::SetPosition(const FVector2D& InPosition) const
{
	if(CanvasPanelSlot)
	{
		CanvasPanelSlot->SetPosition(InPosition);
	}
}

FVector2D FDrawSlot::FSlot::GetSize() const
{
	return CanvasPanelSlot != nullptr ? CanvasPanelSlot	->GetSize() : FVector2D::ZeroVector;
}

UUI_ItemSynthesis::UUI_ItemSynthesis()
	: StartPosition(204.0f, 28.0f)
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
		int32 InLayerDepth = GetLayerDepth(SlotAsset);
		
		if(UUI_ItemSlot* ItemSlot = CreateWidget<UUI_ItemSlot>(GetWorld(), ItemSlotClass))
		{
			ItemSlot->UpdateSlot(SlotAsset);
			
			if(UCanvasPanelSlot* PanelSlot = ItemCanvasPanel->AddChildToCanvas(ItemSlot))
			{
				PanelSlot->SetSize(IconSize); 
				PanelSlot->SetAlignment(FVector2D(0.5f, 0.0f));
				PanelSlot->SetPosition(StartPosition);
				RecursiveUpdateSlot(SlotAsset, StartPosition, --InLayerDepth);
			}
		}
		
		
	}
}

void UUI_ItemSynthesis::RecursiveUpdateSlot(const FSlotAsset* SlotAsset, const FVector2D& InStartPosition,
	int8 SlotLayer)
{
	if(SlotLayer < 0)
	{
		SlotLayer = 0;
	}

	/** 生成合成线和物品, 一开始全部堆积在一个位置 */
	//生成合成线 Lambda
	auto SpawnLine = [&](TArray<FDrawSlot::FSlot>& PanelSlots, UTexture2D* Texture, int32 Layer =1)
	{
		if(UImage* Image = NewObject<UImage>(this))
		{
			Image->SetBrushFromTexture(Texture);
			if(UCanvasPanelSlot* PanelSlot = ItemCanvasPanel->AddChildToCanvas(Image)) 
			{
				PanelSlot->SetSize(FVector2D(IconSize.X * Layer, IconSize.Y)); //X轴方向的线长度可做延展，Y轴方向长度保持不变
				PanelSlot->SetAlignment(FVector2D(0.5f, 0.0f)); // 中心对称
				PanelSlots.Add(FDrawSlot::FSlot(PanelSlot)); //收集
			}
		}
	};

	//生成物品 Lambda
	auto SpawnItem = [&](TArray<FDrawSlot::FAsset>& PanelSlots, const FSlotAsset* InSlotAsset)
	{
		if(UUI_ItemSlot* ItemSlot = CreateWidget<UUI_ItemSlot>(GetWorld(), ItemSlotClass))
		{
			ItemSlot->UpdateSlot(InSlotAsset);
			
			if(UCanvasPanelSlot* PanelSlot = ItemCanvasPanel->AddChildToCanvas(ItemSlot))
			{
				PanelSlot->SetSize(IconSize); 
				PanelSlot->SetAlignment(FVector2D(0.5f, 0.0f));
				PanelSlots.Add(FDrawSlot::FAsset(PanelSlot, InSlotAsset)); 
			}
		}
	};

	int32 NumberChildrens = SlotAsset->ChildrensDataID.Num();
	FDrawSlot DrawSlot;
	
	//生成合成线
	if(NumberChildrens == 1) //只有一个孩子
	{
		SpawnLine(DrawSlot.VLinePanel, LineTexture.VerticalLine); //只需要绘制一条竖线
	}
	else if(NumberChildrens >= 2) // 大于等于两个孩子
	{
		//如果只有两个孩子：首先生成一个倒T,然后两边分别生成一条横线，最后生成两个拐角
		//如果有两个以上，每增加一个，在生成拐角前要多生成一条横线和一个T形线
		SpawnLine(DrawSlot.LREndPanel, LineTexture.TurnLeft); //左拐角
		SpawnLine(DrawSlot.LREndPanel, LineTexture.TurnRight); //右拐角
		SpawnLine(DrawSlot.Re_TLinePanel,LineTexture.Re_TLine); //倒T
		
		for(int32 i = 0; i < NumberChildrens - 2; ++i) //有3个以上孩子才会生成T型线
		{
			SpawnLine(DrawSlot.TLinePanel, LineTexture.TLine); //T型线
		}

		for(int32 i = 0; i < NumberChildrens; ++i) //每个孩子对应一条水平线
		{
			SpawnLine(DrawSlot.HLinePanel, LineTexture.HorizontalLine, SlotLayer); //水平线
		}
	}

	//生产物品
	for(auto &ChildDataID : SlotAsset->ChildrensDataID)
	{
		if(const FSlotAsset* InSlotAsset = MobaPlayerState->GetSlotAssetFromDataID(ChildDataID))
		{
			SpawnItem(DrawSlot.ItemPanel, InSlotAsset);
		}
	}

	int32 ValidNumber = DrawSlot.HLinePanel.Num()+DrawSlot.VLinePanel.Num()+DrawSlot.TLinePanel.Num()+DrawSlot.Re_TLinePanel.Num()+DrawSlot.LREndPanel.Num();
	
	/** 排布 */
	
	if(NumberChildrens == 1)
	{
		FDrawSlot::FSlot NewVLineSlot;
		FDrawSlot::FAsset NewItemSlot;
		if(!DrawSlot.VLinePanel.IsEmpty() && !DrawSlot.ItemPanel.IsEmpty())
		{
			NewVLineSlot = DrawSlot.VLinePanel.Pop();
			NewItemSlot = DrawSlot.ItemPanel.Pop();
		}

		//合成线相对物品的位置
		FVector2D NewPos = FVector2D(InStartPosition.X, InStartPosition.Y + NewVLineSlot.GetSize().Y); 
		NewVLineSlot.SetPosition(NewPos);

		//物品相对合成线的位置
		NewPos = FVector2D(NewPos.X, NewPos.Y + NewVLineSlot.GetSize().Y);
		NewItemSlot.SetPosition(NewPos);
	}
	else if(NumberChildrens >= 2)
	{
		FDrawSlot::FSlot Re_TLineSlot;
		if(!DrawSlot.Re_TLinePanel.IsEmpty())
		{
			Re_TLineSlot = DrawSlot.Re_TLinePanel.Pop();
		}
		 
		FVector2D NewPos = FVector2D(InStartPosition.X, InStartPosition.Y + Re_TLineSlot.GetSize().Y);
		Re_TLineSlot.SetPosition(NewPos);

		float HorizontalWidth = DrawSlot.HLinePanel[0].GetSize().X;
		FVector2D R = NewPos;
		FVector2D L = NewPos;

		auto Add = [&](int32 i, TArray<FDrawSlot::FSlot>& PanelSlots, int32 InSlotLayer)
		{
			FVector2D NewValidPosition = FVector2D::ZeroVector;

			FDrawSlot::FSlot NewLineSlot;
			FDrawSlot::FAsset NewItemSlot;
			
			if(!PanelSlots.IsEmpty() && !DrawSlot.ItemPanel.IsEmpty())
			{
				NewLineSlot = PanelSlots.Pop();
				NewItemSlot = DrawSlot.ItemPanel.Pop();
			}
			
			float NestSpacing = HorizontalWidth/2.0f + NewLineSlot.GetSize().X/2.0f;
			
			if(i % 2 == 0) //偶数在左
			{
				L = FVector2D(L.X - NestSpacing, L.Y);
				NewValidPosition = L;
			}
			else //奇数在右
			{
				R = FVector2D(R.X + NestSpacing, R.Y);
				NewValidPosition = R;
			}

			NewLineSlot.SetPosition(NewValidPosition);
			
			FVector2D NewItemPosition = FVector2D(NewValidPosition.X, NewValidPosition.Y + NewItemSlot.GetSize().Y);
			NewItemSlot.SetPosition(NewItemPosition);

			//递归
			if(const FSlotAsset* NewSlotAsset = NewItemSlot.GetSlotAsset())
			{
				RecursiveUpdateSlot(NewSlotAsset, NewItemPosition, InSlotLayer -= 2);
			}
			
		};

		// 绘制水平
		for(int32 i = 0; i < ValidNumber; ++i)
		{
			FVector2D NewValidPosition = FVector2D::ZeroVector;

			FDrawSlot::FSlot HLine;
			if(!DrawSlot.HLinePanel.IsEmpty())
			{
				HLine = DrawSlot.HLinePanel.Pop();
			}
			
			float NestSpacing = HLine.GetSize().X/2.0f + Re_TLineSlot.GetSize().X/2.0f;
			if(i % 2 == 0) //偶数在左
			{
				L = FVector2D(L.X - NestSpacing, L.Y);
				NewValidPosition = L;
			}
			else //奇数在右
			{
				R = FVector2D(R.X + NestSpacing, R.Y);
				NewValidPosition = R;
			}

			HLine.SetPosition(NewValidPosition);

			if(DrawSlot.TLinePanel.Num())
			{
				Add(i, DrawSlot.TLinePanel, SlotLayer);
			}

			//绘制拐点
			for(int32 j = 0; j<2; j++)
			{
				Add(j, DrawSlot.LREndPanel, SlotLayer);
			}
		}
	}
}

int32 UUI_ItemSynthesis::GetLayerDepth(const FSlotAsset* SlotAsset, float InDepth)
{
	InDepth++;
	TArray<int32> Depths;

	for(auto& Tmp : SlotAsset->ChildrensDataID)
	{
		if(const FSlotAsset* InSlotAsset = MobaPlayerState->GetSlotAssetFromDataID(Tmp))
		{
			Depths.Add(GetLayerDepth(InSlotAsset, InDepth));
		}
	}

	for(auto& Tmp : Depths)
	{
		if(Tmp > InDepth)
		{
			InDepth = Tmp;
		}
	}

	return InDepth;
}
