// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Tool/CharacterSpawnPoint.h"

ACharacterSpawnPoint::ACharacterSpawnPoint()
	: SpawnTime(30.0f)
{
	//不需要同步到客户端
	bNetLoadOnClient = false; 
	SetReplicates(false);
}
