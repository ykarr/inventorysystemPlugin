// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Slots/InventorySlot.h"
/*UI Component*/
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
/*ASyncLoad*/
#include "Engine/AssetManager.h"
//components
#include "Components/InventoryComponent.h"

#include "Widgets/Slots/Tooltip.h"
#include "Widgets/Slots/DragItemVisual.h"
#include "Widgets/ItemDragDropOperation.h"

#include "ItemBase.h"
void UInventorySlot::OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent)
{
	Super::OnOwningPlayerUIComponentInitialized(OwningUIComponent);
	InventoryComponentRef = OwningUIComponent;
	SetQuantityVisible(false);
	//if (!InventoryToolTipClass.IsNull()) {
		//UE_LOG(LogTemp, Display, TEXT("owing1111"));
		//UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(InventoryToolTipClass.ToSoftObjectPath(), [this]() {
			//if (InventoryToolTipClass.Get()) {
				//UE_LOG(LogTemp, Display, TEXT("owing"));
				//InventoryToolTip = CreateWidget<UTooltip>(GetOwningPlayer(), InventoryToolTipClass.Get());
				//SetToolTip(InventoryToolTip);
			//}
		//});
	//}
	
}

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	ItemBorder->SetBrushFromTexture(Pressed);
	//ASyncLoadImageBoard(Pressed,ItemBorder);
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (ItemRef != nullptr) {
		if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {//Is Left Mouse Button Down.
			UE_LOG(LogTemp, Display, TEXT("Click"));
			return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton); //Dragging while holding down the left mouse button
		}
	}
	return Reply.Unhandled(); //이벤트 핸들링이 완료되지 않았음
}

FReply UInventorySlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	if (ItemRef != nullptr) {
		if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton) {
			UE_LOG(LogTemp, Display, TEXT("Click"));

			if (InventoryComponentRef) {
				InventoryComponentRef->OnInventoryUseItem.Execute(SlotIndex);
			}

			return Reply.Handled();
		}
	}
	return Reply.Unhandled(); //이벤트 핸들링이 완료되지 않았음
}

void UInventorySlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (ItemRef&& InventoryToolTip) {
		InventoryToolTip->SettingToolTip(ItemRef);
		SetToolTip(InventoryToolTip);
	}
	else {
		SetToolTip(nullptr);
	}
	//SetToolTip(InventoryToolTip);
	ItemBorder->SetBrushFromTexture(Hover);
	//ASyncLoadImageBoard(Hover, ItemBorder);
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Display, TEXT("Drag Detected"));
	if (ItemRef) {
		const TObjectPtr<UDragItemVisual> DragItemVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass.Get());
		DragItemVisual->SetDragItemImage(ItemRef->GetItemData()->ItemIcon);

		UItemDragDropOperation* ItemDragDropOperation = NewObject<UItemDragDropOperation>();
		//ItemDragDropOperation->SourceItem = ItemRef;
		//ItemDragDropOperation->SurceInventory = ItemRef->InventoryComponent;
		ItemDragDropOperation->FromIndex = SlotIndex;
		ItemDragDropOperation->ItemID = ItemRef->ItemID;
		ItemDragDropOperation->ItemReferenceIndex = SlotIndex;
		ItemDragDropOperation->DefaultDragVisual = DragItemVisual;
		ItemDragDropOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = ItemDragDropOperation;
	}
}

void UInventorySlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	SetToolTip(nullptr);
	Super::NativeOnMouseLeave(InMouseEvent);
	//SetToolTip(nullptr);
	ItemBorder->SetBrushFromTexture(Normal);
	//ASyncLoadImageBoard(Normal,ItemBorder);
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	if (ItemDragDrop) {
		InventoryComponentRef->OnSlotChange.ExecuteIfBound(ItemDragDrop->FromIndex, SlotIndex);
		return true;
	}
	return false;
}

void UInventorySlot::UpdateSlot(UItemBase* ItemBase)
{
	if (ItemBase) {
		ItemRef = ItemBase;
		ItemImage->SetBrushFromSoftTexture(ItemBase->GetItemData()->ItemIcon);
		if (ItemBase->ItemQuantity > 1) {
			QuantityText->SetText(FText::AsNumber(ItemBase->ItemQuantity, &FNumberFormattingOptions().SetUseGrouping(true)));
			SetQuantityVisible(true);
		}
		else {
			SetQuantityVisible(false);
		}
	}
	else {
		ItemRef = nullptr;
		//SlotButton->SetIsEnabled(false);
		ItemImage->SetBrushFromSoftTexture(ItemEmptyIcon);
		SetQuantityVisible(false);
	}
}

void UInventorySlot::ASyncLoadImageBoard(TSoftObjectPtr<UTexture2D> InImage, TObjectPtr<UBorder> InItemBorder)
{
	//if (InImage) {
		UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(InImage.ToSoftObjectPath(), [this, InImage,InItemBorder]() {
			if (Normal.Get()) {
				InItemBorder->SetBrushFromTexture(InImage.Get());
			}
		});
	//}
}

void UInventorySlot::SetQuantityVisible(bool bActive) const
{
	if (bActive) {
		QuantityText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else {
		QuantityText->SetVisibility(ESlateVisibility::Collapsed);
	}
}
