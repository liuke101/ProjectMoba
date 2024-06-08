#include "Character/Turret/MobaTurretCharacter.h"

#include "Net/UnrealNetwork.h"


AMobaTurretCharacter::AMobaTurretCharacter()
{
}

void AMobaTurretCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MobaAssistSystem.Tick(DeltaSeconds);
}

void AMobaTurretCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMobaTurretCharacter, TurretRotator);
}

TArray<FAssistPlayer> AMobaTurretCharacter::GetAssistPlayers() const
{
	return MobaAssistSystem.GetAssistPlayers();
}

void AMobaTurretCharacter::AddAssistPlayer(const int64& InPlayerID)
{
	MobaAssistSystem.AddAssistPlayer(InPlayerID);
}

const FAssistPlayer* AMobaTurretCharacter::GetLastAssistPlayer()
{
	return MobaAssistSystem.GetLastAssistPlayer();
}

