// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MobaPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;

UCLASS()
class AMobaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMobaPlayerController();

	virtual void Tick(float DeltaTime) override;
    virtual void PlayerTick(float DeltaTime) override;
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* SetDestinationClickAction;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	// To add mapping context
	virtual void BeginPlay();
	
	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();

	UFUNCTION(Server, Reliable, WithValidation)
	void VerifyMouseWorldPositionClickOnServer(const FVector& WorldOrigin, const FVector& WorldDirection);

	
private:
	FVector CachedDestination;
	
	float FollowTime; // For how long it has been pressed
};


