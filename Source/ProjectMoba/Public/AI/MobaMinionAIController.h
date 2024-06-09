#pragma once

#include "CoreMinimal.h"
#include "MobaAIController.h"
#include "MobaMinionAIController.generated.h"

struct FAITargetRef;

UCLASS()
class PROJECTMOBA_API AMobaMinionAIController : public AMobaAIController
{
	GENERATED_BODY()

public:
	AMobaMinionAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void InitMobaAIController() override;
	
	virtual AMobaCharacter* FindTarget() override;
	virtual AMobaCharacter* GetTaskTarget() override;
	
	void FindTaskTarget(const TArray<AActor*>& OutActors);

	bool CheckRange() const;
private:
	UPROPERTY()
	TObjectPtr<AMobaCharacter> TaskTarget;;
};


USTRUCT()
struct FAITargetRefs
{
	GENERATED_BODY()

	friend class AMobaMinionAIController;
public:
	FAITargetRefs(){}

	AMobaCharacter* GetTarget();
	
	void InitRef(const AMobaCharacter* TargetCharacter);

	/** 从四个数组中获取最小引用计数 */
	int32 GetMinRefCount() const;

	/** 收集具有最小引用计数的目标 */
	void GetTargetsWithMinRefCount(int32 MinRefCount, TArray<FAITargetRef>& InTargetRefs, TArray<FAITargetRef*>& OutTargetsWithMinRefCount);

	/** 获取最近的目标 */
	AMobaCharacter* GetClosestTarget(const TArray<FAITargetRef*>& MinRefCountTargets);

protected:
	bool InitTargetRef(TArray<FAITargetRef>& TargetRefs, const AMobaCharacter* TargetCharacter);

	
private:
	TArray<FAITargetRef> MinionTargets;
	TArray<FAITargetRef> MonsterTargets;
	TArray<FAITargetRef> TowerTargets;
	TArray<FAITargetRef> PlayerTargets;
};

USTRUCT()
struct FAITargetRef
{
	GENERATED_BODY()

	FAITargetRef()
		: RefCount(0)
		, Distance(0.0f)
	{}

	FAITargetRef(AMobaCharacter* InTargetCharacter, float InDistance)
		: TargetCharacter(InTargetCharacter)
		, RefCount(0)
		, Distance(InDistance)
	{}
	
	UPROPERTY()
	TObjectPtr<AMobaCharacter> TargetCharacter;
	
	int32 RefCount; //引用计数，被多少人人视为目标
	float Distance;
};
