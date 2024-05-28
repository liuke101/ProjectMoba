// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MiniMap/MobaMiniMapUI.h"

#include "Common/RenderingUtils.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Engine/SceneCapture2D.h"
#include "Game/MobaGameState.h"
#include "ProjectMoba/MiscData.h"
#include "UI/MobaHUD.h"

void UMobaMiniMapUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 捕获地图，设置到 MiniMapImage
	if(ASceneCapture2D* SceneCapture2D = RenderingUtils::SpawnSceneCapture2D(GetWorld(), SceneCapture2DClass, MapSize, 0.03f))
    {
		MiniMapImage->SetBrushFromMaterial(Cast<UMaterialInterface>(MiniMapMaterial.GetObject()));   
    }

	// 设置地图大小
	if(MiniMapImage)
	{
		if(UCanvasPanelSlot* ImagePanelSlot = Cast<UCanvasPanelSlot>(MiniMapImage->Slot))
		{
			ImagePanelSlot->SetSize(FVector2D(GetMobaHUD()->GetClientGlobalConfig()->MaxMiniMapSize));
		}
	}
}

void UMobaMiniMapUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (AMobaGameState *MobaGameState = GetWorld()->GetGameState<AMobaGameState>())
	{
		const TArray<FPlayerLocation> &PlayerLocations = MobaGameState->GetPlayerLocations();

		if (UCanvasPanelSlot* ImagePanelSlot = Cast<UCanvasPanelSlot>(MiniMapImage->Slot))
		{
			const float MaxMiniMapSize = GetMobaHUD()->GetClientGlobalConfig()->MaxMiniMapSize;
			const float MaxTargetArmLength = GetMobaHUD()->GetClientGlobalConfig()->MaxTargetArmLength;

			FVector2D LocalSize2D = MyGeometry.GetDrawSize();

			//MiniMap缩放
			FVector2D ZoomSize;
			{
				//TODO:随鼠标滚动缩放
				float TargetArmLength = 800.f;//Cast<ATowerDefenceGameCamera>(GetWorld()->GetFirstPlayerController()->GetPawn())->GetTargetArmLength();
				ZoomSize = FVector2D((MaxMiniMapSize / MaxTargetArmLength) * TargetArmLength);
				ImagePanelSlot->SetSize(ZoomSize);
			}

			//MiniMap位置(玩家移动，MiniMap跟随移动）
			FVector2D MinImageMapOffset;
			{
				//MiniMap图像与玩家的移动方向相反，看上去小地图是静止的，玩家在移动
				FVector Location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
				MinImageMapOffset.Y = -(ZoomSize.X - (ZoomSize.X / MapSize.X) * Location.X);
				MinImageMapOffset.X = -(ZoomSize.Y / MapSize.Y) * Location.Y;

				ImagePanelSlot->SetPosition(MinImageMapOffset);
			}

			//场景中的角色
			for (auto& PlayerLocation : PlayerLocations)
			{
				if (!IsExistence(PlayerLocation.PlayerID))
				{
					if (UImage* Point = NewObject<UImage>(GetWorld(), UImage::StaticClass()))
					{
						if (UCanvasPanelSlot* PanelSlot = MiniMapPanel->AddChildToCanvas(Point))
						{
							PanelSlot->SetZOrder(1.0f);
							PanelSlot->SetAnchors(0.5f);
							//Point->SetBrushFromTexture(Tmp.Value.Icon.LoadSynchronous());
							PanelSlot->SetSize(FVector2D(32.f));
							PanelSlot->SetAlignment(FVector2D(.5f));
							CharacterID_To_PanelSlot.Add(PlayerLocation.PlayerID, PanelSlot);
						}
					}
				}
				else
				{
					if (UCanvasPanelSlot* PanelSlot = CharacterID_To_PanelSlot[PlayerLocation.PlayerID].Get())
					{
						FVector2D MinMapPos;
						//将角色在游戏世界中的位置映射到小地图上的位置
						MinMapPos.Y = ZoomSize.X - (ZoomSize.X / MapSize.X) * PlayerLocation.Location.X + MinImageMapOffset.Y;
						MinMapPos.X = (ZoomSize.Y / MapSize.Y) * PlayerLocation.Location.Y + MinImageMapOffset.X;

						ResetLocation(PanelSlot, MinMapPos, LocalSize2D);
					}
				}
				
			}

			TArray<int64> RemoveGuid;
			for (auto& Tmp : CharacterID_To_PanelSlot)
			{
				bool bContains = false;
				for (auto &L : PlayerLocations)
				{
					if (L.PlayerID == Tmp.Key)
					{
						bContains = true;
						break;
					}
				}

				if (!bContains)
				{
					if (UCanvasPanelSlot* PanelSlot = CharacterID_To_PanelSlot[Tmp.Key].Get())
					{
						MiniMapPanel->RemoveChild(PanelSlot->Content);
						RemoveGuid.Add(Tmp.Key);
					}
				}
			}

			for (auto& Tmp : RemoveGuid)
			{
				CharacterID_To_PanelSlot.Remove(Tmp);
			}
		}
		
	}
}


bool UMobaMiniMapUI::IsExistence(const int64 InID)
{
	for (auto& Tmp : CharacterID_To_PanelSlot)
	{
		if (Tmp.Key == InID)
		{
			return true;
		}
	}

	return false;
}

void UMobaMiniMapUI::ResetLocation(UCanvasPanelSlot* PanelSlot, const FVector2D& MinMapPos, const FVector2D& LocalSize2D)
{
	const FVector2D IconSize = PanelSlot->GetSize() / 2;

	auto IsRange = [&](const float CompareValue, const float Value)->bool
	{
		return Value > -CompareValue && Value < CompareValue;
	};

	FVector2D NewPos = MinMapPos;
	if (MinMapPos.X <= LocalSize2D.X &&
		MinMapPos.Y <= LocalSize2D.Y &&
		MinMapPos.X >= -LocalSize2D.X &&
		MinMapPos.Y >= -LocalSize2D.Y)
	{

	}
	// 限制玩家图标在小地图内
	else if (MinMapPos.Y < -LocalSize2D.Y && IsRange(LocalSize2D.X, MinMapPos.X))//上
	{
		NewPos = FVector2D(MinMapPos.X, -LocalSize2D.Y + IconSize.Y);
	}
	else if (MinMapPos.X < -LocalSize2D.X && IsRange(LocalSize2D.Y, MinMapPos.Y))//左
	{
		NewPos = FVector2D(-LocalSize2D.X + IconSize.Y, MinMapPos.Y);
	}
	else if (MinMapPos.Y > LocalSize2D.Y && IsRange(LocalSize2D.X, MinMapPos.X))//下
	{
		NewPos = FVector2D(MinMapPos.X, LocalSize2D.Y - IconSize.Y);
	}
	else if (MinMapPos.X > LocalSize2D.X && IsRange(LocalSize2D.Y, MinMapPos.Y))//右
	{
		NewPos = FVector2D(LocalSize2D.X - IconSize.X, MinMapPos.Y);
	}
	else if (MinMapPos.X < -LocalSize2D.X && MinMapPos.Y < -LocalSize2D.Y)//左上
	{
		NewPos = -LocalSize2D + IconSize;
	}
	else if (MinMapPos.X < -LocalSize2D.X && MinMapPos.Y > LocalSize2D.Y)//左下
	{
		NewPos = FVector2D(-LocalSize2D.X + IconSize.X, LocalSize2D.Y - IconSize.Y);
	}
	else if (MinMapPos.X > LocalSize2D.X && MinMapPos.Y > LocalSize2D.Y)//右下
	{
		NewPos = LocalSize2D - IconSize;
	}
	else if (MinMapPos.X > LocalSize2D.X && MinMapPos.Y < -LocalSize2D.Y)//右上
	{
		NewPos = FVector2D(LocalSize2D.X - IconSize.X, -LocalSize2D.Y + IconSize.Y);
	}

	PanelSlot->SetPosition(NewPos);
}
