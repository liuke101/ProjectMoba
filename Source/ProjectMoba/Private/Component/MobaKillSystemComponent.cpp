﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MobaKillSystemComponent.h"


UMobaKillSystemComponent::UMobaKillSystemComponent()
	: FirstBlood(INDEX_NONE)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMobaKillSystemComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UMobaKillSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	TArray<FContinuousKill> Removes;
	for(auto &Tmp : ContinuousKillHistory)
	{
		Tmp.KillTime -= DeltaTime; 
		if(Tmp.KillTime <= 0.0f)
		{
			Removes.Add(Tmp);
		}
	}

	for(auto &Tmp : Removes)
	{
		ContinuousKillHistory.Remove(Tmp);
	}
}

UMobaKillSystemComponent::FAccumulatedKill::FAccumulatedKill()
	: KillNumber(0)
	, KillerPlayerID(INDEX_NONE)
	, KilledPlayerID(INDEX_NONE)
{
}

UMobaKillSystemComponent::FAccumulatedKill::FAccumulatedKill(const int64& InKillerPlayerID)
	: KillNumber(0)
	, KillerPlayerID(InKillerPlayerID)
	, KilledPlayerID(INDEX_NONE)
{
}

UMobaKillSystemComponent::FContinuousKill::FContinuousKill()
	: CurrentKillerPlayerID(INDEX_NONE)
	, CurrentKilledPlayerID(INDEX_NONE)
	, KillNumber(0)
	, KillTime(10.0f)
{
}

UMobaKillSystemComponent::FContinuousKill::FContinuousKill(const int64& KillerPlayerID, const int64& KilledPlayerID)	: CurrentKillerPlayerID(KillerPlayerID)
	, CurrentKilledPlayerID(KilledPlayerID)
	, KillNumber(0)
	, KillTime(10.0f)
{
}

void UMobaKillSystemComponent::FContinuousKill::ResetKillTime()
{
	KillTime = 10.0f;
}

void UMobaKillSystemComponent::AddKiller(const int64& InPlayerID)
{
	//防止断线重连后重复添加
	if(IsExistPlayer(InPlayerID)) return;
	
	AccumulatedKillHistory.Add(FAccumulatedKill(InPlayerID));
}

bool UMobaKillSystemComponent::IsExistPlayer(const int64& InPlayerID)
{
	for(auto& Tmp : AccumulatedKillHistory)
	{
		if(Tmp.KillerPlayerID == InPlayerID)
		{
			return true;
		}
	}

	return false;
}

void UMobaKillSystemComponent::KillPrompt(const int64& KillerPlayerID, const int64& KilledPlayerID)
{
	AddContinuousKillHistory(KillerPlayerID, KilledPlayerID);
	
	uint8 CallKillFunctionNum = 0;
	
	for(auto& CK : ContinuousKillHistory)
	{
		//如果击杀者已经在连续击杀列表中
		if(CK.CurrentKillerPlayerID == KillerPlayerID)
		{
			CK.KillNumber++;

			//一血只执行一次
			if(FirstBlood == INDEX_NONE)
			{
				FirstBlood = CK.CurrentKillerPlayerID;
				FirstBloodFuntion(CK.CurrentKillerPlayerID, KilledPlayerID);
				CallKillFunctionNum = 1;
			}

			//连杀
			if(CK.KillNumber == 2)
			{
				DoubleKillsArray.Add(CK.CurrentKillerPlayerID);
				DoubleKillFunction(CK.CurrentKillerPlayerID, KilledPlayerID);
				CallKillFunctionNum = 1;
			}
			else if(CK.KillNumber == 3)
			{
				TripleKillsArray.Add(CK.CurrentKillerPlayerID);
				TripleKillFunction(CK.CurrentKillerPlayerID, KilledPlayerID);
				CallKillFunctionNum = 1;
			}
			else if(CK.KillNumber == 4)
			{
				QuadraKillsArray.Add(CK.CurrentKillerPlayerID);
				QuadraKillFunction(CK.CurrentKillerPlayerID, KilledPlayerID);
				CallKillFunctionNum = 1;
			}
			else if(CK.KillNumber>= 5)
			{
				PentaKillsArray.Add(CK.CurrentKillerPlayerID);
				PentaKillFunction(CK.CurrentKillerPlayerID, KilledPlayerID);
				CallKillFunctionNum = 1;
			}
			
			//累计击杀
			for(auto& AK : AccumulatedKillHistory)
			{
				if(AK.KillerPlayerID == KillerPlayerID)
				{
					AK.KillNumber++;
					
					if(CallKillFunctionNum == 0)
					{
						if(AK.KillNumber == 3)
						{
							DaShaTeShaArray.Add(AK.KillerPlayerID);
							DaShaTeShaFunction(AK.KillerPlayerID, KilledPlayerID);
							CallKillFunctionNum = 1;
						}
						else if(AK.KillNumber == 4)
						{
							SuoXiangPiMiArray.Add(AK.KillerPlayerID);
							SuoXiangPiMiFunction(AK.KillerPlayerID, KilledPlayerID);
							CallKillFunctionNum = 1;
						}
						else if(AK.KillNumber == 5)
						{
							HunShenShiDanArray.Add(AK.KillerPlayerID);
							HunShenShiDanFunction(AK.KillerPlayerID, KilledPlayerID);
							CallKillFunctionNum = 1;
						}
						else if(AK.KillNumber == 6)
						{
							YongGuanSanJunArray.Add(AK.KillerPlayerID);
							YongGuanSanJunFunction(AK.KillerPlayerID, KilledPlayerID);
							CallKillFunctionNum = 1;
						}
						else if(AK.KillNumber == 7)
						{
							YiJiDangQianArray.Add(AK.KillerPlayerID);
							YiJiDangQianFunction(AK.KillerPlayerID, KilledPlayerID);
							CallKillFunctionNum = 1;
						}
						else if(AK.KillNumber == 8)
						{
							WanFuMoDiArray.Add(AK.KillerPlayerID);
							WanFuMoDiFunction(AK.KillerPlayerID, KilledPlayerID);
							CallKillFunctionNum = 1;
						}
						else if(AK.KillNumber == 9)
						{
							JuShiWuShuangArray.Add(AK.KillerPlayerID);
							JuShiWuShuangFunction(AK.KillerPlayerID, KilledPlayerID);
							CallKillFunctionNum = 1;
						}
						else if(AK.KillNumber >= 10)
						{
							TianXiaWuDiArray.Add(AK.KillerPlayerID);
							TianXiaWuDiFunction(AK.KillerPlayerID, KilledPlayerID);
							CallKillFunctionNum = 1;
						}
					}
					break;
				}
			}

			if(CallKillFunctionNum == 0)
			{
				NormalKillFunction(KillerPlayerID, KilledPlayerID);
			}
			break;
		}
	}

	//团灭
	TeamDeathFunction(KillerPlayerID, KilledPlayerID);
}

void UMobaKillSystemComponent::Death(const int64& KilledPlayerID)
{
	for(auto& Tmp : AccumulatedKillHistory)
	{
		// 清空的累计击杀数
		if(Tmp.KillerPlayerID == KilledPlayerID)
		{
			Tmp.KillNumber = 0;
			Tmp.KilledPlayerID = INDEX_NONE;
			break;
		}
	}

	for(auto& Tmp : ContinuousKillHistory)
	{
		// 清空连续击杀数
		if(Tmp.CurrentKillerPlayerID == KilledPlayerID)
		{
			Tmp.KillTime = 0.0f;
			break;
		}
	}
}

void UMobaKillSystemComponent::AddContinuousKillHistory(const int64& KillerPlayerID, const int64& KilledPlayerID)
{
	FContinuousKill ContinuousKill = FContinuousKill(KillerPlayerID, KilledPlayerID);

	int32 Index = ContinuousKillHistory.Find(ContinuousKill);
	if(Index != INDEX_NONE)
	{
		ContinuousKillHistory[Index].ResetKillTime();
	}
	else
	{
		ContinuousKillHistory.Add(ContinuousKill);
	}
}
