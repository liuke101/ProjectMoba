// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleDrawTextType.h"
#include "SimpleDrawTextFunctionLibrary.generated.h"

class ADrawText;
/**
 * 
 */
UCLASS()
class SIMPLEDRAWTEXT_API USimpleDrawTextFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	* Generate the corresponding drawing font.
	*/
	UFUNCTION(BlueprintCallable, Category = SimpleDrawText, meta = (WorldContext = WorldContextObject))
	static ADrawText* SpawnDrawText(UObject *WorldContextObject, const FVector& InLocation, const FString& InText, const FLinearColor& Color, float Percentage, EInfoAnimType Type = EInfoAnimType::TYPE_DEFAULT, int32 PrefixStyleID = -1, bool bBackground = false);
	
	/**
	* Generate the corresponding drawing font.
	*/
	UFUNCTION(BlueprintCallable, Category = SimpleDrawText, meta = (WorldContext = WorldContextObject))
	static ADrawText* SpawnDrawTextByClass(UObject* WorldContextObject,const TSubclassOf<ADrawText> &InDrawTextClass,const FVector& InLocation, const FString& InText, const FLinearColor& Color, float Percentage, EInfoAnimType Type = EInfoAnimType::TYPE_DEFAULT, int32 PrefixStyleID = -1, bool bBackground = false);

	// Solve the bug that the user-defined format cannot be passed in the higher version of UE.
public:
	/**
	* Solve the bug that the user-defined format cannot be passed in the higher version of UE.
	*/
	static FString DrawTextPrint(const TCHAR* Fmt, ...)
	{
		TCHAR Buff[1024] = { 0 };
		int32	Result = -1;
		GET_VARARGS_RESULT_WIDE(Buff, MAX_SPRINTF, MAX_SPRINTF - 1, Fmt, Fmt, Result);
		
		return Buff;
	}

	/**
	* Solve the bug that the user-defined format cannot be passed in the higher version of UE.
	*/
	static FString DrawTextPrint(const ANSICHAR* Fmt, ...)
	{
		ANSICHAR Buff[1024] = { 0 };
		int32	Result = -1;
		GET_VARARGS_RESULT_ANSI(Buff, MAX_SPRINTF, MAX_SPRINTF - 1, Fmt, Fmt, Result);
		
		return UTF8_TO_TCHAR(Buff);
	}

	/**
	* Solve the bug that the user-defined format cannot be passed in the higher version of UE.
	*/
	static int32 DrawTextPrint(TCHAR *InBuff,int32 InBuffSize, const TCHAR* Fmt, ...)
	{
		int32	Result = -1;
		GET_VARARGS_RESULT_WIDE(InBuff, InBuffSize, InBuffSize - 1, Fmt, Fmt, Result);
		return Result;
	}

	/**MAX_SPRINTF
	* Solve the bug that the user-defined format cannot be passed in the higher version of UE.
	*/
	static int32 DrawTextPrint(ANSICHAR *InBuff,int32 InBuffSize, const ANSICHAR* Fmt, ...)
	{
		int32	Result = -1;
		GET_VARARGS_RESULT_ANSI(InBuff, InBuffSize, InBuffSize - 1, Fmt, Fmt, Result);

		return Result;
	}
};
