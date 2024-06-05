#include "MiscData.h"

FKillSystem::FAccumulatedKill::FAccumulatedKill()
	: KillNumber(0)
	, KillerPlayerID(INDEX_NONE)
	, KilledPlayerID(INDEX_NONE)
{
}

FKillSystem::FAccumulatedKill::FAccumulatedKill(const int64& InKillerPlayerID)
	: KillNumber(0)
	, KillerPlayerID(InKillerPlayerID)
	, KilledPlayerID(INDEX_NONE)
{
}

FKillSystem::FContinuousKill::FContinuousKill(): CurrentKillerPlayerID(INDEX_NONE), CurrentKilledPlayerID(INDEX_NONE), KillNumber(0), KillTime(10.0f)
{
}

FKillSystem::FContinuousKill::FContinuousKill(const int64& KillerPlayerID, const int64& KilledPlayerID)
	: CurrentKillerPlayerID(KillerPlayerID)
	, CurrentKilledPlayerID(KilledPlayerID)
	, KillNumber(0)
	, KillTime(10.0f)
{
	ResetTime();
}

void FKillSystem::FContinuousKill::ResetTime()
{
	KillTime = 10.0f;
}

void FKillSystem::AddKiller(const int64& InPlayerID)
{
	AccumulatedKillHistory.Add(FAccumulatedKill(InPlayerID));
}

void FKillSystem::Tick(float DeltaSeconds)
{
	TArray<FContinuousKill> Removes;
	for(auto &Tmp : ContinuousKillHistory)
	{
		Tmp.KillTime -= DeltaSeconds;
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

void FKillSystem::Kill(const int64& KillerPlayerID, const int64& KilledPlayerID)
{
	bool bCallKillFunction = false;
	
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
			}

			//连杀
			switch (CK.KillNumber)
			{
				case 2:
					DoubleKillsArray.Add(CK.CurrentKillerPlayerID);
					DoubleKillFunction(CK.CurrentKillerPlayerID, KilledPlayerID);
					bCallKillFunction = true;
					break;
				case 3:
					TripleKillsArray.Add(CK.CurrentKillerPlayerID);
					TripleKillFunction(CK.CurrentKillerPlayerID, KilledPlayerID);
					bCallKillFunction = true;
					break;
				case 4:
					QuadraKillsArray.Add(CK.CurrentKillerPlayerID);
					QuadraKillFunction(CK.CurrentKillerPlayerID, KilledPlayerID);
					bCallKillFunction = true;
					break;
				case 5:
					PentaKillsArray.Add(CK.CurrentKillerPlayerID);
					PentaKillFunction(CK.CurrentKillerPlayerID, KilledPlayerID);
					bCallKillFunction = true;
					break;
				
				default:
					break;
			}

			//累计击杀
			for(auto& AK : AccumulatedKillHistory)
			{
				if(AK.KillerPlayerID == KillerPlayerID)
				{
					AK.KillNumber++;

					switch(AK.KillNumber)
					{
					case 3:
						DaShaTeShaArray.Add(AK.KillerPlayerID);
						if(!bCallKillFunction) //优先处理连杀
						{
							DaShaTeShaFunction(AK.KillerPlayerID, KilledPlayerID);
						}
						break;
					case 4:
						SuoXiangPiMiArray.Add(AK.KillerPlayerID);
						if(!bCallKillFunction)
						{
							SuoXiangPiMiFunction(AK.KillerPlayerID, KilledPlayerID);
						}
						break;
					case 5:
						HunShenShiDanArray.Add(AK.KillerPlayerID);
						if(!bCallKillFunction)
						{
							HunShenShiDanFunction(AK.KillerPlayerID, KilledPlayerID);
						}
						break;
					case 6:
						YongGuanSanJunArray.Add(AK.KillerPlayerID);
						if(!bCallKillFunction)
						{
							YongGuanSanJunFunction(AK.KillerPlayerID, KilledPlayerID);
						}
						break;
					case 7:
						YiJiDangQianArray.Add(AK.KillerPlayerID);
						if(!bCallKillFunction)
						{
							YiJiDangQianFunction(AK.KillerPlayerID, KilledPlayerID);
						}
						break;
					case 8:
						WanFuMoDiArray.Add(AK.KillerPlayerID);
						if(!bCallKillFunction)
						{
							WanFuMoDiFunction(AK.KillerPlayerID, KilledPlayerID);
						}
						break;
					case 9:
						JuShiWuShuangArray.Add(AK.KillerPlayerID);
						if(!bCallKillFunction)
						{
							JuShiWuShuangFunction(AK.KillerPlayerID, KilledPlayerID);
						}
						break;
					case 10:
						TianXiaWuDiArray.Add(AK.KillerPlayerID);
						if(!bCallKillFunction)
						{
							TianXiaWuDiFunction(AK.KillerPlayerID, KilledPlayerID);
						}
						break;
					default:
						break;
					}
				}
			}
		}
	}
	
}

void FKillSystem::Death(const int64& KilledPlayerID)
{
	for(auto& Tmp : AccumulatedKillHistory)
	{
		// 如果击杀者死亡，则清空击杀者的累计击杀数
		if(Tmp.KillerPlayerID == KilledPlayerID)
		{
			Tmp.KillNumber = 0;
			Tmp.KilledPlayerID = INDEX_NONE;
			break;
		}
	}
}
