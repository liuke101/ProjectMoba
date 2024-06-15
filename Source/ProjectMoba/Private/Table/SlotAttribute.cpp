// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/SlotAttribute.h"

FSlotAttributeValue::FSlotAttributeValue()
	: GainType(ESlotAttributeGainType::ESAGT_Add)
	, ValueType(ESlotAttributeValueType::ESAVT_Value)
	, Value(0)
{
}

FSlotAttribute::FSlotAttribute()
	: AttributeType(ESlotAttributeType::ESAT_Permanent)
	, Level(0)
	, Time(1)
	, BuffDataID(INDEX_NONE)
	, AddLevelDataID(INDEX_NONE)
	, AddLevelAttribute(nullptr)
{
}

void FSlotAttribute::UpdateLevel()
{
	if(AddLevelAttribute)
	{
		Level++;
	}
}
