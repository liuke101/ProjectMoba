// Copyright (C) RenZhai.2020.All Rights Reserved.

#include "SimpleFileHelperBPLibrary.h"
#include "SimpleFileHelper.h"
#include "Misc/FileHelper.h"

USimpleFileHelperBPLibrary::USimpleFileHelperBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{}

FFileHelper::EHashOptions SimpleFileHelper::ToHashOptions(ESimpleHashOptions HashOptions)
{
	switch (HashOptions)
	{
	case ESimpleHashOptions::EnableVerify:
		/** Enable the async task for verifying the hash for the file being loaded */
		return FFileHelper::EHashOptions::EnableVerify;
	case ESimpleHashOptions::ErrorMissingHash:
		/** A missing hash entry should trigger an error */
		return FFileHelper::EHashOptions::ErrorMissingHash;
	default:
		break;;
	}

	return FFileHelper::EHashOptions::None;
}

bool USimpleFileHelperBPLibrary::GetRecentlyModifiedFile(FString& OutPath, const FString& FilePath, bool bRecurse, const FString& InSuffix)
{
	OutPath.Empty();

	//分割后缀
	TArray<FString> Suffixs;
	InSuffix.ParseIntoArray(Suffixs,TEXT(","));

	//判定是否存在后缀的内容
	auto IsSuffix =[&](const FString &InString)->bool
	{
		if (Suffixs.Num() == 0)
		{
			return true;
		}

		for (auto &Tmp : Suffixs)
		{
			if (InString.Contains(Tmp))
			{
				return true;
			}
		}

		return false;
	};

	TArray<FString> Filenames;

	if (bRecurse)
	{
		IFileManager::Get().FindFilesRecursive(Filenames, *FilePath, TEXT("*.*"), true, false);
	}
	else
	{
		IFileManager::Get().FindFiles(Filenames, *FilePath, true, false);
	}

	FDateTime DateTime(0);

	//找到最近的那个文件
	for (auto& Tmp : Filenames)
	{
		if (IsSuffix(Tmp))
		{
			FDateTime TmpDateTime = IFileManager::Get().GetTimeStamp(*Tmp);
			if (TmpDateTime > DateTime)
			{
				DateTime = TmpDateTime;
				OutPath = Tmp;
			}
		}
	}

	return Filenames.Num() > 0 && !OutPath.IsEmpty();
}

bool USimpleFileHelperBPLibrary::LoadFileToArray(TArray<uint8>& Result, const FString &Filename, int32 Flags /*= 0*/)
{
	return FFileHelper::LoadFileToArray(Result, *Filename, Flags);
}

bool USimpleFileHelperBPLibrary::LoadFileToString(FString& Result, const FString &Filename, ESimpleHashOptions VerifyFlags /*= ESimpleHashOptions::None*/)
{
	return FFileHelper::LoadFileToString(Result, *Filename, SimpleFileHelper::ToHashOptions(VerifyFlags));
}

bool USimpleFileHelperBPLibrary::LoadFileToStringArray(TArray<FString>& Result, const FString &Filename, ESimpleHashOptions VerifyFlags /*= ESimpleHashOptions::None*/)
{
	//UE::String::ParseLines();
	return FFileHelper::LoadFileToStringArray(Result, *Filename, SimpleFileHelper::ToHashOptions(VerifyFlags));
}

bool USimpleFileHelperBPLibrary::SaveArrayToFile(const TArray<uint8> &Array, const FString &Filename, int32 WriteFlags /*= 0*/)
{
	return FFileHelper::SaveArrayToFile(Array, *Filename, &IFileManager::Get(), WriteFlags);
}

bool USimpleFileHelperBPLibrary::SaveStringToFile(const FString& String, const FString &Filename, ESimpleEncodingOptions EncodingOptions /*= ESimpleEncodingOptions::AutoDetect*/, int32 WriteFlags /*= 0*/)
{
	return FFileHelper::SaveStringToFile(String, *Filename, (FFileHelper::EEncodingOptions)EncodingOptions, &IFileManager::Get(), WriteFlags);
}

bool USimpleFileHelperBPLibrary::SaveStringArrayToFile(const TArray<FString>& Lines, const FString &Filename, ESimpleEncodingOptions EncodingOptions /*= ESimpleEncodingOptions::AutoDetect*/, int32 WriteFlags /*= 0*/)
{
	return FFileHelper::SaveStringArrayToFile(Lines, *Filename, (FFileHelper::EEncodingOptions)EncodingOptions, &IFileManager::Get(), WriteFlags);
}

bool USimpleFileHelperBPLibrary::GenerateNextBitmapFilename(const FString& Pattern, const FString& Extension, FString& OutFilename)
{
	return FFileHelper::GenerateNextBitmapFilename(*Pattern, Extension, OutFilename);
}

bool USimpleFileHelperBPLibrary::CreateBitmap(const FString &Pattern, int32 DataWidth, int32 DataHeight, const FColor &Data, FVector4 SubRectangle, FString &OutFilename, bool bInWriteAlpha /*= false*/)
{
	TSharedPtr<FIntRect> Rect = nullptr;
	if (!SubRectangle.IsNearlyZero3(0))
	{
		Rect = MakeShareable(new FIntRect);
		Rect->Min.X = SubRectangle.X;
		Rect->Min.Y = SubRectangle.Y;
		Rect->Max.X = SubRectangle.Z;
		Rect->Max.Y = SubRectangle.W;
	}
	 
	return FFileHelper::CreateBitmap(
		*Pattern, 
		DataWidth, 
		DataHeight, 
		&Data, 
		Rect.Get(),
		&IFileManager::Get(),
		(OutFilename.IsEmpty() ? NULL : &OutFilename),
		bInWriteAlpha);
}

bool USimpleFileHelperBPLibrary::LoadANSITextFileToStrings(const FString &InFilename, TArray<FString>& OutStrings)
{
	return FFileHelper::LoadANSITextFileToStrings(*InFilename, &IFileManager::Get(), OutStrings);
}

bool USimpleFileHelperBPLibrary::IsFilenameValidForSaving(const FString& Filename, FText& OutError)
{
	return FFileHelper::IsFilenameValidForSaving(Filename, OutError);
}
