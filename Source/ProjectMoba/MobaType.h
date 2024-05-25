#pragma once
#include "CoreMinimal.h"
#include "MobaType.generated.h" // 生成反射信息

UENUM()
enum class ESkillKey : uint8
{
	ESK_W,
	ESK_E,
	ESK_R,
	ESK_F,
	ESK_Space
};

UENUM() // 需要同步，所以使用UENUM引入反射
enum class ETeamType : uint8
{
	ETT_None,
	
	ETT_Red,
	ETT_Blue,
	ETT_Neutral
};

UENUM()
enum class ECharacterType : uint8
{
	ECT_None,
	
	ECT_Player,  //玩家
	ECT_RemoteMinion, // 远程小兵
	ECT_WarriorMinion, // 近战小兵
	ECT_BigMinion, // 大兵
	ECT_SuperMinion, // 超级兵

	//炮塔
	ECT_1st_Tower, // 1级塔
	ECT_2nd_Tower, // 2级塔
	ECT_3rd_Tower, // 3级塔
	ECT_Base_Tower, // 4级塔

	//野怪
	ECT_WildMonster,
};