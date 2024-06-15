#include "MiscData.h"

FPlayerLocation::FPlayerLocation()
{
}

FPlayerLocation::FPlayerLocation(const int64 InplayerID, const FVector& InLocation)
	: PlayerID(InplayerID)
	, Location(InLocation)
{
}

bool operator==(const FPlayerLocation& LHS, const FPlayerLocation& RHS)
{
	if(LHS.PlayerID == RHS.PlayerID && LHS.Location == RHS.Location)
	{
		return true;
	}
	return false;
}

FSlotData::FSlotData()
	: DataID(INDEX_NONE)
	, SlotIcon(nullptr)
	, CD(0.0f)
	, Number(INDEX_NONE)
	, MaxStackingQuantity(5)
	, bCancelBuy(true)
{
}

FSlotDataNetPackage::FSlotDataNetPackage()
{
}

FKillNetPackgae::FKillNetPackgae()
	: KillType(EKillType::EKT_NormalKill)
	, KillerName("DefaultKiller")
	, KilledName("DefaultKilled")
{
}

FPlayerKDANetPackage::FPlayerKDANetPackage()
	: KillNum(0)
	, DeathNum(0)
	, AssistNum(0)
	, MinionKillNum(0)
{
}

FPlayerInfoNetPackage::FPlayerInfoNetPackage(): PlayerID(INDEX_NONE), CharacterLevel(0)
{
}

FLookPlayerInfoNetPackage::FLookPlayerInfoNetPackage(): PlayerID(INDEX_NONE)
{
}

FBuffNetPackage::FBuffNetPackage(): DataID(INDEX_NONE), MaxCD(1)
{
}

FSkillLevelUpNetPackage::FSkillLevelUpNetPackage()
	: Level(INDEX_NONE)
	, SlotID(INDEX_NONE)
	, bHideAllSlot(false)
	, bEnableCurrentSlot(true)
{
	
}
