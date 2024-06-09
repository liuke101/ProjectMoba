#include "Item/DamageBox.h"

#include "AI/MobaAIController.h"
#include "Character/MobaCharacter.h"
#include "Common/CalculationUnit.h"
#include "Common/MethodUnit.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


ADamageBox::ADamageBox()
{
	PrimaryActorTick.bCanEverTick = true;

	bNetLoadOnClient = true; //不在客户端加载
	bReplicates = true; //关闭复制
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = Root;
	
	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	DamageBox->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	//设置碰撞属性
	DamageBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

void ADamageBox::BeginPlay()
{
	Super::BeginPlay();
	
	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &ADamageBox::BeginOverlap);
}

void ADamageBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADamageBox::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(AMobaCharacter* InstigatorCharacter = Cast<AMobaCharacter>(GetInstigator()))
	{
		if(AMobaCharacter* TargetCharacter = Cast<AMobaCharacter>(OtherActor))
		{
			//服务器端
			if(GetWorld()->IsNetMode(NM_DedicatedServer))
			{
				//排除死亡角色
				if(TargetCharacter->IsDead()) return;
				//排除自己和友军
				if(InstigatorCharacter == TargetCharacter && MethodUnit::IsFriendly(InstigatorCharacter, TargetCharacter)) return;
				
				//单体目标检测
				if(bSingleCheck)
				{
					if(AMobaAIController* AIController = Cast<AMobaAIController>(InstigatorCharacter->GetController()))
					{
						if(AIController->GetTarget() != TargetCharacter) 
						{
							return;
						}
					}
				}
				
				//造成伤害
				UGameplayStatics::ApplyDamage(
					TargetCharacter,
					CalculationUnit::GetTotalDamage(TargetCharacter, InstigatorCharacter),
					InstigatorCharacter->GetController(),
					InstigatorCharacter,
					UDamageType::StaticClass());

				//销毁
				Destroy();
			}
		}
	}
}

