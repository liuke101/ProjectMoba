// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_MobaMinion.h"

#include "AI/Service/BTService_MobaCharacter.h"
#include "AI/MobaMinionAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Character/MobaCharacter.h"
#include "Table/CharacterAttribute.h"

void UBTService_MobaMinion::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ScheduleNextTick(OwnerComp, NodeMemory);
	
	if(Blackboard_Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() &&
		Blackboard_Distance.SelectedKeyType == UBlackboardKeyType_Float::StaticClass() &&
		Blackboard_Location.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass() &&
		Blackboard_Death.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
	{
		if(AMobaMinionAIController* OwnerAIController = OwnerComp.GetOwner<AMobaMinionAIController>())
		{
			if(AMobaCharacter* OwnerCharacter = OwnerAIController->GetPawn<AMobaCharacter>())
			{
				if(UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
				{
					// 检测死亡
					Blackboard->SetValueAsBool(Blackboard_Death.SelectedKeyName, OwnerCharacter->IsDead());
					if (OwnerCharacter->IsDead()) return;

					// 获取目标
					AMobaCharacter* Target = Cast<AMobaCharacter>(Blackboard->GetValueAsObject(Blackboard_Target.SelectedKeyName));
					
					//判断是否是任务目标
					auto IsTaskTarget = [&]()->bool
					{
						if(AMobaCharacter* TaskTarget = OwnerAIController->GetTaskTarget())
						{
							return TaskTarget == Target;
						}
						return false;	
					};

					//如果目标为空或者目标死亡，就重新寻找目标
					if(!Target || Target->IsDead())
					{
						Target = OwnerAIController->FindTarget();
					}
					else //如果目标不为空，就判断是否在检查范围内
					{
						if(OwnerAIController->CheckRange())
						{
							//如果是任务目标，就重新寻找目标
							if(IsTaskTarget())
							{
								Target = OwnerAIController->FindTarget();
							}
						}
					}

					//判断目标是否离开检测范围（跑了）
					if(Target)
					{
						if(!IsTaskTarget())
						{
							float Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
							//如果超过检测范围，就重新寻找目标
							if(Distance > 1000.0f)
							{
								Target = OwnerAIController->FindTarget();
							}
						}
					}
					
					Blackboard->SetValueAsObject(Blackboard_Target.SelectedKeyName, Target);


					// 获取攻击范围
					float AttackRange = OwnerCharacter->GetCharacterAttribute()->AttackRange;
					
					float Distance =  999999.0f;

					//lambda 定位目标
					auto LocateTarget = [&]()
					{
						//如果距离大于攻击距离，就继续向目标移动
						if(Distance > AttackRange) 
						{
							Blackboard->SetValueAsVector(Blackboard_Location.SelectedKeyName,  Target->GetActorLocation());
						}
						else//如果距离小于等于攻击范围，就停止移动，只将角色面向转向目标
						{
							FVector Dir = Target->GetActorLocation() - OwnerCharacter->GetActorLocation();
							Dir.Normalize();
							FVector FacePos = OwnerCharacter->GetActorLocation() + Dir * 20.0f; //往前略微偏移，保证进入攻击范围
							Blackboard->SetValueAsVector(Blackboard_Location.SelectedKeyName, FacePos);
						}
					};
					
					if(Target)
					{
						Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());

						LocateTarget();
					}
			
					/** 设置目标距离 */
					Blackboard->SetValueAsFloat(Blackboard_Distance.SelectedKeyName, Distance);
				}
			}
		}
   }
}

void UBTService_MobaMinion::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
}
