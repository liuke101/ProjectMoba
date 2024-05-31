// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Drop/UI_NativeOnDrop.h"
#include "UI_Shop.generated.h"

class UUI_Item;
class UButton;
class UUniformGridPanel;
class UCheckBox;
/**
 * 
 */
UCLASS()
class PROJECTMOBA_API UUI_Shop : public UUI_NativeOnDrop
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** 顶部按钮点击隐藏商城 */
	UFUNCTION(BlueprintCallable)
	void OnClickedWidget();

	/** F11商城显隐 */
	UFUNCTION(BlueprintCallable)
	void OnNativeKey();
protected:
	UFUNCTION()
	void CheckBoxAll(bool bIsChecked);

	UFUNCTION()
	void CheckBoxConsumables(bool bIsChecked);

	UFUNCTION()
	void CheckBoxAuxiliaryProps(bool bIsChecked);

	UFUNCTION()
	void CheckBoxPhysicalAttack(bool bIsChecked);

	UFUNCTION()
	void CheckBoxMagicAttack(bool bIsChecked);

	UFUNCTION()
	void CheckBoxPhysicalPenetration(bool bIsChecked);

	UFUNCTION()
	void CheckBoxMagicPenetration(bool bIsChecked);

	UFUNCTION()
	void CheckBoxCrit(bool bIsChecked);

	UFUNCTION()
	void CheckBoxArmor(bool bIsChecked);

	UFUNCTION()
	void CheckBoxMagicResistance(bool bIsChecked);

	UFUNCTION()
	void CheckBoxAttackSpeed(bool bIsChecked);

	UFUNCTION()
	void CheckBoxShoes(bool bIsChecked);

protected:
	void SetCheckBoxArray(ECheckBoxState CheckBoxState);

protected:
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(meta = (BindWidget))
	UCheckBox* All;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* Consumables; //消耗品

	UPROPERTY(meta = (BindWidget))
	UCheckBox* AuxiliaryProps; //辅助道具

	UPROPERTY(meta = (BindWidget))
	UCheckBox* PhysicalAttack; //物理攻击

	UPROPERTY(meta = (BindWidget))
	UCheckBox* MagicAttack; //魔法攻击

	UPROPERTY(meta = (BindWidget))
	UCheckBox* PhysicalPenetration; //物理穿透

	UPROPERTY(meta = (BindWidget))
	UCheckBox* MagicPenetration; //魔法穿透

	UPROPERTY(meta = (BindWidget))
	UCheckBox* Crit; //暴击

	UPROPERTY(meta = (BindWidget))
	UCheckBox* Armor;  //护甲

	UPROPERTY(meta = (BindWidget))
	UCheckBox* MagicResistance;  //魔抗

	UPROPERTY(meta = (BindWidget))
	UCheckBox* AttackSpeed; //攻速

	UPROPERTY(meta = (BindWidget))
	UCheckBox* Shoes; //鞋子

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* ItemGrid; //物品栏

	UPROPERTY(meta = (BindWidget))
	UButton* TopButton; //顶部按钮

	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUI_Item> ItemClass;
	
private:
	TArray<UCheckBox*> CheckBoxArray;
	FVector2D Offset;
};
