// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MobaPawn.generated.h"

class ADrawText;
class AMobaHeroCharacter;
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
	
	FORCEINLINE AMobaHeroCharacter* GetControlledMobaHero() const { return ControlledMobaHero; }
	FORCEINLINE void SetControlledMobaHero(AMobaHeroCharacter* InControlledMobaHero) { ControlledMobaHero = InControlledMobaHero; }
	
	void SkillAttack(int32 SkillDataID) const;

#pragma region RPC

	UFUNCTION(Server, Reliable)
	void Server_CharacterMoveTo(const FVector& Destination);

	UFUNCTION(Server, Reliable)
	void Server_CharacterMoveToTargetWithAttack(const FVector& Destination, const APawn* TargetPawn);
#pragma endregion
	
#pragma region "Getter/Setter"
	FORCEINLINE  UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE  USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Moba|Class")
	int64 GetPlayerID();

	UPlayerDataComponent* GetPlayerDataComponent() const;
#pragma endregion
	

protected:
	UPROPERTY() // 防止被GC回收
	TObjectPtr<AMobaHeroCharacter> ControlledMobaHero;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> RootBox;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;
};
