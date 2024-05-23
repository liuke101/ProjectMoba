// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Service/BTService_MobaCharacter.h"

#include "AI/MobaAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Character/MobaCharacter.h"
#include "Table/CharacterAttribute.h"

void UBTService_MobaCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if(Blackboard_Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() &&
		Blackboard_Distance.SelectedKeyType == UBlackboardKeyType_Float::StaticClass() &&
		Blackboard_Location.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		if(AMobaAIController* OwnerAIController = OwnerComp.GetOwner<AMobaAIController>())
		{
			if(AMobaCharacter* OwnerCharacter = OwnerAIController->GetPawn<AMobaCharacter>())
			{
				if(UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent())
				{
					AMobaCharacter* Target = Cast<AMobaCharacter>(Blackboard->GetValueAsObject(Blackboard_Target.SelectedKeyName));
					if(!Target)
					{
						Target = OwnerAIController->FindTarget();
					}
		
					float Distance =  999999.0f;
					if(Target)
					{
						//如果角色死亡，就将目标置空
						if(OwnerCharacter->IsDead())
						{
							OwnerAIController->SetTarget(nullptr);
						}
						else //如果角色没有死亡，追踪并攻击目标
						{
							Distance = FVector::Distance(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
							float AttackRange = Target->GetCharacterAttribute()->AttackRange;
							//如果距离大于攻击范围，就继续向目标移动
							if(Distance > AttackRange)
							{
								Blackboard->SetValueAsVector(Blackboard_Location.SelectedKeyName,  Target->GetActorLocation());
							}
							else //如果距离小于等于攻击范围，就停止移动，只将角色面向转向目标
							{
								FVector Dir = Target->GetActorLocation() - OwnerCharacter->GetActorLocation();
								Dir.Normalize();
								FVector FacePos = OwnerCharacter->GetActorLocation() + Dir * 20.0f;
								Blackboard->SetValueAsVector(Blackboard_Location.SelectedKeyName, FacePos);
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

void UBTService_MobaCharacter::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if(UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		Blackboard_Target.ResolveSelectedKey(*BBAsset);
		Blackboard_Distance.ResolveSelectedKey(*BBAsset);
		Blackboard_Location.ResolveSelectedKey(*BBAsset);
	}
}
