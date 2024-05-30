// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaPlayerState.h"

#include "ThreadManage.h"
#include "Common/MethodUnit.h"
#include "Component/PlayerDataComponent.h"
#include "ProjectMoba/MiscData.h"
#include "Table/CharacterAsset.h"
#include "Table/SlotAttribute.h"
#include "Table/SlotAsset.h"

AMobaPlayerState::AMobaPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	PlayerDataComponent = CreateDefaultSubobject<UPlayerDataComponent>(TEXT("PlayerDataComponent"));
}

void AMobaPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//服务器更新CD
	if(GetLocalRole() == ROLE_Authority)
	{
		TArray<int32> RemoveSlots;

		for(auto& Tmp : PlayerDataComponent->SlotQueue)
		{
			Tmp.Value->CD -= DeltaSeconds;
			if(Tmp.Value->CD <= 0.f)
			{
				Tmp.Value->CD = 0.f;
				if(const FSlotAsset* SlotAsset = GetSlotAssetTemplate(Tmp.Value->SlotID))
				{
					if(FSlotAttribute* SlotAttribute = GetSlotAttributeFromID(Tmp.Key))
					{
						//检测数量
						CheckInventory(Tmp.Key);

						//客户端矫正CD
						Client_EndUpdateCD(Tmp.Key, *Tmp.Value);
					}
				}
				
				RemoveSlots.Add(Tmp.Key);
			}
		}

		for(auto& RemoveSlot : RemoveSlots)
		{
			PlayerDataComponent->SlotQueue.Remove(RemoveSlot);
		}
	}
}

void AMobaPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// 创建6个Inventory Slot
	if(GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		for(int32 i = 0; i < 6; i++)
		{
			RecursionCreateInventorySlots();
		}
		for(int32 i = 0; i < 4; i++)
		{
			RecursionCreateSkillSlots();
		}
		
		// 延迟执行，等待客户端生成
		GThread::GetCoroutines().BindLambda(2.0f,[&]()
		{
			FSlotDataNetPackage NetInventoryPackage;
			GetInventorySlotNetPackage(NetInventoryPackage);
			Client_InitInventorySlots(NetInventoryPackage);

			//测试Inventory(正式功能为点击购买)
			AddSlotToInventory(99990);
			AddSlotToInventory(99991);

			FSlotDataNetPackage NetSkillPackage;
			GetSkillSlotNetPackage(NetSkillPackage);
			Client_InitSkillSlots(NetSkillPackage);
			//测试Skill
			InitSkillSlot();
		});
	}
}

const FSlotAsset* AMobaPlayerState::GetSlotAssetTemplate(const int32 InID)
{
	for(auto& SlotAsset : *GetSlotAssetsTemplate())
	{
		if(SlotAsset->ID == InID)
		{
			return SlotAsset;
		}
	}
	return nullptr;
}

const TArray<FSlotAsset*>* AMobaPlayerState::GetSlotAssetsTemplate()
{
	if(CacheSlotAssets.IsEmpty())
	{
		checkf(DT_SlotAsset, TEXT("SlotAssetTable为空，请在BP_MobaPlayerState中配置"));
		DT_SlotAsset->GetAllRows(TEXT("SlotAssetTable"), CacheSlotAssets);
	}

	return &CacheSlotAssets;
}

const FSlotAttribute* AMobaPlayerState::GetSlotAttributeTemplate(const int32 InID)
{
	for(auto& SlotAttribute : *GetSlotAttributesTemplate())
	{
		if(SlotAttribute->ID == InID)
		{
			return SlotAttribute;
		}
	}

	return nullptr;
}

const TArray<FSlotAttribute*>* AMobaPlayerState::GetSlotAttributesTemplate()
{
	if(CacheSlotAttributes.IsEmpty())
	{
		checkf(DT_SlotAttribute, TEXT("SlotAttributeTable为空，请在BP_MobaPlayerState中配置"));
		DT_SlotAttribute->GetAllRows(TEXT("SlotAttributeTable"), CacheSlotAttributes);
	}

	return &CacheSlotAttributes;
}

FSlotAttribute* AMobaPlayerState::GetSlotAttributeFromID(const int32 InInventoryID) const
{
	if(PlayerDataComponent->SlotAttribute_Internal.Contains(InInventoryID))
	{
		return PlayerDataComponent->SlotAttribute_Internal[InInventoryID];
	}
	return nullptr;
}

bool AMobaPlayerState::IsCDValid(int32 InInventoryID) const
{
	if(FSlotData* SlotData = GetInventorySlotData(InInventoryID))
	{
		return SlotData->CD <= 0.f;
	}
	// else if(0) //技能
	// {
	// 	
	// }
	return false;
}

bool AMobaPlayerState::AddSlotAttributes(const int32 InInventoryID, const int32 InSlotID)
{
	if(const FSlotAttribute* SlotAttribute = GetSlotAttributeTemplate(InSlotID))
	{
		//如果不为空，直接替换
		if(PlayerDataComponent->SlotAttribute_Internal.Contains(InInventoryID))
		{
			*PlayerDataComponent->SlotAttribute_Internal[InInventoryID] = *SlotAttribute;
		}
		else //否则直接添加
		{
			PlayerDataComponent->SlotAttribute_Internal.Add(InInventoryID, *SlotAttribute);
		}
		return true;
	}
	return false;
}

bool AMobaPlayerState::RecursionAddSlotAttributes(const int32 InSlotID)
{
	if(const FSlotAttribute* SlotAttribute = GetSlotAttributeTemplate(InSlotID))
	{
		int32 RandID = FMath::RandRange(0, 9999999);
		//如果为空，直接添加
		if(!PlayerDataComponent->SlotAttribute_Internal.Contains(RandID))
		{
			PlayerDataComponent->SlotAttribute_Internal.Add(RandID, *SlotAttribute);
			return true;
		}

		//否则
		return RecursionAddSlotAttributes(InSlotID);
	}
	return false;
}

FSlotData* AMobaPlayerState::GetSlotData(int32 ID) const
{
	if(FSlotData* InventorySlotData = GetInventorySlotData(ID))
	{
		return InventorySlotData;
	}

	if(FSlotData* SkillSlotData = GetSkillSlotData(ID))
	{
		return SkillSlotData;
	}

	return nullptr;
}


void AMobaPlayerState::RecursionCreateInventorySlots()
{
	RecursionCreatelSlots(PlayerDataComponent->InventorySlots, PlayerDataComponent->SkillSlots);
}

bool AMobaPlayerState::AddSlotToInventory(int32 InSlotID)
{
	if(HasEmptyInventorySlot())
	{
		if(const FSlotAsset* SlotAsset = GetSlotAssetTemplate(InSlotID))
		{
			int32 InInventoryID = INDEX_NONE;
			for(auto& Tmp : *GetInventorySlots())
			{
				//找空位
				if(Tmp.Value.SlotID == INDEX_NONE)
				{
					Tmp.Value.Number = 3;
					Tmp.Value.SlotID = SlotAsset->ID; //SlotID对应DataTable, 用于获取对应的数据
					Tmp.Value.SlotIcon = SlotAsset->SlotIcon;
					InInventoryID = Tmp.Key; //InventoryID对应UI格子的ID
					break;
				}
			}

			if(InInventoryID != INDEX_NONE && AddSlotAttributes(InInventoryID, InSlotID)) //添加到Inventory
			{
				//通知本机客户端，更新Slot
				Client_UpdateSlot(InInventoryID, PlayerDataComponent->InventorySlots[InInventoryID]);
			}
			return true;
		}
	}

	return false;
}

bool AMobaPlayerState::HasEmptyInventorySlot() const
{
	// 如果有空位，返回true
	for(auto& Tmp : *GetInventorySlots())
	{
		if(Tmp.Value.SlotID == INDEX_NONE)
		{
			return true;
		}
	}
	return false;
}

bool AMobaPlayerState::IsValidInventorySlot(int32 InInventoryID)
{
	for(auto& Tmp : *GetInventorySlots())
	{
		if(Tmp.Key == InInventoryID)
		{
			return Tmp.Value.SlotID != INDEX_NONE;
		}
	}
	return false;
}

TMap<int32, FSlotData>* AMobaPlayerState::GetInventorySlots() const
{
	return &PlayerDataComponent->InventorySlots;
}


FSlotData* AMobaPlayerState::GetInventorySlotData(int32 InInventoryID) const
{
	if (GetInventorySlots()->Contains(InInventoryID))
	{
		return &GetInventorySlots()->FindChecked(InInventoryID);
	}

	return nullptr;
}

void AMobaPlayerState::GetAllInventoryIDs(TArray<int32>& OutInventoryIDs) const
{
	GetAllIDs(*GetInventorySlots(), OutInventoryIDs);
}

void AMobaPlayerState::CheckInventory(int32 InInventoryID) const
{
	if(FSlotData* SlotData = GetInventorySlotData(InInventoryID))
	{
		//如果Slot上物品数量为0，重置、清空Slot
		if(SlotData->Number == 0)
		{
			SlotData->Reset();
			PlayerDataComponent->SlotAttribute_Internal.Remove(InInventoryID);
		}
	}
}

void AMobaPlayerState::RecursionCreateSkillSlots()
{
	RecursionCreatelSlots(PlayerDataComponent->SkillSlots, PlayerDataComponent->InventorySlots);
}

TMap<int32, FSlotData>* AMobaPlayerState::GetSkillSlots() const
{
	return &PlayerDataComponent->SkillSlots;
}

FSlotData* AMobaPlayerState::GetSkillSlotData(int32 InSkillID) const
{
	if (GetSkillSlots()->Contains(InSkillID))
	{
		return &GetSkillSlots()->FindChecked(InSkillID);
	}

	return nullptr;
}

void AMobaPlayerState::InitSkillSlot()
{
	if(const FCharacterAsset* CharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), PlayerDataComponent->PlayerID))
	{
		auto AddSlotToSkillBar = [&](const FCharacterSkill& Skill)
		{
			int InSkillID = INDEX_NONE;
			for(auto& Tmp : *GetSkillSlots())
			{
				if(Tmp.Value.SlotID == INDEX_NONE)
				{
					Tmp.Value.SlotID = Skill.ID;
					Tmp.Value.SlotIcon = Skill.Icon;
					Tmp.Value.Number = INDEX_NONE;
					Tmp.Value.CD = 0.f;
					InSkillID = Tmp.Key;
					break;
				}
			}

			if(InSkillID != INDEX_NONE && AddSlotAttributes(InSkillID, Skill.ID))
			{
				Client_UpdateSlot(InSkillID, PlayerDataComponent->SkillSlots[InSkillID]);
			}
		};

		AddSlotToSkillBar(CharacterAsset->W_Skill);
		AddSlotToSkillBar(CharacterAsset->E_Skill);
		AddSlotToSkillBar(CharacterAsset->R_Skill);
		AddSlotToSkillBar(CharacterAsset->F_Skill);
	}
}

void AMobaPlayerState::GetAllSkillIDs(TArray<int32>& OutSkillIDs) const
{
	GetAllIDs(*GetSkillSlots(), OutSkillIDs);
}


void AMobaPlayerState::RecursionCreatelSlots(TMap<int32, FSlotData>& CreateSlots, TMap<int32, FSlotData>& CompareSlots)
{
	int32 ID = FMath::RandRange(0, 9999999);
	if(!CreateSlots.Contains(ID) && !CompareSlots.Contains(ID)) //保证该ID没有被使用过
	{
		CreateSlots.Add(ID, FSlotData());
	}
	else
	{
		RecursionCreatelSlots(CreateSlots, CompareSlots);
	}
}

void AMobaPlayerState::GetAllIDs(const TMap<int32, FSlotData>& InSlots, TArray<int32>& OutIDs)
{
	for(auto& Tmp : InSlots)
	{
		OutIDs.Add(Tmp.Key);
	}
}


void AMobaPlayerState::GetInventorySlotNetPackage(FSlotDataNetPackage& OutNetPackage)
{
	for(auto& Tmp : *GetInventorySlots())
	{
		OutNetPackage.IDs.Add(Tmp.Key);
		OutNetPackage.SlotDatas.Add(Tmp.Value);
	}
}

void AMobaPlayerState::GetSkillSlotNetPackage(FSlotDataNetPackage& OutNetPackage)
{
	for(auto& Tmp : *GetSkillSlots())
	{
		OutNetPackage.IDs.Add(Tmp.Key);
		OutNetPackage.SlotDatas.Add(Tmp.Value);
	}
}


void AMobaPlayerState::Server_UpdateInventory_Implementation(int32 InMoveInventoryID, int32 InTargetInventoryID)
{
	if(PlayerDataComponent->InventorySlots.Contains(InMoveInventoryID) && PlayerDataComponent->InventorySlots.Contains(InTargetInventoryID))
	{
		FSlotData& MoveInventorySlotData = PlayerDataComponent->InventorySlots[InMoveInventoryID]; //移动到目标Slot的数据
		FSlotData& TargetInventorySlotData = PlayerDataComponent->InventorySlots[InTargetInventoryID]; //目标Slot原来的数据

		//如果目标Slot为空位置，直接移动
		if(TargetInventorySlotData.SlotID == INDEX_NONE)
		{
			//Slot数据移动
			TargetInventorySlotData = MoveInventorySlotData;
			MoveInventorySlotData.Reset();
			
			//队列移动 和CD相关
			if(PlayerDataComponent->SlotQueue.Contains(InMoveInventoryID))
			{
				PlayerDataComponent->SlotQueue.Remove(InMoveInventoryID);
				PlayerDataComponent->SlotQueue.Add(InTargetInventoryID, &TargetInventorySlotData);
			}

			//属性移动
			PlayerDataComponent->SlotAttribute_Internal.SetKeyToNewKey(InMoveInventoryID, InTargetInventoryID);
		}
		else //如果目标Slot不为空，交换位置
		{
			//Slot数据交换
			Swap(MoveInventorySlotData, TargetInventorySlotData);

			//队列交换
			if(PlayerDataComponent->SlotQueue.Contains(InMoveInventoryID))
			{
				PlayerDataComponent->SlotQueue.Remove(InMoveInventoryID);
				PlayerDataComponent->SlotQueue.Add(InMoveInventoryID, &MoveInventorySlotData);
			}

			if(PlayerDataComponent->SlotQueue.Contains(InTargetInventoryID))
			{
				PlayerDataComponent->SlotQueue.Remove(InTargetInventoryID);
				PlayerDataComponent->SlotQueue.Add(InTargetInventoryID, &TargetInventorySlotData);
			}

			//属性交换
			PlayerDataComponent->SlotAttribute_Internal.SwapKey(InMoveInventoryID, InTargetInventoryID);
		}

		//通知客户端更新UI
		Client_UpdateSlot(InMoveInventoryID, MoveInventorySlotData);
		Client_UpdateSlot(InTargetInventoryID, TargetInventorySlotData);
	}
	
}

void AMobaPlayerState::Server_Use_Implementation(int32 InInventoryID)
{
	if(FSlotData* SlotData = GetInventorySlotData(InInventoryID))
	{
		if(SlotData->Number > 0)
		{
			SlotData->Number--;
			CheckInventory(InInventoryID); 
			Client_UpdateSlot(InInventoryID, *SlotData);
		}

		if(FSlotAttribute* SlotAttribute = GetSlotAttributeFromID(InInventoryID))
		{
			SlotData->CD = SlotAttribute->CD;
			PlayerDataComponent->SlotQueue.Add(InInventoryID, SlotData);
			Client_StartUpdateCD(InInventoryID, *SlotData);
		}
	}

	
}

void AMobaPlayerState::Client_InitInventorySlots_Implementation(const FSlotDataNetPackage& InSlotDataNetPackage)
{
	//清空旧数据
	PlayerDataComponent->InventorySlots.Empty(); 

	//读取网络包，加载到InventorySlots
	for(int32 i = 0; i < InSlotDataNetPackage.IDs.Num(); i++)
	{
		PlayerDataComponent->InventorySlots.Add(InSlotDataNetPackage.IDs[i], InSlotDataNetPackage.SlotDatas[i]);
	}

	//多播委托
	InitSlotDelegate.Broadcast();
}


void AMobaPlayerState::Client_InitSkillSlots_Implementation(const FSlotDataNetPackage& InSlotDataNetPackage)
{
	//清空旧数据
	PlayerDataComponent->SkillSlots.Empty(); 

	//读取网络包，加载到InventorySlots
	for(int32 i = 0; i < InSlotDataNetPackage.IDs.Num(); i++)
	{
		PlayerDataComponent->SkillSlots.Add(InSlotDataNetPackage.IDs[i], InSlotDataNetPackage.SlotDatas[i]);
	}

	//多播委托
	InitSlotDelegate.Broadcast();
}

void AMobaPlayerState::Client_UpdateSlot_Implementation(int32 InInventoryID, const FSlotData& InNetSlotData)
{
	//更新Slot
	if(FSlotData* SlotData = GetInventorySlotData(InInventoryID))
	{
		*SlotData = InNetSlotData;
	}
	// else if(0) //技能
	// {
	// 	
	// }

	UpdateSlotDelegate.Broadcast(InInventoryID);
	
}

void AMobaPlayerState::Client_StartUpdateCD_Implementation(int32 InInventoryID, const FSlotData& InNetSlotData)
{
	if(PlayerDataComponent->InventorySlots.Contains(InInventoryID))
	{
		PlayerDataComponent->InventorySlots[InInventoryID].CD = InNetSlotData.CD;
	}

	StartUpdateCDSlotDelegate.Broadcast(InInventoryID);
	
}

void AMobaPlayerState::Client_EndUpdateCD_Implementation(int32 InInventoryID, const FSlotData& InNetSlotData)
{
	if(PlayerDataComponent->InventorySlots.Contains(InInventoryID))
	{
		PlayerDataComponent->InventorySlots[InInventoryID].CD = InNetSlotData.CD;
	}

	EndUpdateCDSlotDelegate.Broadcast(InInventoryID);
	
}

