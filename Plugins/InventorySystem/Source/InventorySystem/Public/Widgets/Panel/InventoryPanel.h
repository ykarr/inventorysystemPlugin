// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InventoryWidgetBase.h"
#include "InventoryPanel.generated.h"

class UInventorySlot;
class UUniformGridPanel;
class UItemBase;
class UTooltip;
class UButton;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryPanel : public UInventoryWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TObjectPtr<UTooltip> InventoryToolTip;
protected:
	virtual void OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent) override;
	virtual void NativeConstruct()override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;//사용자가 위젯에 문가를 드롭했을때.
	void UpdateInventorySlotToMatchIndex(int32 Index, UItemBase* ItemBase);
	void InitInventoryGridWidget();
	
	//Inventory Slots
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TArray<TObjectPtr< UInventorySlot>> SlotsArray;
	
private:
	UFUNCTION()
	void CloseInventoryPanel();
	UInventoryComponent* InventoryComponentRef;

	UPROPERTY(EditDefaultsOnly, Category = "InventoryUI")
	TSoftClassPtr<UInventorySlot> InventorySlotClass;
	UPROPERTY(EditDefaultsOnly, Category = "InventoryUI")
	TSubclassOf<UTooltip> InventoryToolTipClass;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseBtn;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGridPanel;
};
