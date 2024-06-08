// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SimpleDrawTextType.h"
#include "SimpleDrawTextStyle.generated.h"

USTRUCT(BlueprintType)
struct FSimpleDrawTextStyle :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

		FSimpleDrawTextStyle();

	UPROPERTY(EditDefaultsOnly, Category = "Simple Draw Text")
		int32 ID;

	UPROPERTY(EditDefaultsOnly, Category = "Simple Draw Text")
		UTexture2D *Icon;
};
