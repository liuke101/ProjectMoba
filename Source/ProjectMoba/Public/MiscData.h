// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MiscData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerLocation
{
	GENERATED_BODY()

	FPlayerLocation()
		: CharacterID(INDEX_NONE),
		  Location(FVector::ZeroVector)
	{
	};

	FPlayerLocation(const int32 InCharacterID, const FVector& InLocation)
		: CharacterID(InCharacterID),
		  Location(InLocation)
	{
	}

	UPROPERTY()
	int32 CharacterID;

	UPROPERTY()
	FVector Location;
	
};
