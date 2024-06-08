#include "Component/PlayerDataComponent.h"

#include "Net/UnrealNetwork.h"


UPlayerDataComponent::UPlayerDataComponent()
	: PlayerName("DefaultName")
	, TeamType(ETeamType::ETT_Red)
	, Gold(900)
	, KillNum(0)
	, DeathNum(0)
	, AssistNum(0)
	, MinionKillNum(0)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	SetIsReplicatedByDefault(true);
}


void UPlayerDataComponent::BeginPlay()
{
	Super::BeginPlay();

	//暂时随机生成一个PlayerID，后面改成从DB服务器获取
	PlayerID = FMath::RandRange(0, 1000000); 

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

	DOREPLIFETIME_CONDITION(UPlayerDataComponent, Gold, COND_OwnerOnly); //仅同步给拥有者
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
	FSlotAttribute_Element Element; //拷贝Tmp而不是引用，否则Remove崩溃
	
	for(const auto& Tmp : AttributeElements)
	{
		if(Tmp.Key == InKey)
		{
			Element = Tmp;
			break;
		}
	} 
	
	if(Element.IsValid())
	{
		AttributeElements.Remove(Element);
	}
}

void UPlayerDataComponent::FSlotAttribute_Internal::SetKeyToNewKey(int32 OldKey, int32 NewKey)
{
	for(auto& Element : AttributeElements)
	{
		if(Element.Key == OldKey)
		{
			Element.Key = NewKey;
			break;
		}
	}
}

void UPlayerDataComponent::FSlotAttribute_Internal::SwapKey(int32 KeyA, int32 KeyB)
{
	FSlotAttribute_Element* ElementA = nullptr;
	FSlotAttribute_Element* ElementB = nullptr;
	
	for(auto& Element : AttributeElements)
	{
		if(Element.Key == KeyA)
		{
			ElementA = &Element;
		}
		else if(Element.Key == KeyB)
		{
			ElementB = &Element;
		}

		if(ElementA && ElementB && ElementA!=ElementB)
		{
			Swap(ElementA->Key, ElementB->Key);
			break;
		}
	}

	
}

