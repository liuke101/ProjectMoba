// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_MobaPlayer.h"

#include "AI/Service/BTService_MobaCharacter.h"

#include "AI/MobaAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Character/MobaCharacter.h"
#include "Table/CharacterAttribute.h"

void UBTService_MobaPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if(Blackboard_Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() &&
		Blackboard_Distance.SelectedKeyType == UBlackboardKeyType_Float::StaticClass() &&
		Blackboard_Location.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass() &&
		Blackboard_Death.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
	{
		if(AMobaAIController* OwnerAIController = OwnerComp.GetOwner<AMobaAIController>())
		{
			if(AMobaCharacter* OwnerCharacter = OwnerAIController->GetPawn<AMobaCharacter>())
			{
				if(UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
				{
					//检测死亡
					Blackboard->SetValueAsBool(Blackboard_Death.SelectedKeyName, OwnerCharacter->IsDead());
					if (OwnerCharacter->IsDead()) return;

					// 获取攻击范围
					float AttackRange = OwnerCharacter->GetCharacterAttribute()->AttackRange;

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

					if(IsTaskTarget())
					{
						Target = OwnerAIController->FindTarget();
						if(Target)
						{
							if(!IsTaskTarget())
							{
								float Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
								if(Distance > AttackRange)
								{
									Target = OwnerAIController->GetTaskTarget();
								}
							}
						}
						Blackboard->SetValueAsObject(Blackboard_Target.SelectedKeyName, Target);
					}
					else
					{
						if(!Target || Target->IsDead())//如果目标为空或者目标死亡，就重新寻找目标
						{
							Target = OwnerAIController->FindTarget();
							if(Target)
							{
								Blackboard->SetValueAsObject(Blackboard_Target.SelectedKeyName, Target);
							}
						}
					}
					
		
					float Distance =  999999.0f;

					auto CheckRangeAttack = [&]()
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

						if(IsTaskTarget())
						{
							CheckRangeAttack();
						}
						else
						{
							//如果目标角色死亡，就将目标置空
							if(Distance > 2000.0f)
							{
								OwnerAIController->SetTarget(nullptr);
								Distance = 999999.0f;
							}
							else
							{
								CheckRangeAttack();
							}
						}
					}
					
			
					/** 设置目标距离 */
					Blackboard->SetValueAsFloat(Blackboard_Distance.SelectedKeyName, Distance);
				}
			}
		}
   }
}

void UBTService_MobaPlayer::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if(UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		Blackboard_Target.ResolveSelectedKey(*BBAsset);
		Blackboard_Distance.ResolveSelectedKey(*BBAsset);
		Blackboard_Location.ResolveSelectedKey(*BBAsset);
		Blackboard_Death.ResolveSelectedKey(*BBAsset);
	}
}
