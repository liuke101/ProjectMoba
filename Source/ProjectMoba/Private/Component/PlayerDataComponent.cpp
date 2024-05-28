#include "Component/PlayerDataComponent.h"

#include "Net/UnrealNetwork.h"


UPlayerDataComponent::UPlayerDataComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	PlayerID = FMath::RandRange(0,100000); //暂时随机生成一个PlayerID，后面改成从DB服务器获取
}


void UPlayerDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// TODO:暂时通过读txt获取数据，后面改成通过socket从服务器读取
	FString NumberString;
	TArray<FString> TestPlayerData;
	FFileHelper::LoadFileToStringArray(TestPlayerData, *(FPaths::ProjectDir() / TEXT("PlayerData.txt"))); //从txt读取角色ID
	if(TestPlayerData.Num()==2)
	{
		PlayerName = FName(*TestPlayerData[0]);
		TeamType = static_cast<ETeamType>(FCString::Atoi(*TestPlayerData[1]));
	}
}

void UPlayerDataComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// DOREPLIFETIME_CONDITION(UPlayerDataComponent, PlayerName, COND_InitialOnly);
	// DOREPLIFETIME_CONDITION(UPlayerDataComponent, TeamType, COND_InitialOnly)
	// DOREPLIFETIME_CONDITION(UPlayerDataComponent, PlayerID, COND_InitialOnly);
}


void UPlayerDataComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UPlayerDataComponent::FSlotAttribute_Internal::Contains(int32 InKey)
{
	for(auto& Tmp : AttributeElements)
	{
		if(Tmp.Key == InKey)
		{
			return true;
		}
	}

	return false;
}

void UPlayerDataComponent::FSlotAttribute_Internal::Add(int32 Key, const FSlotAttribute& Value)
{
	AttributeElements.Add(FSlotAttribute_Element(Key,Value));
}

void UPlayerDataComponent::FSlotAttribute_Internal::Remove(int32 InKey)
{
	for(auto& Tmp : AttributeElements)
	{
		if(Tmp.IsValid())
		{
			if(Tmp.Key == InKey)
			{
				AttributeElements.Remove(Tmp);
				break;
			}
		}
	}
}

void UPlayerDataComponent::FSlotAttribute_Internal::SetKeyToNewKey(int32 OldKey, int32 NewKey)
{
}

void UPlayerDataComponent::FSlotAttribute_Internal::SwapKey(int32 KeyA, int32 KeyB)
{
}

