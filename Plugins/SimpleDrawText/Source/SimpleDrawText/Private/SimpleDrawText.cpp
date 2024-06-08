// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "SimpleDrawText.h"
#include "Actor/DrawText.h"
#include "Log/SimpleDrawTextLog.h"

#define LOCTEXT_NAMESPACE "FSimpleDrawTextModule"

void FSimpleDrawTextModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

}

void FSimpleDrawTextModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

FString FSimpleDrawTextHelp::GetPaths(const int32 Index)
{
	FString NeedPaths;
	FString DrawTextPath = FPaths::ProjectDir() / TEXT("DrawTextPath.txt");
	FString ProjectDrawTextPath = FPaths::ProjectPluginsDir() / TEXT("SimpleDrawText/DrawTextPath.txt");
	if (IFileManager::Get().FileExists(*DrawTextPath))
	{
		TArray<FString> Strings;
		FFileHelper::LoadFileToStringArray(Strings, *DrawTextPath);

		if (Strings.IsValidIndex(Index))
		{
			NeedPaths = Strings[Index];
		}
	}
	else if (IFileManager::Get().FileExists(*ProjectDrawTextPath))
	{
		ECopyResult Result = (ECopyResult)IFileManager::Get().Copy(*DrawTextPath, *ProjectDrawTextPath);
		if (Result == ECopyResult::COPY_OK)
		{
			return GetPaths(Index);
		}
		else if (Result == ECopyResult::COPY_Fail)
		{
			//??? ??
		}
		else if (Result == ECopyResult::COPY_Canceled)
		{
			//??? ȡ?
		}
	}
	else
	{
		TArray<FString> Strings;
		Strings.Add("Blueprint'/Game/DrawText_BP.DrawText_BP_C'");
		Strings.Add("/Game/Table/SimpleDrawTextStyle");
		
		FFileHelper::SaveStringArrayToFile(Strings, *DrawTextPath);
		FFileHelper::SaveStringArrayToFile(Strings, *ProjectDrawTextPath);

		if (Strings.IsValidIndex(Index))
		{
			NeedPaths = Strings[Index];
		}
	}

	return NeedPaths;
}

FString FSimpleDrawTextHelp::GetClassPaths() 
{
	return GetPaths(0);
}

FString FSimpleDrawTextHelp::GetStylePaths() 
{
	return GetPaths(1);
}

UClass* FSimpleDrawTextModule::GetDrawTextClass()
{
	/** BUG: 无效 */
	const FString ClassPath = FSimpleDrawTextHelp::GetClassPaths();
	UE_LOG(SimpleDrawTextLog, Log, TEXT("Draw text class path = %s"), *ClassPath);

	if (UClass *DrawTextClass = StaticLoadClass(ADrawText::StaticClass(), nullptr, *ClassPath))
	{
		UE_LOG(SimpleDrawTextLog, Log, TEXT("class name = %s"), *DrawTextClass->GetName());
		return DrawTextClass;
	}

	UE_LOG(SimpleDrawTextLog, Error, TEXT("Unable to read draw class."));

	return NULL;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSimpleDrawTextModule, SimpleDrawText)
