#include "Character/Turret/MobaTurret.h"

#include "Net/UnrealNetwork.h"


AMobaTurret::AMobaTurret()
{
}

void AMobaTurret::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMobaTurret, TowersRotator);
}

