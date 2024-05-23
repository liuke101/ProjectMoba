// Copyright (C) RenZhai.2019.All Rights Reserved.

#include "Coroutines/SimpleCoroutines.h"

ICoroutinesObject::ICoroutinesObject()
	:bAwaken(false)
{

}

TArray<TSharedPtr<ICoroutinesObject>> ICoroutinesObject::Array;

FCoroutinesObject::FCoroutinesObject(float InTotalTime, const FSimpleDelegate &InSimpleDelegate)
	:ICoroutinesObject()
	,SimpleDelegate(InSimpleDelegate)
	,TotalTime(InTotalTime)
	,RuningTime(0.f)
{

}

FCoroutinesObject::FCoroutinesObject(const FSimpleOnGoingDelegate& InSimpleDelegate)
	:ICoroutinesObject()
	, SimpleOnGoingDelegate(InSimpleDelegate)
	, TotalTime(INDEX_NONE)
	, RuningTime(0.f)
{
}

FCoroutinesObject::FCoroutinesObject(float InTotalTime, const FSimpleOnGoingDelegate& InSimpleDelegate)
	:ICoroutinesObject()
	, SimpleOnGoingDelegate(InSimpleDelegate)
	, TotalTime(InTotalTime)
	, RuningTime(0.f)
{
}

FCoroutinesObject::FCoroutinesObject(const FSimpleDelegate &InSimpleDelegate)
	:ICoroutinesObject()
	, SimpleDelegate(InSimpleDelegate)
	, TotalTime(INDEX_NONE)
	, RuningTime(0.f)
{

}

void FCoroutinesObject::Update(FCoroutinesRequest &CoroutinesRequest)
{
	if (TotalTime != INDEX_NONE)
	{
		RuningTime += CoroutinesRequest.IntervalTime;

		//计算OnGoing时间
		if (SimpleOnGoingDelegate.IsBound())
		{
			if (TotalTime != 0.f)
			{
				float Ratio = RuningTime / TotalTime;
				SimpleOnGoingDelegate.ExecuteIfBound(Ratio, CoroutinesRequest.IntervalTime);
			}
		}
		
		if (RuningTime >= TotalTime)
		{
			SimpleDelegate.ExecuteIfBound();
			CoroutinesRequest.bCompleteRequest = true;
		}
	}
	else
	{
		if (bAwaken)
		{
			SimpleDelegate.ExecuteIfBound();
			CoroutinesRequest.bCompleteRequest = true;
		}
	}
}

FCoroutinesRequest::FCoroutinesRequest(float InIntervalTime)
	:bCompleteRequest(false)
	,IntervalTime(InIntervalTime)
{

}
