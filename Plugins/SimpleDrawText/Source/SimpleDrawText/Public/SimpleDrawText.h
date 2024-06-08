// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class SIMPLEDRAWTEXT_API FSimpleDrawTextModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	UClass* GetDrawTextClass();
};

class FSimpleDrawTextHelp
{
public:
	static FString GetClassPaths();
	static FString GetStylePaths();

private:
	static FString GetPaths(const int32 Index = 0);
};
