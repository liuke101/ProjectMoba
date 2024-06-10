// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MobaTableBase.h"
#include "SlotAttribute.generated.h"


/** TODO:BUFF系统 6.17-6.25 */

/** 属性类型 */
UENUM()
enum class ESlotAttributeType : uint8
{
	ESAT_Burst UMETA(DisplayName = "Burst"), //瞬发
	ESAT_Continuous UMETA(DisplayName = "Continuous"), //持续
	ESAT_Permanent UMETA(DisplayName = "Permanent"), //永久
};

/** 增益类型 */
UENUM()
enum class ESlotAttributeGainType : uint8
{
	ESAGT_Add UMETA(DisplayName = "Add"), //+
	ESAGT_Substract UMETA(DisplayName = "Substract"), //-
};

/** 值类型 */
UENUM()
enum class ESlotAttributeValueType : uint8
{
	ESAVT_Value UMETA(DisplayName = "Value"), //值
	ESAVT_Percent UMETA(DisplayName = "Percent"), //百分比
};

USTRUCT()
struct FSlotAttributeValue
{
	GENERATED_BODY()
	
	FSlotAttributeValue();

	UPROPERTY(EditDefaultsOnly, Category = "Slot Attribute")
	ESlotAttributeGainType GainType;

	UPROPERTY(EditDefaultsOnly, Category = "Slot Attribut")
	ESlotAttributeValueType ValueType;

	UPROPERTY(EditDefaultsOnly, Category = "Slot Attribute")
	float Value;
};


USTRUCT(BlueprintType)
struct FSlotAttribute : public FMobaTableBase
{
	GENERATED_USTRUCT_BODY()

	FSlotAttribute();

	UPROPERTY(EditDefaultsOnly, Category = "Slot Attribute")
	ESlotAttributeType AttributeType;

	UPROPERTY(EditDefaultsOnly, Category = "Slot Attribute")
	int32 Level;

	UPROPERTY(EditDefaultsOnly, Category = "Slot Attribute")
	FSlotAttributeValue Health;

	UPROPERTY(EditDefaultsOnly, Category = "Slot Attribute")
	FSlotAttributeValue CD;
};
