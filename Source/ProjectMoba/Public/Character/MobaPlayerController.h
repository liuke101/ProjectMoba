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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
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

#pragma region 技能
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> W_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> E_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> R_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> F_Action;
#pragma endregion

#pragma region 道具栏
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Space_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Q_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> One_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Two_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Three_Action;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Four_Action;
#pragma endregion

#pragma region 商店
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> P_Action;
#pragma endregion

#pragma region 队伍
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> Tab_Action;
#pragma endregion
	
protected:
	/** 如果Character应该移动到鼠标光标. */
	bool bMoveToMouseCursor = false;

	/** 在PlayerTick中由本地客户端调用，导航到鼠标光标位置 */
	void MoveToMouseCursor();

	/** Input Action 回调函数 */
	void OnLeftClickPressed();
	void OnLeftClickReleased();
	void OnRightClickPressed();
	void OnRightClickReleased();

	void MouseLockedInViewport(); //鼠标锁定在视口
	void SpawnNavigateClickFX() const;

	/** 获取鼠标点击位置的世界坐标 */
	bool GetClientWorldPosFromScreenPos(FVector& WorldOrigin, FVector& WorldDirection) const;

	/** RPC */
	UFUNCTION(Server, Reliable)
	void Server_VerifyMouseWorldPositionClick(const FVector& WorldOrigin, const FVector& WorldDirection);

	UFUNCTION(Server, Reliable)
	void Server_GetClickTargetCharacterInfo(const FVector& WorldOrigin, const FVector& WorldDirection);
	
private:
	/** 点击粒子特效 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Moba|FX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> FXCursor = nullptr;
	
};




