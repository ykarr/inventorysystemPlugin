// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Panel/InventoryPanel.h"
#include "Widgets/Slots/InventorySlot.h"
#include "Components/UniformGridPanel.h"
#include "Engine\AssetManager.h"
#include "Components/InventoryComponent.h"
#include "Widgets/Slots/Tooltip.h"
#include "Components/Button.h"

void UInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();
	InventoryToolTip = CreateWidget<UTooltip>(GetOwningPlayer(), InventoryToolTipClass);
	InitInventoryGridWidget();
}
void UInventoryPanel::InitInventoryGridWidget()
{
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(InventorySlotClass.ToSoftObjectPath(), [this]() {
		if (InventorySlotClass.Get()) {
			for (int32 i = 0; i < InventoryComponentRef->GetInventorycapacity(); i++) {
				TObjectPtr<UInventorySlot> ItemSlot = CreateWidget<UInventorySlot>(GetOwningPlayer(), InventorySlotClass.Get());
				//ItemSlot->InventoryWidgetController = InventoryWidgetController;
				ItemSlot->SlotIndex = i;
				ItemSlot->InventoryToolTip = InventoryToolTip;
				//ItemSlot->CurrentSlotType = 1; //0 Equip, 1 Inven, 2 quick
				InventoryGridPanel->AddChildToUniformGrid(ItemSlot, i / 5, i % 5);
				SlotsArray.Add(ItemSlot);
			}
		}
	});
}

void UInventoryPanel::OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent)
{
	Super::OnOwningPlayerUIComponentInitialized(OwningUIComponent);
	InventoryComponentRef = OwningUIComponent;
	InventoryComponentRef->OnSlotUpdateDelegate.BindUObject(this, &ThisClass::UpdateInventorySlotToMatchIndex);
	CloseBtn->OnClicked.AddUniqueDynamic(this,&ThisClass::CloseInventoryPanel);
}

void UInventoryPanel::UpdateInventorySlotToMatchIndex(int32 Index, UItemBase* ItemBase)
{
	SlotsArray[Index]->UpdateSlot(ItemBase);
}

void UInventoryPanel::CloseInventoryPanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

bool UInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return true;
}
