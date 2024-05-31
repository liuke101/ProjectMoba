// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Drop/UI_NativeOnDrop.h"
#include "UI_ItemBase.generated.h"

class UImage;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_ItemBase : public UUI_NativeOnDrop
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

public:
	void SetIcon(UTexture2D* InIcon2D);

	UFUNCTION(BlueprintCallable)
	virtual void OnClickedWidget() {}

	FORCEINLINE int32 GetItemID() const { return ItemID; }
	FORCEINLINE void SetItemID(int32 InID) { ItemID = InID; }

private:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	UButton* ItemButton;
	
	int32 ItemID;
};
