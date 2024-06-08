// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "SimpleDrawTextFunctionLibrary.h"
#include "SimpleDrawText.h"
#include "Actor/DrawText.h"
#include "Log/SimpleDrawTextLog.h"

ADrawText* USimpleDrawTextFunctionLibrary::SpawnDrawText(
	UObject* WorldContextObject,
	const FVector &InLocation,
	const FString& InText, 
	const FLinearColor& Color,
	float Percentage, 
	EInfoAnimType Type, 
	int32 PrefixStyleID,
	bool bBackground)
{
	FSimpleDrawTextModule& DrawTextModule = FModuleManager::GetModuleChecked<FSimpleDrawTextModule>("SimpleDrawText");
	
	return SpawnDrawTextByClass(
		WorldContextObject,
		DrawTextModule.GetDrawTextClass(),
		InLocation, InText, Color, Percentage, Type, PrefixStyleID, bBackground);
}

ADrawText* USimpleDrawTextFunctionLibrary::SpawnDrawTextByClass(UObject* WorldContextObject, const TSubclassOf<ADrawText>& InDrawTextClass, const FVector& InLocation, const FString& InText, const FLinearColor& Color, float Percentage, EInfoAnimType Type, int32 PrefixStyleID, bool bBackground)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FSimpleDrawTextModule& DrawTextModule = FModuleManager::GetModuleChecked<FSimpleDrawTextModule>("SimpleDrawText");
		if (ADrawText* InDrawText = World->SpawnActor<ADrawText>(InDrawTextClass, InLocation, FRotator::ZeroRotator))
		{
			InDrawText->SetTextBlock(InText, Color, Percentage, Type, PrefixStyleID, bBackground);
			UE_LOG(SimpleDrawTextLog, Log, TEXT("DrawText Object name=%s"), *InDrawText->GetName());

			return InDrawText;
		}

		UE_LOG(SimpleDrawTextLog, Error, TEXT("Unable to survive draw text object"));
	}

	return NULL;
}
