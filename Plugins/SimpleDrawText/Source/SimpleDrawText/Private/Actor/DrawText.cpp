// Copyright (C) RenZhai.2023.All Rights Reserved.


#include "Actor/DrawText.h"
#include "Components/WidgetComponent.h"
#include "Components/SceneComponent.h"
#include "UI/UI_InformationWidget.h"

// Sets default values
ADrawText::ADrawText()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootWidget = CreateDefaultSubobject<USceneComponent>(TEXT("RootWidget"));
	RootComponent = RootWidget;

	Height = 10.f;
	RandRange = 50.f;

	Widget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	Widget->SetupAttachment(RootComponent);
	Widget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetReplicates(false);
}

// Called when the game starts or when spawned
void ADrawText::BeginPlay()
{
	Super::BeginPlay();
	
	//????????
	FVector Location = GetActorLocation();
	Location = FVector(
		FMath::RandRange(Location.X - RandRange, Location.X + RandRange),
		FMath::RandRange(Location.Y - RandRange, Location.Y + RandRange),
		Location.Z);

	SetActorLocation(Location);

	if (CurveFloatClass.ExternalCurve)
	{
		FOnTimelineFloat TimeLineDelegate;
		FOnTimelineEvent FinishedEvent;
		TimeLineDelegate.BindUFunction(this, FName("TimeLineRangeTime"));
		FinishedEvent.BindUFunction(this, FName("TimeLineFinished"));
		TimeLine.AddInterpFloat(CurveFloatClass.ExternalCurve, TimeLineDelegate);
		TimeLine.SetLooping(false);
		TimeLine.PlayFromStart();
		TimeLine.SetTimelineFinishedFunc(FinishedEvent);
	}
}

// Called every frame
void ADrawText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurveFloatClass.ExternalCurve)
	{
		TimeLine.TickTimeline(DeltaTime);
	}
}

void ADrawText::TimeLineRangeTime(float value)
{
	if (Height > 0)
	{
		FVector NewHeight = FMath::Lerp(FVector::ZeroVector, FVector(0, 0, Height), value);
		Widget->AddRelativeLocation(NewHeight);
	}
}

void ADrawText::TimeLineFinished()
{
	Destroy(true);
}

void ADrawText::SetTextBlock(
	const FString &InText,
	const FLinearColor &Color, 
	float Percentage, 
	EInfoAnimType Type, int32 PrefixStyleID, 
	bool bBackground)
{
	if (UUI_InformationWidget *InfoWidget = Cast<UUI_InformationWidget>(Widget->GetUserWidgetObject()))
	{
		InfoWidget->SetTextBlockStyle(InText, Color, Percentage, Type, PrefixStyleID, bBackground);
	}
}

void ADrawText::SetHeight(float value)
{
	Height = value;
}

void ADrawText::SetRandRange(float value)
{
	RandRange = value;
}
