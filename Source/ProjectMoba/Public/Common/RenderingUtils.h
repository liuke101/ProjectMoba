// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "CoreMinimal.h"

class ASceneCapture2D;

namespace RenderingUtils
{
	struct FScreenShot
	{
		FScreenShot(
			int32 InWidth,
			int32 InHeight,
			UTexture2D*& InTexture,
			UObject* InOuter,
			int32 InImageQuality = 80,
			bool bInShowUI = false,
			bool bAddFilenameSuffix = true);

		FString& GetFilename() { return Filename; }
	protected:
		void OnScreenshotCapturedInternal(int32 SrcWidth, int32 SrcHeight, const TArray<FColor>& OrigBitmap);
	private:
		UTexture2D*& Texture;
		FDelegateHandle ScreenShotDelegateHandle;
		int32 ScaledWidth;
		int32 ScaledHeight;
		int32 ImageQuality;
		UObject* Outer;
		FString Filename;
	};
	
	/** 捕获实时MiniMap */
	ASceneCapture2D* SpawnSceneCapture2D(UWorld* World, UClass* SceneCaptureClass, FVector2D& MapSize, float Life = 0.f);
}
