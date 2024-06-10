#pragma once
#include "CoreMinimal.h"
#include "MobaType.generated.h" // 生成反射信息

/** 角色类型 */
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

	//商店
	ECT_HomeShop UMETA(DisplayName="HomeShop"), //基地商店
};

/** 队伍类型 */
UENUM() // 需要同步，所以使用UENUM引入反射
enum class ETeamType : uint8
{
	ETT_None UMETA(DisplayName="None"),
	ETT_Red UMETA(DisplayName="Red"),
	ETT_Blue UMETA(DisplayName="Blue"),
	ETT_Neutral UMETA(DisplayName="Neutral")
};

/** Slot键位 */
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

/** Slot类型 */
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

/** 角色属性协议 */
UENUM(Blueprintable)
enum class ECharacterAttributeType : uint8
{
	ECAT_All UMETA(DisplayName="None"), //代表更新所有数据（整包）
	ECAT_Level UMETA(DisplayName="Level"), 
	ECAT_MaxHealth UMETA(DisplayName = "MaxHealth"),
	ECAT_CurrentHealth UMETA(DisplayName = "CurrentHealth"),
	ECAT_MaxMana UMETA(DisplayName = "MaxMana"),
	ECAT_CurrentMana UMETA(DisplayName = "CurrentMana"),
	ECAT_PhysicalAttack UMETA(DisplayName = "PhysicalAttack"),
	ECAT_Armor UMETA(DisplayName = "Armor"),
	ECAT_PhysicalPenetration UMETA(DisplayName = "PhysicalPenetration"),
	ECAT_MagicAttack UMETA(DisplayName = "MagicAttack"),
	ECAT_MagicResistance UMETA(DisplayName = "MagicResistance"),
	ECAT_MagicPenetration UMETA(DisplayName = "MagicPenetration"),
	ECAT_WalkSpeed UMETA(DisplayName = "WalkSpeed"),
	ECAT_AttackSpeed UMETA(DisplayName = "AttackSpeed"),
	ECAT_MaxEXP UMETA(DisplayName = "MaxEXP"),
	ECAT_CurrentEXP UMETA(DisplayName = "CurrentEXP"),
	ECAT_CriticalRate UMETA(DisplayName = "CriticalRate"),
	//攻击范围AttackRange暂不同步，不会受装备影响
};

/** 击杀类型 */
UENUM(Blueprintable)
enum class EKillType : uint8
{
	EKT_NormalKill UMETA(DisplayName="NormalKill"), //普通击杀
	EKT_FirstBlood UMETA(DisplayName="FirstBlood"), //一血

	EKT_DoubleKill UMETA(DisplayName="DoubleKill"), //双杀
	EKT_TripleKill UMETA(DisplayName="TripleKill"), //三杀
	EKT_QuadraKill UMETA(DisplayName="QuadraKill"), //四杀
	EKT_PentaKill UMETA(DisplayName="PentaKill"), //五杀

	EKT_DaShaTeSha UMETA(DisplayName="DaShaTeSha"), //大杀特杀
	EKT_SuoXiangPiMi UMETA(DisplayName="SuoXiangPiMi"), //所向披靡
	EKT_HunShenShiDan UMETA(DisplayName="HunShenShiDan"), //浑身是胆
	EKT_YongGuanSanJun UMETA(DisplayName="YongGuanSanJun"), //勇冠三军
	EKT_YiJiDangQian UMETA(DisplayName="YiJiDangQian"), //一骑当千
	EKT_WanFuMoDi UMETA(DisplayName="WanFuMoDi"), //万夫莫敌
	EKT_JuShiWuShuang UMETA(DisplayName="JuShiWuShuang"), //举世无双
	EKT_TianXiaWuDi UMETA(DisplayName="TianXiaWuDi"), //天下无敌

	EKT_TeamDeath UMETA(DisplayName="TuanMie"), //团灭
};
