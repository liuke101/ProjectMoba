// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/SlotAttribute.h"

FSlotAttributeValue::FSlotAttributeValue()
	: GainType(ESlotAttributeGainType::ESAGT_Add)
	, ValueType(ESlotAttributeValueType::ESAVT_Value)
	, Value(0)
{
}

FSlotAttribute::FSlotAttribute()
	: AttributeType(ESlotAttributeType::ESAT_Burst)
	, Level(1)
	, Time(1)
{
}
