// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Curves/CurveFloat.h"
#include "Components/TimelineComponent.h"
#include "SimpleDrawTextType.h"
#include "DrawText.generated.h"

UCLASS()
class SIMPLEDRAWTEXT_API ADrawText : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "TimeLine")
	FRuntimeFloatCurve CurveFloatClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* Widget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseAttrubute", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootWidget;

	UPROPERTY(EditDefaultsOnly, Category = "DrawText")
	float Height;

	UPROPERTY(EditDefaultsOnly, Category = "DrawText")
	float RandRange;

public:	
	// Sets default values for this actor's properties
	ADrawText();

	UFUNCTION(BlueprintCallable, Category = DrawText)
	void SetHeight(float value);

	UFUNCTION(BlueprintCallable, Category = DrawText)
	void SetRandRange(float value);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void TimeLineRangeTime(float value);

	UFUNCTION()
	void TimeLineFinished();

	void SetTextBlock(const FString &InText,const FLinearColor &Color, float Percentage, EInfoAnimType Type = EInfoAnimType::TYPE_DEFAULT, int32 PrefixStyleID = INDEX_NONE, bool bBackground = false);
protected:
	FTimeline TimeLine;
};
