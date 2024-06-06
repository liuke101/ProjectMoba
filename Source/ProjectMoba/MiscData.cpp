﻿#include "MiscData.h"

FPlayerLocation::FPlayerLocation()
{
}

FPlayerLocation::FPlayerLocation(const int64 InplayerID, const FVector& InLocation)
	: PlayerID(InplayerID)
	, Location(InLocation)
{
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

FPlayerKillInfoNetPackage::FPlayerKillInfoNetPackage()
	: KillNum(0)
	, DeathNum(0)
	, AssistNum(0)
	, MinionKillNum(0)
{
}

FAssistPlayer::FAssistPlayer()
	: PlayerID(INDEX_NONE)
	, AssistTime(10.0f)
{
}