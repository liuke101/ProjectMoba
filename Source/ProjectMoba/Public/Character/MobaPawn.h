// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ProjectMoba/MobaType.h"
#include "MobaPawn.generated.h"

class UNiagaraSystem;
class UBoxComponent;
class USpringArmComponent;
class UCameraComponent;
class AMobaCharacter;

UCLASS()
class PROJECTMOBA_API AMobaPawn : public APawn
{
	GENERATED_BODY()

public:
	AMobaPawn();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** 相机与角色解耦 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Moba|Class")
	TSubclassOf<ACharacter> DefaultCharacterClass;
	
	FORCEINLINE  UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE  USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	UFUNCTION(Server, Reliable)
	void CharacterMoveToOnServer(const FVector& Destination);

	UFUNCTION(Server, Reliable, WithValidation)
	void CharacterMoveToTargetWithAttackOnServer(const FVector& Destination, const APawn* TargetPawn);

	void SkillAttack(ESkillKey SkillKey, TWeakObjectPtr<AMobaCharacter> InTarget) const;
protected:
	UPROPERTY() // 防止被GC回收
	AMobaCharacter* MobaCharacter;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RootBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;
	
	
};
