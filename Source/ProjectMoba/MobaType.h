﻿#pragma once
#include "CoreMinimal.h"
#include "MobaType.generated.h" // 生成反射信息

UENUM()
enum class ECharacterType : uint8
{
	ECT_None UMETA(DisplayName="None"),
	
	ECT_Player UMETA(DisplayName="Player"),  //玩家
	ECT_RemoteMinion UMETA(DisplayName="RemoteMinion"), // 远程小兵
	ECT_WarriorMinion UMETA(DisplayName="WarriorMinion"), // 近战小兵
	ECT_BigMinion UMETA(DisplayName="BigMinion"), // 大兵
	ECT_SuperMinion UMETA(DisplayName="SuperMinion"), // 超级兵

	//炮塔
	ECT_1st_Tower UMETA(DisplayName="1st_Tower"), // 1级塔
	ECT_2nd_Tower UMETA(DisplayName="2nd_Tower"), // 2级塔
	ECT_3rd_Tower UMETA(DisplayName="3rd_Tower"), // 3级塔
	ECT_Base_Tower UMETA(DisplayName="Base_Tower"), // 4级塔

	//野怪
	ECT_WildMonster UMETA(DisplayName="WildMonster"),
	ECT_BossMonster UMETA(DisplayName="BossMonster"),
};

UENUM() // 需要同步，所以使用UENUM引入反射
enum class ETeamType : uint8
{
	ETT_None UMETA(DisplayName="None"),
	
	ETT_Red UMETA(DisplayName="Red"),
	ETT_Blue UMETA(DisplayName="Blue"),
	ETT_Neutral UMETA(DisplayName="Neutral")
};

UENUM()
enum class ESlotKey : uint8
{
	ESK_W UMETA(DisplayName="W"),
	ESK_E UMETA(DisplayName="E"),
	ESK_R UMETA(DisplayName="R"),
	ESK_F UMETA(DisplayName="F"),
	ESK_Space UMETA(DisplayName="Space"),
	ESK_Q UMETA(DisplayName="Q"),
	ESK_1 UMETA(DisplayName="1"),
	ESK_2 UMETA(DisplayName="2"),
	ESK_3 UMETA(DisplayName="3"),
	ESK_4 UMETA(DisplayName="4"),
};

UENUM()
enum class ESlotType : uint8
{
	EST_All UMETA(DisplayName="All"),
	EST_Consumables UMETA(DisplayName="Consumables"), //消耗品
	EST_AuxiliaryProps UMETA(DisplayName="AuxiliaryProps"), //辅助道具
	EST_PhysicalAttack UMETA(DisplayName="PhysicalAttack"), //物理攻击
	EST_MagicAttack UMETA(DisplayName="MagicAttack"), //魔法攻击
	EST_PhysicalPenetration UMETA(DisplayName="PhysicalPenetration"), //物理穿透
	EST_MagicPenetration UMETA(DisplayName="MagicPenetration"), //魔法穿透
	EST_Crit UMETA(DisplayName="Crit"), //暴击
	EST_Armor UMETA(DisplayName="Armor"), //护甲
	EST_MagicResistance UMETA(DisplayName="MagicResistance"), //魔抗
	EST_AttackSpeed UMETA(DisplayName="AttackSpeed"), //攻速
	EST_Shoes UMETA(DisplayName="Shoes"), //鞋
};

UENUM()
enum class ESkillType : uint8
{
	EST_Burst UMETA(DisplayName="Burst"), // 爆发
	EST_Continuous UMETA(DisplayName="Continuous"), //持续
};