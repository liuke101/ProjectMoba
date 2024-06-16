// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MobaPlayerState.h"

#include "ThreadManage.h"
#include "Character/Hero/MobaHeroCharacter.h"
#include "Character/Tool/CharacterSpawnPoint.h"
#include "Common/CalculationUnit.h"
#include "Common/MethodUnit.h"
#include "Component/MobaAssistSystemComponent.h"
#include "Component/PlayerDataComponent.h"
#include "Game/MobaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectMoba/MiscData.h"
#include "Table/CharacterAsset.h"
#include "Table/SlotAttribute.h"
#include "Table/SlotAsset.h"

AMobaPlayerState::AMobaPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	PlayerDataComponent = CreateDefaultSubobject<UPlayerDataComponent>(TEXT("PlayerDataComponent"));
	
	MobaAssitSystemComponent = CreateDefaultSubobject<UMobaAssistSystemComponent>(TEXT("MobaAssitComponent"));
}

void AMobaPlayerState::BeginPlay()
{
	Super::BeginPlay();

	/** 获取基地商店位置 */
	TArray<AActor*> SpawnPointArrays;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacterSpawnPoint::StaticClass() , SpawnPointArrays);

	for(auto& Tmp : SpawnPointArrays)
	{
		if(ACharacterSpawnPoint* ShopSpawnPoint = Cast<ACharacterSpawnPoint>(Tmp))
		{
			if(ShopSpawnPoint->GetCharacterType() == ECharacterType::ECT_HomeShop && ShopSpawnPoint->GetTeamType() == PlayerDataComponent->TeamType)
			{
				HomeShopLocation = ShopSpawnPoint->GetActorLocation();
				break;
			}
			
		}
	}

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
		
		GThread::GetCoroutines().BindLambda(2.0f,[&]()
		{
			// 初始化Slot
			FSlotDataNetPackage NetInventoryPackage;
			GetInventorySlotNetPackage(NetInventoryPackage);
			Client_InitInventorySlots(NetInventoryPackage); //初始化背包

			FSlotDataNetPackage NetSkillPackage;
			GetSkillSlotNetPackage(NetSkillPackage);
			Client_InitSkillSlots(NetSkillPackage); //客户端初始化技能栏
			
			InitSkillSlot();//服务端初始化技能栏
			ShowSkillLevelUpUI();  //初始角色允许升级技能，默认技能点为1
			
			if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
			{
				//初始化属性(整包更新)
				MobaGameState->RequestUpdateCharacterAttribute(GetPlayerID(), GetPlayerID(), ECharacterAttributeType::ECAT_All);

				// 注册BUFF
				if(FCharacterAttribute* CharacterAttribute = MobaGameState->GetCharacterAttributeFromPlayerID(GetPlayerID()))
				{
					CharacterAttribute->SetBuff(PlayerDataComponent->SlotAttributes.ToSharedRef());
				}
			}

			
		});
	}
}

void AMobaPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GetLocalRole() == ROLE_Authority)
	{
		Tick_Server_AddGoldPerSecond(DeltaSeconds);
		Tick_Server_CheckDistanceFromHomeShop(DeltaSeconds);
		Tick_Server_UpdateBuff(DeltaSeconds);
		
		Tick_Server_UpdateSlotCD(DeltaSeconds);
	}
}


void AMobaPlayerState::Tick_Server_AddGoldPerSecond(float DeltaSeconds)
{
	GoldTime+=DeltaSeconds;
	if(GoldTime >= 1.f)
	{
		GoldTime = 0.f;
		PlayerDataComponent->Gold += 2;
	}
}

void AMobaPlayerState::Tick_Server_CheckDistanceFromHomeShop(float DeltaSeconds)
{
	float Distance = FVector::Dist(HomeShopLocation, GetPawn()->GetActorLocation());
	TArray<int32> RemoveSlotIDs;
	for(auto& Tmp : PlayerDataComponent->SaleSlotDistanceQueue)
	{
		//如果角色距离商店大于1000，则无法原价取消，只能打折出售
		if(Distance >= 1000.0f)
		{
			Tmp.Value->bCancelBuy = false;
			Client_UpdateSlot(Tmp.Key, *Tmp.Value);

			//计算完毕，添加到移除数组
			RemoveSlotIDs.Add(Tmp.Key); 
		}
	}
	// 从队列中移除已经处理的Slot
	for(auto& RemoveSlotID : RemoveSlotIDs)
	{
		PlayerDataComponent->SaleSlotDistanceQueue.Remove(RemoveSlotID);
	}
}

void AMobaPlayerState::Tick_Server_UpdateBuff(float DeltaSeconds)
{
	/** Buff 计算 */
	BuffTime += DeltaSeconds;
	if(BuffTime >= 1.f)
	{
		if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
		{
			if(FCharacterAttribute* CharacterAttribute = MethodUnit::GetCharacterAttributeFromPlayerID(GetWorld(), GetPlayerID()))
			{
				if(AMobaPawn* MobaPawn = Cast<AMobaPawn>(GetPawn()))
				{
					if(AMobaHeroCharacter* MobaHero = MobaPawn->GetControlledMobaHero())
					{
						BuffTime = 0.f;

						//lambda 获取属性变化量
						auto GetAttributeChangeValue = [&](const FSlotAttributeValue& InSlotAttribute, float &InNewValue, const ECharacterAttributeType& CharacterAttributeType)->float
						{
							float LastValue = InNewValue;
							
							InNewValue = CalculationUnit::GetSlotAttributeValue(InSlotAttribute, InNewValue);
							if(InNewValue != LastValue)
							{
								//如果属性变化，则更新属性
								MobaGameState->RequestUpdateCharacterAttribute(PlayerDataComponent->PlayerID, PlayerDataComponent->PlayerID, CharacterAttributeType);
							}

							return InNewValue - LastValue;
						};

						//根据属性变化量 更新UI
						for(auto& Tmp : PlayerDataComponent->SlotCDQueue)
						{
							if(const FSlotAttribute* BuffSlotAttribute = GetSlotAttributeFromSlotID(Tmp.Key))
							{
								//只计算持续性Buff
								if(BuffSlotAttribute->AttributeType == ESlotAttributeType::ESAT_Continuous)
								{
									//广播增益数值
									float HealthChange = GetAttributeChangeValue(BuffSlotAttribute->CurrentHealth, CharacterAttribute->CurrentHealth, ECharacterAttributeType::ECAT_CurrentHealth);
									if(HealthChange > 0)
									{
										MobaHero->Multicast_SpwanDrawDamageText(HealthChange,0.1f, FColor::Green, MobaHero->GetActorLocation());
									}
									else if(HealthChange < 0)
									{
										MobaHero->Multicast_SpwanDrawDamageText(HealthChange,0.1f, FColor::Red, MobaHero->GetActorLocation());
									}

									float ManaChange = GetAttributeChangeValue(BuffSlotAttribute->CurrentMana, CharacterAttribute->CurrentMana, ECharacterAttributeType::ECAT_CurrentMana);
									if(ManaChange > 0)
									{
										MobaHero->Multicast_SpwanDrawDamageText(ManaChange,0.1f, FColor::Blue, MobaHero->GetActorLocation());
									}
									else if(ManaChange < 0)
									{
										MobaHero->Multicast_SpwanDrawDamageText(ManaChange,0.1f, FColor::Purple, MobaHero->GetActorLocation());
									}

									//更新状态栏
									if(HealthChange != 0 || ManaChange != 0)
									{
										MobaHero->Multicast_StatusBar(CharacterAttribute->GetHealthPercent(), CharacterAttribute->GetManaPercent());
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void AMobaPlayerState::Tick_Server_UpdateSlotCD(float DeltaSeconds)
{
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		TArray<int32> RemoveSlotIDs;
	
		for(auto& Tmp : PlayerDataComponent->SlotCDQueue)
		{
			Tmp.Value->CD -= DeltaSeconds;
			if(Tmp.Value->CD <= 0.f)
			{
				Tmp.Value->CD = 0.f;
				if(const FSlotAsset* SlotAsset = GetSlotAssetFromDataID(Tmp.Value->DataID))
				{
				
				}
				if(const FSlotAttribute* SlotAttribute = GetSlotAttributeFromSlotID(Tmp.Key))
				{
					//持续型物品buff
					if(SlotAttribute->AttributeType == ESlotAttributeType::ESAT_Continuous)
					{
						//只结束物品buff
						MobaGameState->Multicast_EndBuff(GetPlayerID(), SlotAttribute->DataID); 
					}
				
					//检测数量
					CheckInventory(Tmp.Key);

					//客户端矫正CD
					Client_EndUpdateCD(Tmp.Key, *Tmp.Value);
				}
				//计算完毕，添加到移除数组
				RemoveSlotIDs.Add(Tmp.Key);
			}
		}

		//已完成CD计算的Slot从CD队列中移除。
		for(auto& RemoveSlotID : RemoveSlotIDs)
		{
			PlayerDataComponent->SlotCDQueue.Remove(RemoveSlotID);
		}
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

FSlotAttribute* AMobaPlayerState::GetSlotAttributeFromDataID(const int32 DataID)
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
	if(PlayerDataComponent->SlotAttributes->Contains(SlotID))
	{
		return (*PlayerDataComponent->SlotAttributes)[SlotID];
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

bool AMobaPlayerState::IsContinualSlotAttribute(int32 DataID)
{
	if(const FSlotAttribute* SlotAttribute = GetSlotAttributeFromDataID(DataID))
	{
		return SlotAttribute->AttributeType == ESlotAttributeType::ESAT_Continuous;
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
	//根据DataID获取SlotAttribute
	if(const FSlotAttribute* SlotAttribute = GetSlotAttributeFromDataID(DataID))
	{
		return AddSlotAttributes(SlotID, SlotAttribute);
	}
	return false;
}

bool AMobaPlayerState::AddSlotAttributes(int32 SlotID, const FSlotAttribute* SlotAttribute) 
{
	if(SlotAttribute)
	{
		//如果不为空，直接替换
		if(PlayerDataComponent->SlotAttributes->Contains(SlotID))
		{
			*(*PlayerDataComponent->SlotAttributes.Get())[SlotID] = *SlotAttribute;
		}
		else //否则直接添加
		{
			PlayerDataComponent->SlotAttributes->Add(SlotID, *SlotAttribute); //注意这里解引用了，所以SlotAttribute里面存的是拷贝
		}

		//如果属性类型为持续性，则认为是Buff, 通知客户端更新BuffBar
		if(SlotAttribute->AttributeType == ESlotAttributeType::ESAT_Continuous)
		{
			if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
			{
				MobaGameState->Multicast_UpdateBuff(GetPlayerID(), SlotAttribute->DataID, SlotAttribute->CD.Value); 
			}
		}

		//设置升级的属性, 如何升级通过AddLevelDataID读表获取
		if(SlotAttribute->AddLevelDataID != INDEX_NONE)
		{
			if(const FSlotAttribute* AddLevelAttribute = GetSlotAttributeFromDataID(SlotAttribute->AddLevelDataID))
			{
				if(PlayerDataComponent->SlotAttributes->Contains(SlotID))
				{
					(*PlayerDataComponent->SlotAttributes)[SlotID]->AddLevelAttribute = AddLevelAttribute;
				}
			}
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
		if(!PlayerDataComponent->SlotAttributes->Contains(RandSlotID))
		{
			PlayerDataComponent->SlotAttributes->Add(RandSlotID, *SlotAttribute);
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
	if(const FSlotAsset* SlotAsset = GetSlotAssetFromDataID(DataID))
	{
		bool bIsExist = IsExistInInventory(DataID);
		bool bIsConsumables = SlotAsset->SlotType.Contains(ESlotType::EST_Consumables);

		//lambda
		auto AddSlot = [&]()->int32
		{
			for(auto& Tmp : *GetInventorySlots())
			{
				//找空位
				if(Tmp.Value.DataID == INDEX_NONE)
				{
					//如果当前仓库中不存在该物品，且为消耗品
					if(!bIsExist && bIsConsumables)
					{
						Tmp.Value.Number = 1; 
					}
					
					Tmp.Value.DataID = SlotAsset->DataID; 
					Tmp.Value.SlotIcon = SlotAsset->SlotIcon;
					
					return Tmp.Key; 
				}
			}
			return INDEX_NONE;
		};

		int32 SlotID = INDEX_NONE;
		if(bIsConsumables) //如果是消耗品
		{
			if (bIsExist) //如果已经存在背包, 数量+1
			{
				//遍历所有Slot
				for(auto& Tmp : *GetInventorySlots())
				{
					if(SlotAsset->DataID == Tmp.Value.DataID && Tmp.Value.DataID != INDEX_NONE)
					{
						//这里要加限制，否则会超过最大堆叠数量
						if(Tmp.Value.Number < Tmp.Value.MaxStackingQuantity)
						{
							Tmp.Value.Number++;

							//通知本机客户端，更新Slot
							Client_UpdateSlot(Tmp.Key, Tmp.Value);
							return true; // 只会添加到第一个符合条件的Slot
						}
					}
				}
			}
			else //如果不存在背包，添加到新的Slot
			{
				SlotID = AddSlot();
			}
		}
		else //如果不是消耗品，直接添加到新的Slot
		{
			SlotID = AddSlot();
		}
		
		
		if(SlotID != INDEX_NONE) //添加到Inventory
		{
			bool bAttribute = true;
			if(!IsContinualSlotAttribute(DataID))
			{
				bAttribute = AddSlotAttributes(SlotID, DataID);
			}
			
			if(bAttribute)
			{
				//通知本机客户端，更新Slot
				Client_UpdateSlot(SlotID, PlayerDataComponent->InventorySlots[SlotID]);
			}
		}
		return true;
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

bool AMobaPlayerState::IsValidInventorySlot(int32 SlotID) const
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

bool AMobaPlayerState::IsExistInInventory(int32 ItemDataID) const
{
	for(auto& Tmp : *GetInventorySlots())
	{
		if(Tmp.Value.DataID == ItemDataID)
		{
			if(Tmp.Value.Number < Tmp.Value.MaxStackingQuantity) //限制物品堆叠数量
			{
				return true;
			}
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
			PlayerDataComponent->SlotAttributes->Remove(SlotID);
		}
	}
}

void AMobaPlayerState::Sell(int32 SlotID, int32 DataID, float Discount)
{
	if(const FSlotAsset* SlotAsset = GetSlotAssetFromDataID(DataID))
	{
		if(FSlotData* SlotData = GetInventorySlotData(SlotID))
		{
			//如果是消耗品
			if(SlotData->Number > 0)
			{
				SlotData->Number--;
				CheckInventory(SlotID);
			}
			else //如果是装备
			{
				SlotData->Reset();
				PlayerDataComponent->SlotAttributes->Remove(SlotID);
			}

			//打折出售
			PlayerDataComponent->Gold += SlotAsset->SlotGold * Discount; 

			//通知客户端更新UI
			Client_UpdateSlot(SlotID, *SlotData);
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
		//lambda
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

			if(SlotID != INDEX_NONE)
			{
				if(AddSlotAttributes(SlotID, Skill.DataID))
				{
					Client_UpdateSlot(SlotID, PlayerDataComponent->SkillSlots[SlotID]);
				}
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

void AMobaPlayerState::AddSkillSlotPoint(int32 SlotID)
{
	int32 SKillDataID = GetSkillDataIDFromSlotID(SlotID);
	if(SKillDataID != INDEX_NONE)
	{
		if(FSlotAttribute* SkillSlotAttribute= GetSlotAttributeFromDataID(SKillDataID))
		{
			SkillSlotAttribute->UpdateLevel();
		}
	}
}

void AMobaPlayerState::ShowSkillLevelUpUI()
{
	//收集要关闭升级按钮的SkillSlotID
	TArray<int32> SlotIDs;

	for(auto& Tmp : *GetSkillSlots())
	{
		if(FSlotAttribute* SlotAttribute = GetSlotAttributeFromDataID(Tmp.Value.DataID))
		{
			//如果已经满级，需要关闭升级按钮
			if(SlotAttribute->Level >= 3)
			{
				SlotIDs.Add(Tmp.Key);
				continue;
			}
			//如果没有满级
			else
			{
				//如果有限制条件
				if(SlotAttribute->LimitCondition.IsValidIndex(0))
				{
					int32 LimitLevel = SlotAttribute->LimitCondition[0];
					//如果不满足限制等级，需要关闭升级按钮
					if(SlotAttribute->Level < LimitLevel)
					{
						SlotIDs.Add(Tmp.Key);
						continue;
					}
				}
			}
		}
	}
	
	Client_ShowSkillLevelUp(SlotIDs);
}

TArray<FAssistPlayer> AMobaPlayerState::GetAssistPlayers() const
{
	return MobaAssitSystemComponent->GetAssistPlayers();
}

void AMobaPlayerState::AddAssistPlayer(const int64& InPlayerID) const
{
	MobaAssitSystemComponent->AddAssistPlayer(InPlayerID);
}

const FAssistPlayer* AMobaPlayerState::GetLastAssistPlayer() const
{
	return MobaAssitSystemComponent->GetLastAssistPlayer();
}

void AMobaPlayerState::UpdateCharacterInfo(const int64& InPlayerID)
{
	//TopPanel 背包数据
	FLookPlayerInfoNetPackage LookPlayerInfoNetPackage;
	LookPlayerInfoNetPackage.PlayerID = InPlayerID;

	//Buff数据
	TArray<FBuffNetPackage> BuffNetPackages;
	
	MethodUnit::ServerCallAllPlayerState<AMobaPlayerState>(GetWorld(), [&](AMobaPlayerState* MobaPlayerState)
	{
		if(MobaPlayerState->GetPlayerID() == InPlayerID)
		{
			//收集背包数据
			MobaPlayerState->GetInventorySlotNetPackage(LookPlayerInfoNetPackage.SlotDataNetPackage); 

			//收集BUff数据
			GetBuffNetPackages(BuffNetPackages);
			
			return MethodUnit::EServerCallType::ECT_ProgressComplete;
		}
		return MethodUnit::EServerCallType::ECT_InProgress;
	});
	
	Client_UpdateCharacterInfoTopPanel(LookPlayerInfoNetPackage);

	if(!BuffNetPackages.IsEmpty())
	{
		Client_UpdateBuffInfo(BuffNetPackages);
	}
	
	//更新属性(通过发送整包）
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		MobaGameState->RequestUpdateCharacterAttribute(GetPlayerID(), InPlayerID, ECharacterAttributeType::ECAT_All);
	}

	
	
}

void AMobaPlayerState::GetPlayerKDANetPackage(FPlayerKDANetPackage& OutPlayerKDANetPackage)
{
	OutPlayerKDANetPackage.KillNum = GetPlayerDataComponent()->KillNum;
	OutPlayerKDANetPackage.AssistNum = GetPlayerDataComponent()->AssistNum;
	OutPlayerKDANetPackage.DeathNum = GetPlayerDataComponent()->DeathNum;
	OutPlayerKDANetPackage.MinionKillNum = GetPlayerDataComponent()->MinionKillNum;
}

void AMobaPlayerState::UpdateKDAInfo()
{
	FPlayerKDANetPackage KDANetPackage;
	GetPlayerKDANetPackage(KDANetPackage);
	Client_UpdateKDAInfo(KDANetPackage);
}

void AMobaPlayerState::AddGold(int32 Gold)
{
	PlayerDataComponent->Gold += Gold;
	if(AMobaHeroCharacter* MobaHeroCharacter = GetPawn<AMobaPawn>()->GetControlledMobaHero())
	{
		MobaHeroCharacter->Multicast_SpwanDrawGoldText(Gold, 0.01f, FLinearColor::Yellow, MobaHeroCharacter->GetActorLocation());
	}
}


void AMobaPlayerState::Client_UpdatePlayerData_Implementation(const int64& InPlayerID)
{
	PlayerDataComponent->PlayerID = InPlayerID; //客户端本机玩家获取PlayerID
}

void AMobaPlayerState::GetInventorySlotNetPackage(FSlotDataNetPackage& OutNetPackage)
{
	GetSlotNetPackage(GetInventorySlots(), OutNetPackage);
}

void AMobaPlayerState::GetSkillSlotNetPackage(FSlotDataNetPackage& OutNetPackage)
{
	GetSlotNetPackage(GetSkillSlots(), OutNetPackage);
}

void AMobaPlayerState::GetBuffNetPackages(TArray<FBuffNetPackage>& OutBuffNetPackages)
{
	if(PlayerDataComponent)
	{
		for(auto& Tmp : PlayerDataComponent->SlotCDQueue)
		{
			if(const FSlotAttribute* SlotAttribute = GetSlotAttributeFromSlotID(Tmp.Key))
			{
				if(SlotAttribute->AttributeType == ESlotAttributeType::ESAT_Continuous)
				{
					FBuffNetPackage BuffNetPackage;
					BuffNetPackage.DataID = SlotAttribute->DataID;
					BuffNetPackage.MaxCD = SlotAttribute->CD.Value;
					OutBuffNetPackages.Add(BuffNetPackage);
				}
			}
		}
	}
}

void AMobaPlayerState::GetSlotNetPackage(TMap<int32, FSlotData>* InSlots, FSlotDataNetPackage& OutNetPackage)
{
	for(auto& Tmp : *InSlots)
	{
		OutNetPackage.SlotIDs.Add(Tmp.Key);
		OutNetPackage.SlotDatas.Add(Tmp.Value);
	}
}

void AMobaPlayerState::Server_UpdateSkillLevel_Implementation(int32 SlotID)
{
	//lambda 获取角色等级
	auto GetCharacterLevel = [&]()->int32
	{
		if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
		{
			if(FCharacterAttribute* CharacterAttribute = MobaGameState->GetCharacterAttributeFromPlayerID(GetPlayerID()))
			{
				return CharacterAttribute->Level;
			}
		}
		return INDEX_NONE;
	};

	//如果有技能点
	if(GetPlayerDataComponent()->SkillPoint >= 1)
	{
		if(FSlotAttribute* SkillSlotAttribute = GetSlotAttributeFromSlotID(SlotID))
		{
			if(SkillSlotAttribute->Level < 3) //技能最大等级为3
			{
				GetPlayerDataComponent()->SkillPoint--;
				SkillSlotAttribute->UpdateLevel();

				//收集数据包
				FSkillLevelUpNetPackage SkillLevelUpNetPackage;
				SkillLevelUpNetPackage.SlotID = SlotID;
				SkillLevelUpNetPackage.Level = SkillSlotAttribute->Level;

				//BUG:大招UI无变化
				//有限制条件，即大招
				if(!SkillSlotAttribute->LimitCondition.IsEmpty())
				{
					int32 LimitLevel = SkillSlotAttribute->LimitCondition[0];
					int32 CharacterLevel = GetCharacterLevel();
					
					//当角色等级达到限制等级时
					if(CharacterLevel >= LimitLevel)
					{
						SkillSlotAttribute->LimitCondition.Remove(LimitLevel);

						//没有技能点，通知客户端全部隐藏升级界面
						if(GetPlayerDataComponent()->SkillPoint <= 0)
						{
							SkillLevelUpNetPackage.bHideAllSlot = true;
							SkillLevelUpNetPackage.bEnableCurrentSlot = true;
						}
						else //有可用技能点
						{
							if(SkillSlotAttribute->LimitCondition.IsValidIndex(0))
							{
								LimitLevel = SkillSlotAttribute->LimitCondition[0];
								if(CharacterLevel >= LimitLevel)
								{
									SkillLevelUpNetPackage.bHideAllSlot = false;
									SkillLevelUpNetPackage.bEnableCurrentSlot = true;
								}
								else
								{
									SkillLevelUpNetPackage.bHideAllSlot = false;
									SkillLevelUpNetPackage.bEnableCurrentSlot = false;
								}
							}
							else 
							{
								SkillLevelUpNetPackage.bHideAllSlot = false;
								SkillLevelUpNetPackage.bEnableCurrentSlot = false;
							}
						}
					}
				
				}
				else //没有限制条件，即小技能
				{
					if(GetPlayerDataComponent()->SkillPoint <= 0)
					{
						SkillLevelUpNetPackage.bHideAllSlot = true;
						SkillLevelUpNetPackage.bEnableCurrentSlot = true;
					}
					else
					{
						SkillLevelUpNetPackage.bHideAllSlot = false;
						SkillLevelUpNetPackage.bEnableCurrentSlot = true;
					}
				}

				Client_UpdateSkillLevel(SkillLevelUpNetPackage);
			}
		}
	}
}

void AMobaPlayerState::Client_ShowSkillLevelUp_Implementation(const TArray<int32>& SlotIDs)
{
	if(!ShowSkillLevelUpDelegate.ExecuteIfBound(SlotIDs))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("未绑定ShowSkillLevelUpDelegate委托"));
		
		//没绑定就延迟后再次尝试，直到成功
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			Client_ShowSkillLevelUp(SlotIDs);
		});
	}
}

void AMobaPlayerState::Client_UpdateSkillLevel_Implementation(const FSkillLevelUpNetPackage& SkillLevelUpNetPackage)
{
	if(!UpdateSkillLevelDelegate.ExecuteIfBound(SkillLevelUpNetPackage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("未绑定UpdateSkillLevelDelegate委托"));
		
		//没绑定就延迟后再次尝试，直到成功
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			Client_UpdateSkillLevel(SkillLevelUpNetPackage);
		});
	}
}

void AMobaPlayerState::Client_UpdateBuffInfo_Implementation(const TArray<FBuffNetPackage>& BuffNetPackages)
{
	if(!BuffInfoDelegate.ExecuteIfBound(BuffNetPackages))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("未绑定BuffInfoDelegate委托"));
		
		//没绑定就延迟后再次尝试，直到成功
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			Client_UpdateBuffInfo(BuffNetPackages);
		});
	}
}

void AMobaPlayerState::Server_ReleaseSkillKey_Implementation()
{
	if(AMobaPawn* MobaPawn = Cast<AMobaPawn>(GetPawn()))
	{
		if(AMobaHeroCharacter* MobaHeroCharacter = Cast<AMobaHeroCharacter>(MobaPawn->GetControlledMobaHero()))
		{
			MobaHeroCharacter->PressSkillKey = false;
		}
	}
}

void AMobaPlayerState::Client_UpdateTeamKillCount_Implementation(const int32& FriendlyKillCount,
                                                                 const int32& EnemyKillCount)
{
	if(!TeamKillCountDelegate.ExecuteIfBound(FriendlyKillCount, EnemyKillCount))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("未绑定TeamKillCountDelegate委托"));
		
		//没绑定就延迟后再次尝试，直到成功
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			Client_UpdateTeamKillCount(FriendlyKillCount, EnemyKillCount);
		});
	}
}

void AMobaPlayerState::Client_UpdateKDAInfo_Implementation(const FPlayerKDANetPackage& PlayerKDANetPackage)
{
	if(!KDAInfoDelegate.ExecuteIfBound(PlayerKDANetPackage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("未绑定KDAInfoDelegate委托"));
		
		//没绑定就延迟后再次尝试，直到成功
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			Client_UpdateKDAInfo(PlayerKDANetPackage);
		});
		
	}
}

void AMobaPlayerState::Server_RequestAllPlayerTeamInfos_Implementation()
{
	//收集队伍信息
	TArray<FPlayerTeamNetPackage> PlayerTeamNetPackages;
	
	MethodUnit::ServerCallAllPlayerState<AMobaPlayerState>(GetWorld(), [&](AMobaPlayerState* MobaPlayerState)
	{
		FPlayerTeamNetPackage PlayerTeamNetPackage;

		// 收集玩家信息
		PlayerTeamNetPackage.PlayerInfoNetPackage.PlayerID = MobaPlayerState->GetPlayerID();
		
		if(const FCharacterAsset* CharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(
			GetWorld(), MobaPlayerState->GetPlayerID()))
		{
			PlayerTeamNetPackage.PlayerInfoNetPackage.PlayerIcon = CharacterAsset->CharacterIcon;
		}
		
		if(const FCharacterAttribute* CharacterAttribute = MethodUnit::GetCharacterAttributeFromPlayerID(
				GetWorld(), MobaPlayerState->GetPlayerID()))
		{
			PlayerTeamNetPackage.PlayerInfoNetPackage.CharacterLevel = CharacterAttribute->Level;
		}

		//收集KDA信息
		MobaPlayerState->GetPlayerKDANetPackage(PlayerTeamNetPackage.PlayerInfoNetPackage.PlayerKDANetPackage);

		//收集背包数据
		for(auto& Tmp : *MobaPlayerState->GetInventorySlots())
		{
			PlayerTeamNetPackage.SlotDataNetPackage.SlotIDs.Add(Tmp.Key);
			PlayerTeamNetPackage.SlotDataNetPackage.SlotDatas.Add(Tmp.Value);
		}

		
		PlayerTeamNetPackages.Add(PlayerTeamNetPackage);
		
		return MethodUnit::EServerCallType::ECT_InProgress;
	});

	//发送网络传输包到客户端，客户端进行响应
	Client_ResponseAllPlayerTeamInfos(PlayerTeamNetPackages);
}

void AMobaPlayerState::Client_ResponseAllPlayerTeamInfos_Implementation(
	const TArray<FPlayerTeamNetPackage>& PlayerTeamNetPackage)
{
	if(!TeamInfoDelegate.ExecuteIfBound(PlayerTeamNetPackage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("未绑定TeamInfoDelegate委托"));
		
		//没绑定就延迟后再次尝试，直到成功
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			Client_ResponseAllPlayerTeamInfos(PlayerTeamNetPackage);
		});
	}
}

void AMobaPlayerState::Client_UpdateKillMessage_Implementation(const FKillNetPackgae& KillNetPackgae)
{
	if(!KillMessageDelegate.ExecuteIfBound(KillNetPackgae))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("未绑定KillMessageDelegate委托"));
		
		//没绑定就延迟后再次尝试，直到成功
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			Client_UpdateKillMessage(KillNetPackgae);
		});
	}
}


void AMobaPlayerState::Client_ResponseUpdateCharacterAttribute_Implementation(int64 InPlayerID, const ECharacterAttributeType CharacterAttributeType, float Value)
{
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		MobaGameState->ResponseUpdateCharacterAttribute(InPlayerID, CharacterAttributeType, Value);
	}
}

void AMobaPlayerState::Client_ResponseUpdateAllCharacterAttributes_Implementation(int64 InPlayerID,
	const FCharacterAttribute& CharacterAttribute)
{
	if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
	{
		MobaGameState->ResponseUpdateAllCharacterAttributes(InPlayerID, CharacterAttribute);
	}
}

void AMobaPlayerState::Client_UpdateCharacterInfoTopPanel_Implementation(const FLookPlayerInfoNetPackage& LookPlayerInfoNetPackage)
{
	if(!LookPlayerInfoDelegate.ExecuteIfBound(LookPlayerInfoNetPackage))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("未绑定BindPlayerIDDelegate委托"));
		
		//没绑定就延迟后再次尝试，直到成功
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			Client_UpdateCharacterInfoTopPanel(LookPlayerInfoNetPackage);
		});
	}

	
}

void AMobaPlayerState::Client_HideCharacterInfoTopPanel_Implementation()
{
	if(!HideTopPanelDelegate.ExecuteIfBound())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("未绑定HideTopPanelDelegate委托"));

		//没绑定就延迟后再次尝试，直到成功
		GThread::GetCoroutines().BindLambda(0.3f, [&]()
		{
			Client_HideCharacterInfoTopPanel();
		});
	}
}


void AMobaPlayerState::Server_Buy_Implementation(int32 DataID)
{
	if(const FSlotAsset* SlotAsset = GetSlotAssetFromDataID(DataID))
	{
		if(PlayerDataComponent->Gold >= SlotAsset->SlotGold)
		{
			if(AddSlotToInventory(DataID))
			{
				PlayerDataComponent->Gold -= SlotAsset->SlotGold;
			}
		}
	}
}

void AMobaPlayerState::Server_Sell_Implementation(int32 SlotID, int32 DataID)
{
	//5折出售
	Sell(SlotID, DataID, 0.5f);
}

void AMobaPlayerState::Server_CancelBuy_Implementation(int32 SlotID, int32 DataID)
{
	//全额退款
	Sell(SlotID, DataID, 1.0f);
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
			if(PlayerDataComponent->SlotCDQueue.Contains(MoveSlotID))
			{
				PlayerDataComponent->SlotCDQueue.Remove(MoveSlotID);
				PlayerDataComponent->SlotCDQueue.Add(TargetSlotID, &TargetInventorySlotData);
			}

			//属性移动
			PlayerDataComponent->SlotAttributes->SetKeyToNewKey(MoveSlotID, TargetSlotID);
		}
		else //如果目标Slot不为空，交换位置
		{
			//Slot数据交换
			Swap(MoveInventorySlotData, TargetInventorySlotData);

			//队列交换
			if(PlayerDataComponent->SlotCDQueue.Contains(MoveSlotID))
			{
				PlayerDataComponent->SlotCDQueue.Remove(MoveSlotID);
				PlayerDataComponent->SlotCDQueue.Add(MoveSlotID, &MoveInventorySlotData);
			}

			if(PlayerDataComponent->SlotCDQueue.Contains(TargetSlotID))
			{
				PlayerDataComponent->SlotCDQueue.Remove(TargetSlotID);
				PlayerDataComponent->SlotCDQueue.Add(TargetSlotID, &TargetInventorySlotData);
			}

			//属性交换
			PlayerDataComponent->SlotAttributes->SwapKey(MoveSlotID, TargetSlotID);
		}

		//通知客户端更新UI
		Client_UpdateSlot(MoveSlotID, MoveInventorySlotData);
		Client_UpdateSlot(TargetSlotID, TargetInventorySlotData);
	}
	
}

void AMobaPlayerState::Server_Use_Implementation(int32 SlotID)
{
	FSlotData* SlotData = GetInventorySlotData(SlotID);

	//如果SlotData为空，说明是技能，重新获取技能数据
	if(!SlotData)
	{
		SlotData = GetSkillSlotData(SlotID);
		if(SlotData)
		{
			if(AMobaGameState* MobaGameState = MethodUnit::GetMobaGameState(GetWorld()))
			{
				if(FCharacterAttribute* CharacterAttribute = MobaGameState->GetCharacterAttributeFromPlayerID(GetPlayerID()))
				{
					if(FSlotAttribute* SkillSlotAttribute = GetSlotAttributeFromSlotID(SlotID))
					{
						//如果技能消耗生命值
						if(CharacterAttribute->CostHealth(SkillSlotAttribute->HealthCost))
						{
							//更新生命值
							MobaGameState->RequestUpdateCharacterAttribute( GetPlayerID(),  GetPlayerID(),ECharacterAttributeType::ECAT_CurrentHealth);
							
							if(AMobaHeroCharacter* MobaHeroCharacter = GetPawn<AMobaPawn>()->GetControlledMobaHero())
							{
								MobaGameState->Multicast_CharacterAttributeChanged(MobaHeroCharacter, ECharacterAttributeType::ECAT_CurrentHealth, CharacterAttribute->GetHealthPercent());
							}
						}
						//如果技能消耗法力值
						else if(CharacterAttribute->CostMana(SkillSlotAttribute->ManaCost))
						{
							MobaGameState->RequestUpdateCharacterAttribute( GetPlayerID(),  GetPlayerID(),ECharacterAttributeType::ECAT_CurrentMana);
							
							if(AMobaHeroCharacter* MobaHeroCharacter = GetPawn<AMobaPawn>()->GetControlledMobaHero())
							{
								MobaGameState->Multicast_CharacterAttributeChanged(MobaHeroCharacter, ECharacterAttributeType::ECAT_CurrentMana, CharacterAttribute->GetManaPercent());
							}
						}
						else//蓝量低于技能消耗法力值，则无法释放技能
						{
							//TODO: 提示音
							return;
						}
					}
				}
			}
		}
	}
	
	if(SlotData)
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

		if(IsContinualSlotAttribute(SlotData->DataID))
		{
			AddSlotAttributes(SlotID, SlotData->DataID);
		}

		if(const FSlotAttribute* SlotAttribute = GetSlotAttributeFromSlotID(SlotID))
		{
			SlotData->CD = SlotAttribute->CD.Value;
			PlayerDataComponent->SlotCDQueue.Add(SlotID, SlotData);
			Client_StartUpdateCD(SlotID, *SlotData);

			//执行技能
			int32 SkillDataID = GetSkillDataIDFromSlotID(SlotID);
			if (SkillDataID != INDEX_NONE)
			{
				if(AMobaPawn* MobaPawn = GetPawn<AMobaPawn>())
				{
					if(AMobaHeroCharacter* MobaHeroCharacter = Cast<AMobaHeroCharacter>(MobaPawn->GetControlledMobaHero()))
					{
						MobaHeroCharacter->PressSkillKey = true; //按下技能键
					}
					
					MobaPawn->SkillAttack(SkillDataID);
				}
			}
		}
	}

	
}

void AMobaPlayerState::Client_InitInventorySlots_Implementation(const FSlotDataNetPackage& SlotDataNetPackage)
{
	//客户端缓存数据
	for(int32 i = 0; i < SlotDataNetPackage.SlotIDs.Num(); i++)
	{
		if(PlayerDataComponent->InventorySlots.Contains(SlotDataNetPackage.SlotIDs[i]))
		{
			PlayerDataComponent->InventorySlots[SlotDataNetPackage.SlotIDs[i]] = SlotDataNetPackage.SlotDatas[i];
		}
		else
		{
			PlayerDataComponent->InventorySlots.Add(SlotDataNetPackage.SlotIDs[i], SlotDataNetPackage.SlotDatas[i]);
		}
	}

	//多播委托
	InitSlotDelegate.Broadcast(SlotDataNetPackage.SlotIDs);
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
	InitSlotDelegate.Broadcast(SlotDataNetPackage.SlotIDs);
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

