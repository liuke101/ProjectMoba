// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Drop/UI_NativeOnDrop.h"
#include "UI_ItemBase.generated.h"

struct FSlotAsset;
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
	virtual void OnClickedWidget();

	virtual void UpdateSlot(const FSlotAsset* SlotAsset);
	
	FORCEINLINE int32 GetItemDataID() const { return ItemDataID; }
	FORCEINLINE void SetItemDataID(int32 DataID) { ItemDataID = DataID; }

	FORCEINLINE UImage* GetItemIcon() const { return ItemIcon; }
	FORCEINLINE UButton* GetItemButton() const { return ItemButton; }
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ItemButton;
	
	int32 ItemDataID;
};
