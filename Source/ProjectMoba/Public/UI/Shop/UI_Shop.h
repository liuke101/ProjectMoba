// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectMoba/MobaType.h"
#include "UI/Drop/UI_NativeOnDrop.h"
#include "UI_Shop.generated.h"

class UUI_ItemSale;
class UUI_ItemSynthesis;
class UUI_Item;
class UButton;
class UUniformGridPanel;
class UCheckBox;

/** 商店UI */
UCLASS()
class PROJECTMOBA_API UUI_Shop : public UUI_NativeOnDrop
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	//Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	/** 更新指定类型的物品，实现在商城中点击CheckBox显示特定类型物品 */
	void UpdateItem(ESlotType SlotType);
	
	void SetCheckBoxArray(ECheckBoxState CheckBoxState);
	
#pragma region UI回调
protected:
	/** 顶部按钮点击隐藏商城 */
	UFUNCTION(BlueprintCallable)
	void OnClickedWidget();

	/** F11商城显隐 */
	UFUNCTION(BlueprintCallable)
	void Show();

	/** Item合成 */
	UFUNCTION(BlueprintCallable)
	void OnCallUpdateItemSynthesis(int32 SlotID);

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

	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
#pragma endregion

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> All;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> Consumables; //消耗品

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> AuxiliaryProps; //辅助道具

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> PhysicalAttack; //物理攻击

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> MagicAttack; //魔法攻击

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> PhysicalPenetration; //物理穿透

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> MagicPenetration; //魔法穿透

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> Crit; //暴击

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> Armor;  //护甲

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> MagicResistance;  //魔抗

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> AttackSpeed; //攻速

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> Shoes; //鞋子

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> ItemGrid; //物品栏

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> TopButton; //顶部按钮

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_ItemSynthesis> ItemSynthesis; //装备合成界面

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_ItemSale> ItemSale; //装备出售区域

	UPROPERTY(EditDefaultsOnly, Category = "Moba|UI")
	TSubclassOf<UUI_Item> ItemClass;
private:
	TArray<TObjectPtr<UCheckBox>> CheckBoxArray;
	FVector2D Offset;
};
