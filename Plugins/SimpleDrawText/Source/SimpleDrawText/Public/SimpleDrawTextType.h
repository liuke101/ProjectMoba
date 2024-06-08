// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleDrawTextType.generated.h"

class UWidgetAnimation;

UENUM(BlueprintType)
enum class EInfoAnimType :uint8
{
	TYPE_DEFAULT					UMETA(DisplayName = "Default"),
	TYPE_GRADUALCHANGE				UMETA(DisplayName = "Gradual Change"),
	TYPE_CRIT						UMETA(DisplayName = "Crit"),
	TYPE_EXPLOSION					UMETA(DisplayName = "Explosion"),
	TYPE_GENTLE						UMETA(DisplayName = "Gentle"),
	TYPE_SMALLER					UMETA(DisplayName = "Smaller"),
	TYPE_MAX						UMETA(DisplayName = "Max"),
};
