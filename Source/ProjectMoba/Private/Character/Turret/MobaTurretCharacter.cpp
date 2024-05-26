#include "Character/Turret/MobaTurretCharacter.h"

#include "Net/UnrealNetwork.h"


AMobaTurretCharacter::AMobaTurretCharacter()
{
}

void AMobaTurretCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMobaTurretCharacter, TurretRotator);
}

