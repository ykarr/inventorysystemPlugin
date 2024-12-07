// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Slots/QuickSlot.h"
/*UI Component*/
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
//components
#include "Components/InventoryComponent.h"

#include "Widgets/Slots/Tooltip.h"
#include "Widgets/Slots/DragItemVisual.h"
#include "Widgets/ItemDragDropOperation.h"

#include "ItemBase.h"


void UQuickSlot::OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent)
{
	Super::OnOwningPlayerUIComponentInitialized(OwningUIComponent);
	InventoryComponentRef = OwningUIComponent;
	SetQuantityVisible(false);
	SetToolTip(nullptr);
}

FReply UQuickSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (!ItemID.IsNone()) {
		if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {//Is Left Mouse Button Down.
			UE_LOG(LogTemp, Display, TEXT("Click"));
			return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton); //Dragging while holding down the left mouse button
		}
	}
	return Reply.Unhandled(); //이벤트 핸들링이 완료되지 않았음
}

FReply UQuickSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	return Reply.Unhandled(); //이벤트 핸들링이 완료되지 않았음
}

void UQuickSlot::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UQuickSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	UE_LOG(LogTemp, Display, TEXT("Drag Detected"));
	if (true) {
		const TObjectPtr<UDragItemVisual> DragItemVisual = CreateWidget<UDragItemVisual>(this, DragItemVisualClass.Get());
		DragItemVisual->SetDragItemImage(ItemFillIcon);

		UItemDragDropOperation* ItemDragDropOperation = NewObject<UItemDragDropOperation>();
		//ItemDragDropOperation->SourceItem = ItemRef;
		//ItemDragDropOperation->SurceInventory = ItemRef->InventoryComponent;
		//ItemDragDropOperation->FromIndex = SlotIndex;
		ItemDragDropOperation->ItemID = ItemID;
		ItemDragDropOperation->QuickSlotName = QuickSlotName;
		ItemDragDropOperation->DefaultDragVisual = DragItemVisual;
		ItemDragDropOperation->Pivot = EDragPivot::BottomLeft;

		OutOperation = ItemDragDropOperation;
	}
}

void UQuickSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

bool UQuickSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	const UItemDragDropOperation* ItemDragDrop = Cast<UItemDragDropOperation>(InOperation);
	if (!ItemDragDrop->ItemID.IsNone()) {
		InventoryComponentRef->OnSetQuickSlotDelegate.ExecuteIfBound(ItemDragDrop->QuickSlotName, ItemDragDrop->ItemID, false);
	}
	if (ItemDragDrop) {

		InventoryComponentRef->OnSetQuickSlotDelegate.ExecuteIfBound(QuickSlotName, ItemDragDrop->ItemID,true);
		return true;
	}
	return false;
}
void UQuickSlot::UpdateQuickSlot(int32 InItemNum, TSoftObjectPtr<UTexture2D> InItemData)
{
	if (InItemNum>=1 && InItemData.IsValid()) {
		
			ItemFillIcon = InItemData;
			ItemBorder->SetBrushFromTexture(BorderFillIcon);
			ItemImage->SetBrushFromSoftTexture(ItemFillIcon);
			if (InItemNum > 1) {
				QuantityText->SetText(FText::AsNumber(InItemNum, &FNumberFormattingOptions().SetUseGrouping(true)));
				SetQuantityVisible(true);
			}
			else {
				SetQuantityVisible(false);
			}
		
	}
	else {
		//UE_LOG(LogTemp, Display, TEXT("kkkkkkkkkkkkkkkkk"));
		//ItemData = nullptr;
		//SlotButton->SetIsEnabled(false);
		ItemID = FName();
		ItemBorder->SetBrushFromTexture(BorderEmptyIcon);
		ItemImage->SetBrushFromSoftTexture(ItemEmptyIcon);
		SetQuantityVisible(false);
	}
}
void UQuickSlot::SetQuantityVisible(bool bActive) const
{
	if (bActive) {
		QuantityText->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
	else {
		QuantityText->SetVisibility(ESlateVisibility::Collapsed);
	}
}