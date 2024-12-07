// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InventoryWidgetBase.h"
#include "ItemDropOverlay.generated.h"

class UInventoryPanel;
class UEquipPanel;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UItemDropOverlay : public UInventoryWidgetBase
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> InventoryWidget;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> EquipSystem;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> QuickSlotWidgets;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
protected:
	virtual void OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent) override;

	UInventoryComponent* InventoryComponent;
};
