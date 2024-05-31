// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaPlayerState.h"

#include "ThreadManage.h"
#include "Character/MobaCharacter.h"
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
				if(const FSlotAsset* SlotAsset = GetSlotAssetFromDataID(Tmp.Value->DataID))
				{
					
				}
				// else
				// {
				// 	
				// }

				if(FSlotAttribute* SlotAttribute = GetSlotAttributeFromSlotID(Tmp.Key))
				{
					//检测数量
					CheckInventory(Tmp.Key);

					//客户端矫正CD
					Client_EndUpdateCD(Tmp.Key, *Tmp.Value);
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

	if(GetWorld()->IsNetMode(NM_DedicatedServer))
	{
		//创建Slot
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

const TArray<FSlotAsset*>* AMobaPlayerState::GetSlotAssets()
{
	if(CacheSlotAssets.IsEmpty())
	{
		checkf(DT_SlotAsset, TEXT("SlotAssetTable为空，请在BP_MobaPlayerState中配置"));
		DT_SlotAsset->GetAllRows(TEXT("SlotAssetTable"), CacheSlotAssets);
	}

	return &CacheSlotAssets;
}

const FSlotAsset* AMobaPlayerState::GetSlotAssetFromDataID(const int32 DataID)
{
	for(auto& SlotAsset : *GetSlotAssets())
	{
		if(SlotAsset->DataID == DataID)
		{
			return SlotAsset;
		}
	}
	return nullptr;
}

const TArray<FSlotAttribute*>* AMobaPlayerState::GetSlotAttributes()
{
	if(CacheSlotAttributes.IsEmpty())
	{
		checkf(DT_SlotAttribute, TEXT("SlotAttributeTable为空，请在BP_MobaPlayerState中配置"));
		DT_SlotAttribute->GetAllRows(TEXT("SlotAttributeTable"), CacheSlotAttributes);
	}

	return &CacheSlotAttributes;
}

const FSlotAttribute* AMobaPlayerState::GetSlotAttributeFromDataID(const int32 DataID)
{
	for(auto& SlotAttribute : *GetSlotAttributes())
	{
		if(SlotAttribute->DataID == DataID)
		{
			return SlotAttribute;
		}
	}

	return nullptr;
}

FSlotAttribute* AMobaPlayerState::GetSlotAttributeFromSlotID(const int32 SlotID) const
{
	if(PlayerDataComponent->SlotAttribute_Internal.Contains(SlotID))
	{
		return PlayerDataComponent->SlotAttribute_Internal[SlotID];
	}
	return nullptr;
}

bool AMobaPlayerState::IsCDValid(int32 SlotID) const
{
	if(const FSlotData* SlotData = GetSlotData(SlotID))
	{
		return SlotData->CD <= 0.f;
	}
	
	return false;
}

void AMobaPlayerState::RecursionCreatelSlots(TMap<int32, FSlotData>& CreateSlots, TMap<int32, FSlotData>& CompareSlots)
{
	int32 RandSlotID = FMath::RandRange(0, 999999);
	if(!CreateSlots.Contains(RandSlotID) && !CompareSlots.Contains(RandSlotID)) //保证该ID没有被使用过
	{
		//添加的Slot数据都为默认值
		CreateSlots.Add(RandSlotID, FSlotData());
	}
	else
	{
		RecursionCreatelSlots(CreateSlots, CompareSlots);
	}
}

void AMobaPlayerState::GetAllSlotIDs(const TMap<int32, FSlotData>& InSlots, TArray<int32>& OutSlotIDs)
{
	for(auto& Tmp : InSlots)
	{
		OutSlotIDs.Add(Tmp.Key);
	}
}

bool AMobaPlayerState::AddSlotAttributes(const int32 SlotID, const int32 DataID)
{
	if(const FSlotAttribute* SlotAttribute = GetSlotAttributeFromDataID(DataID))
	{
		//如果不为空，直接替换
		if(PlayerDataComponent->SlotAttribute_Internal.Contains(SlotID))
		{
			*PlayerDataComponent->SlotAttribute_Internal[SlotID] = *SlotAttribute;
		}
		else //否则直接添加
		{
			PlayerDataComponent->SlotAttribute_Internal.Add(SlotID, *SlotAttribute);
		}
		return true;
	}
	return false;
}

bool AMobaPlayerState::RecursionAddSlotAttributes(const int32 SlotID)
{
	if(const FSlotAttribute* SlotAttribute = GetSlotAttributeFromDataID(SlotID))
	{
		int32 RandSlotID = FMath::RandRange(0, 999999);
		//如果为空，直接添加
		if(!PlayerDataComponent->SlotAttribute_Internal.Contains(RandSlotID))
		{
			PlayerDataComponent->SlotAttribute_Internal.Add(RandSlotID, *SlotAttribute);
			return true;
		}

		//否则
		return RecursionAddSlotAttributes(SlotID);
	}
	return false;
}

FSlotData* AMobaPlayerState::GetSlotData(int32 SlotID) const
{
	if(FSlotData* InventorySlotData = GetInventorySlotData(SlotID))
	{
		return InventorySlotData;
	}

	if(FSlotData* SkillSlotData = GetSkillSlotData(SlotID))
	{
		return SkillSlotData;
	}

	return nullptr;
}


void AMobaPlayerState::RecursionCreateInventorySlots()
{
	RecursionCreatelSlots(PlayerDataComponent->InventorySlots, PlayerDataComponent->SkillSlots);
}

bool AMobaPlayerState::AddSlotToInventory(int32 DataID)
{
	if(HasEmptyInventorySlot())
	{
		if(const FSlotAsset* SlotAsset = GetSlotAssetFromDataID(DataID))
		{
			int32 SlotID = INDEX_NONE;
			for(auto& Tmp : *GetInventorySlots())
			{
				//找空位
				if(Tmp.Value.DataID == INDEX_NONE)
				{
					Tmp.Value.Number = 3; //暂定为3
					Tmp.Value.DataID = SlotAsset->DataID; 
					Tmp.Value.SlotIcon = SlotAsset->SlotIcon;
					SlotID = Tmp.Key; 
					break;
				}
			}

			if(SlotID != INDEX_NONE && AddSlotAttributes(SlotID, DataID)) //添加到Inventory
			{
				//通知本机客户端，更新Slot
				Client_UpdateSlot(SlotID, PlayerDataComponent->InventorySlots[SlotID]);
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
		if(Tmp.Value.DataID == INDEX_NONE)
		{
			return true;
		}
	}
	return false;
}

bool AMobaPlayerState::IsValidInventorySlot(int32 SlotID)
{
	for(auto& Tmp : *GetInventorySlots())
	{
		if(Tmp.Key == SlotID)
		{
			return Tmp.Value.DataID != INDEX_NONE;
		}
	}
	return false;
}

TMap<int32, FSlotData>* AMobaPlayerState::GetInventorySlots() const
{
	return &PlayerDataComponent->InventorySlots;
}


FSlotData* AMobaPlayerState::GetInventorySlotData(int32 SlotID) const
{
	return GetInventorySlots()->Find(SlotID);
}

void AMobaPlayerState::GetAllInventorySlotIDs(TArray<int32>& OutSlotIDs) const
{
	GetAllSlotIDs(*GetInventorySlots(), OutSlotIDs);
}

void AMobaPlayerState::CheckInventory(int32 SlotID) const
{
	if(FSlotData* SlotData = GetInventorySlotData(SlotID))
	{
		//如果Slot上物品数量为0，重置、清空Slot
		if(SlotData->Number == 0)
		{
			SlotData->Reset();
			PlayerDataComponent->SlotAttribute_Internal.Remove(SlotID);
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

FSlotData* AMobaPlayerState::GetSkillSlotData(int32 SlotID) const
{
	if (GetSkillSlots()->Contains(SlotID))
	{
		return &GetSkillSlots()->FindChecked(SlotID);
	}

	return nullptr;
}

void AMobaPlayerState::InitSkillSlot()
{
	if(const FCharacterAsset* CharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), PlayerDataComponent->PlayerID))
	{
		auto AddSlotToSkillBar = [&](const FCharacterSkill& Skill)
		{
			int SlotID = INDEX_NONE;
			for(auto& Tmp : *GetSkillSlots())
			{
				if(Tmp.Value.DataID == INDEX_NONE)
				{
					Tmp.Value.DataID = Skill.DataID;
					Tmp.Value.SlotIcon = Skill.Icon;
					Tmp.Value.Number = INDEX_NONE;
					Tmp.Value.CD = 0.f;
					SlotID = Tmp.Key;
					break;
				}
			}

			if(SlotID != INDEX_NONE && AddSlotAttributes(SlotID, Skill.DataID))
			{
				Client_UpdateSlot(SlotID, PlayerDataComponent->SkillSlots[SlotID]);
			}
		};

		AddSlotToSkillBar(CharacterAsset->W_Skill);
		AddSlotToSkillBar(CharacterAsset->E_Skill);
		AddSlotToSkillBar(CharacterAsset->R_Skill);
		AddSlotToSkillBar(CharacterAsset->F_Skill);
	}
}

void AMobaPlayerState::GetAllSkillSlotIDs(TArray<int32>& OutSlotIDs) const
{
	GetAllSlotIDs(*GetSkillSlots(), OutSlotIDs);
}

int32 AMobaPlayerState::GetSkillDataIDFromSlotID(int32 SlotID) const
{
	for (auto& Tmp : PlayerDataComponent->SkillSlots)
	{
		if (Tmp.Key == SlotID)
		{
			return Tmp.Value.DataID;
		}
	}

	return INDEX_NONE;
}

void AMobaPlayerState::GetInventorySlotNetPackage(FSlotDataNetPackage& OutNetPackage)
{
	GetSlotNetPackage(GetInventorySlots(), OutNetPackage);
}

void AMobaPlayerState::GetSkillSlotNetPackage(FSlotDataNetPackage& OutNetPackage)
{
	GetSlotNetPackage(GetSkillSlots(), OutNetPackage);
}

void AMobaPlayerState::GetSlotNetPackage(TMap<int32, FSlotData>* InSlots, FSlotDataNetPackage& OutNetPackage)
{
	for(auto& Tmp : *InSlots)
	{
		OutNetPackage.SlotIDs.Add(Tmp.Key);
		OutNetPackage.SlotDatas.Add(Tmp.Value);
	}
}


void AMobaPlayerState::Server_UpdateInventory_Implementation(int32 MoveSlotID, int32 TargetSlotID)
{
	if(PlayerDataComponent->InventorySlots.Contains(MoveSlotID) && PlayerDataComponent->InventorySlots.Contains(TargetSlotID))
	{
		FSlotData& MoveInventorySlotData = PlayerDataComponent->InventorySlots[MoveSlotID]; //移动到目标Slot的数据
		FSlotData& TargetInventorySlotData = PlayerDataComponent->InventorySlots[TargetSlotID]; //目标Slot原来的数据

		//如果目标Slot为空位置，直接移动
		if(TargetInventorySlotData.DataID == INDEX_NONE)
		{
			//Slot数据移动
			TargetInventorySlotData = MoveInventorySlotData;
			MoveInventorySlotData.Reset();
			
			//队列移动 和CD相关
			if(PlayerDataComponent->SlotQueue.Contains(MoveSlotID))
			{
				PlayerDataComponent->SlotQueue.Remove(MoveSlotID);
				PlayerDataComponent->SlotQueue.Add(TargetSlotID, &TargetInventorySlotData);
			}

			//属性移动
			PlayerDataComponent->SlotAttribute_Internal.SetKeyToNewKey(MoveSlotID, TargetSlotID);
		}
		else //如果目标Slot不为空，交换位置
		{
			//Slot数据交换
			Swap(MoveInventorySlotData, TargetInventorySlotData);

			//队列交换
			if(PlayerDataComponent->SlotQueue.Contains(MoveSlotID))
			{
				PlayerDataComponent->SlotQueue.Remove(MoveSlotID);
				PlayerDataComponent->SlotQueue.Add(MoveSlotID, &MoveInventorySlotData);
			}

			if(PlayerDataComponent->SlotQueue.Contains(TargetSlotID))
			{
				PlayerDataComponent->SlotQueue.Remove(TargetSlotID);
				PlayerDataComponent->SlotQueue.Add(TargetSlotID, &TargetInventorySlotData);
			}

			//属性交换
			PlayerDataComponent->SlotAttribute_Internal.SwapKey(MoveSlotID, TargetSlotID);
		}

		//通知客户端更新UI
		Client_UpdateSlot(MoveSlotID, MoveInventorySlotData);
		Client_UpdateSlot(TargetSlotID, TargetInventorySlotData);
	}
	
}

void AMobaPlayerState::Server_Use_Implementation(int32 SlotID)
{
	if(FSlotData* SlotData = GetSlotData(SlotID))
	{
		//不等于 INDEX_NONE时为 InventorySlot
		if(SlotData->Number != INDEX_NONE)
		{
			if(SlotData->Number > 0)
			{
				SlotData->Number--;
				CheckInventory(SlotID); 
				Client_UpdateSlot(SlotID, *SlotData);
			}
		}

		if(FSlotAttribute* SlotAttribute = GetSlotAttributeFromSlotID(SlotID))
		{
			SlotData->CD = SlotAttribute->CD;
			PlayerDataComponent->SlotQueue.Add(SlotID, SlotData);
			Client_StartUpdateCD(SlotID, *SlotData);

			//执行技能
			int32 SkillDataID = GetSkillDataIDFromSlotID(SlotID);
			if (SkillDataID != INDEX_NONE)
			{
				if(AMobaPawn* MobaPawn = GetPawn<AMobaPawn>())
				{
					MobaPawn->SkillAttack(SkillDataID);
				}
			}
		}
	}

	
}

void AMobaPlayerState::Client_InitInventorySlots_Implementation(const FSlotDataNetPackage& SlotDataNetPackage)
{
	//清空旧数据
	PlayerDataComponent->InventorySlots.Empty(); 

	//读取网络包，加载到InventorySlots
	for(int32 i = 0; i < SlotDataNetPackage.SlotIDs.Num(); i++)
	{
		PlayerDataComponent->InventorySlots.Add(SlotDataNetPackage.SlotIDs[i], SlotDataNetPackage.SlotDatas[i]);
	}

	//多播委托
	InitSlotDelegate.Broadcast();
}


void AMobaPlayerState::Client_InitSkillSlots_Implementation(const FSlotDataNetPackage& SlotDataNetPackage)
{
	//清空旧数据
	PlayerDataComponent->SkillSlots.Empty(); 

	//读取网络包，加载到InventorySlots
	for(int32 i = 0; i < SlotDataNetPackage.SlotIDs.Num(); i++)
	{
		PlayerDataComponent->SkillSlots.Add(SlotDataNetPackage.SlotIDs[i], SlotDataNetPackage.SlotDatas[i]);
	}

	//多播委托
	InitSlotDelegate.Broadcast();
}

void AMobaPlayerState::Client_UpdateSlot_Implementation(int32 SlotID, const FSlotData& NetSlotData)
{
	//更新Slot
	if(FSlotData* SlotData = GetSlotData(SlotID))
	{
		*SlotData = NetSlotData;
	}
	
	UpdateSlotDelegate.Broadcast(SlotID);
}

void AMobaPlayerState::Client_StartUpdateCD_Implementation(int32 SlotID, const FSlotData& NetSlotData)
{
	//更新Slot
	if(FSlotData* SlotData = GetSlotData(SlotID))
	{
		*SlotData = NetSlotData;
	}

	StartUpdateCDSlotDelegate.Broadcast(SlotID);
	
}

void AMobaPlayerState::Client_EndUpdateCD_Implementation(int32 SlotID, const FSlotData& NetSlotData)
{
	//更新Slot
	if(FSlotData* SlotData = GetSlotData(SlotID))
	{
		*SlotData = NetSlotData;
	}

	EndUpdateCDSlotDelegate.Broadcast(SlotID);
}

