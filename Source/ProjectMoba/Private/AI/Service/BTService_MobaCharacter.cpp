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

void UBTService_MobaCharacter::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	if(Blackboard_Target.SelectedKeyType == UBlackboardKeyType_Object::StaticClass() &&
		Blackboard_Distance.SelectedKeyType == UBlackboardKeyType_Float::StaticClass() &&
		Blackboard_Location.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		if(AMobaAIController* OwnerAIController = Cast<AMobaAIController>(OwnerComp.GetOwner()))
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
					Distance = FVector::Distance(OwnerAIController->GetPawn()->GetActorLocation(), Target->GetActorLocation());

					/** 设置目标位置 */
					FVector Location = Target->GetActorLocation();
					Blackboard->SetValueAsVector(Blackboard_Location.SelectedKeyName, FVector(Location.X, Location.Y, FMath::Min(Location.Z, 100))); //限制一下X轴
				}
				
				/** 设置目标距离 */
				Blackboard->SetValueAsFloat(Blackboard_Distance.SelectedKeyName, Distance);
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
