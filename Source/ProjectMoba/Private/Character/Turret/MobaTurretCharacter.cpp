#include "Character/Turret/MobaTurretCharacter.h"

#include "Component/MobaAssistSystemComponent.h"
#include "Net/UnrealNetwork.h"


AMobaTurretCharacter::AMobaTurretCharacter()
{
}

void AMobaTurretCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMobaTurretCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMobaTurretCharacter, TurretRotator);
}

TArray<FAssistPlayer> AMobaTurretCharacter::GetAssistPlayers() const
{
	return MobaAssistSystemComponent->GetAssistPlayers();
}

void AMobaTurretCharacter::AddAssistPlayer(const int64& InPlayerID) const
{
	MobaAssistSystemComponent->AddAssistPlayer(InPlayerID);
}

const FAssistPlayer* AMobaTurretCharacter::GetLastAssistPlayer() const
{
	return MobaAssistSystemComponent->GetLastAssistPlayer();
}

