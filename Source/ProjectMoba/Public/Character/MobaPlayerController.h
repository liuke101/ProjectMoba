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
class PROJECTMOBA_API AMobaPlayerController : public APlayerController
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
	TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;
	
	/** Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeftClick_Action = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RightClick_Action = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> W_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> E_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> R_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> F_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Space_Action;

protected:
	/** 如果Character应该移动到鼠标光标. */
	bool bMoveToMouseCursor = false;

	/** 在PlayerTick中由本地客户端调用，导航到鼠标光标位置 */
	void MoveToMouseCursor();

	/** Input Action 回调函数 */
	void OnRightClickPressed();
	void OnRightClickReleased();

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
	TObjectPtr<UNiagaraSystem> FXCursor = nullptr;
	
};


