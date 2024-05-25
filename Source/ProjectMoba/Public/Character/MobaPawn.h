﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ProjectMoba/MobaType.h"
#include "MobaPawn.generated.h"

class AMobaHero;
class UNiagaraSystem;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class AMobaCharacter;
class UPlayerDataComponent;

UCLASS()
class PROJECTMOBA_API AMobaPawn : public APawn
{
	GENERATED_BODY()

public:
	AMobaPawn();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PossessedBy(AController* NewController) override;
	
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** 相机与角色解耦 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moba|Class")
	TSubclassOf<ACharacter> DefaultCharacterClass;
	
	UPROPERTY() // 防止被GC回收
	TObjectPtr<AMobaHero> MobaCharacter;
	
	UFUNCTION(Server, Reliable)
	void CharacterMoveToOnServer(const FVector& Destination);

	UFUNCTION(Server, Reliable, WithValidation)
	void CharacterMoveToTargetWithAttackOnServer(const FVector& Destination, const APawn* TargetPawn);

	void SkillAttack(ESkillKey SkillKey, TWeakObjectPtr<AMobaHero> InTarget);
	
	FORCEINLINE  UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE  USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Moba|Class")
	int64 GetPlayerID();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Moba|Class")
	FORCEINLINE int32 GetCharacterID() const { return CharacterID; }
	
	UPlayerDataComponent* GetPlayerDataComponent() const;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> RootBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	int32 CharacterID;
};
