// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Overlay/ItemDropOverlay.h"
#include "Widgets/ItemDragDropOperation.h"
#include "Components/InventoryComponent.h"


void UItemDropOverlay::OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent)
{
	Super::OnOwningPlayerUIComponentInitialized(OwningUIComponent);
	InventoryComponent = OwningUIComponent;
	
}
bool UItemDropOverlay::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (InventoryComponent) {
		UItemDragDropOperation* ItemDragDropOperation = Cast<UItemDragDropOperation>(InOperation);
		if (!ItemDragDropOperation->QuickSlotName.IsNone()) {
			InventoryComponent->OnSetQuickSlotDelegate.ExecuteIfBound(ItemDragDropOperation->QuickSlotName, ItemDragDropOperation->ItemID, false);
			return true;
		}
		InventoryComponent->OnInventoryDropItem.ExecuteIfBound(ItemDragDropOperation->FromIndex);
		return true;
	}
	
	return false;
}