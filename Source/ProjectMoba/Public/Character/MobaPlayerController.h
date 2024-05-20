// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SubclassOf.h"
#include "GameFramework/PlayerController.h"
#include "MobaPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UNiagaraSystem;

UCLASS()
class AMobaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMobaPlayerController();

    virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override; // To add mapping context
	virtual void SetupInputComponent() override;

public:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext = nullptr;
	
	/** Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* W_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* E_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* R_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* F_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* Space_Action;

protected:
	/** 如果Character应该移动到鼠标光标. */
	bool bMoveToMouseCursor = false;

	/** 在PlayerTick中由本地客户端调用，导航到鼠标光标位置 */
	void MoveToMouseCursor();

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	UFUNCTION(Server, Reliable)
	void OnWPressed();
	void OnWReleased();

	UFUNCTION(Server, Reliable)
	void OnEPressed();
	void OnEReleased();

	UFUNCTION(Server, Reliable)
	void OnRPressed();
	void OnRReleased();

	UFUNCTION(Server, Reliable)
	void OnFPressed();
	void OnFReleased();
	
	UFUNCTION(Server, Reliable)
	void OnSpacePressed();
	void OnSpaceReleased();
	

	UFUNCTION(Server, Reliable, WithValidation)
	void VerifyMouseWorldPositionClickOnServer(const FVector& WorldOrigin, const FVector& WorldDirection);

	void SpawnNavigateClickFX();
private:
	/** 点击粒子特效 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|FX", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* FXCursor = nullptr;
	
};

