// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiscData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerLocation
{
	GENERATED_BODY()

	FPlayerLocation()
		: playerID(INDEX_NONE),
		  Location(FVector::ZeroVector)
	{
	};

	FPlayerLocation(const int64 InplayerID, const FVector& InLocation)
		: playerID(InplayerID),
		  Location(InLocation)
	{
	}

	UPROPERTY()
	int64 playerID;

	UPROPERTY()
	FVector Location;
	
};
