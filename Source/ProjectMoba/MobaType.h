#pragma once
#include "CoreMinimal.h"
#include "MobaType.generated.h" // 生成反射信息

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
	ETT_Red,
	ETT_Blue,
	ETT_Neutral
};