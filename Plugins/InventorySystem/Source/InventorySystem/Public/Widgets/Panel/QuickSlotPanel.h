// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InventoryWidgetBase.h"
#include "ItemDataTable.h"
#include "QuickSlotPanel.generated.h"

class UQuickSlot;
class UTooltip;
class UHorizontalBox;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UQuickSlotPanel : public UInventoryWidgetBase
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct()override;
	void InitQuickSlot();
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TObjectPtr<UTooltip> InventoryToolTip;
protected:
	virtual void OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent) override;

	UPROPERTY(EditDefaultsOnly, Category = "QuickSlot")
	TSoftClassPtr<UQuickSlot> QuickSlotClass;
	//TArray<UQuickSlot*> QuickSlotArrays;
	TMap<FName,UQuickSlot*> QuickSlotArrays;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UTooltip> InventoryToolTipClass;
	UInventoryComponent* InventoryComponentRef;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	UHorizontalBox* QuickSlotHorizontalBox;

	void UpdateInventoryQuickSlotToMatchName(FName Index, int32 InItemNum, TSoftObjectPtr<UTexture2D>  ItemData , FName InItemID);
};
