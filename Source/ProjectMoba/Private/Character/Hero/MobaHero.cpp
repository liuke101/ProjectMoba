#include "Character/Hero/MobaHero.h"

#include "Common/MethodUnit.h"
#include "Table/CharacterAsset.h"


AMobaHero::AMobaHero()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMobaHero::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMobaHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMobaHero::SkillAttack(ESkillKey SkillKey, TWeakObjectPtr<AMobaHero> InTarget) 
{
	if(UAnimMontage* Montage = GetCurrentSkillMontage(SkillKey))
	{
		//广播动画
		MultiCastPlayerAnimMontage(Montage);
	}
}

UAnimMontage* AMobaHero::GetCurrentSkillMontage(ESkillKey SkillKey) const
{
	if(const FCharacterAsset* CharacterAsset = MethodUnit::GetCharacterAssetFromPlayerID(GetWorld(), GetPlayerID()))
	{
		switch (SkillKey)
		{
		case ESkillKey::ESK_W:
			return CharacterAsset->W_SkillMontage;
		case ESkillKey::ESK_E:
			return CharacterAsset->E_SkillMontage;
		case ESkillKey::ESK_R:
			return CharacterAsset->R_SkillMontage;
		case ESkillKey::ESK_F:
			return CharacterAsset->F_SkillMontage;
		case ESkillKey::ESK_Space:
			return CharacterAsset->Space_SkillMontage;
		}
	}
	return nullptr;
}
