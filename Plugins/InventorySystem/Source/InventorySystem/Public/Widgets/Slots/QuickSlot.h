// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InventoryWidgetBase.h"
#include "ItemDataTable.h"
#include "QuickSlot.generated.h"

class UBorder;
class UImage;
class UTextBlock;
class UTooltip;
class UTexture2D;
class UItemBase;
class UDragItemVisual;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UQuickSlot : public UInventoryWidgetBase
{
	GENERATED_BODY()
public:
	FName ItemID;
	//TSoftObjectPtr<UTexture2D> ItemDataIcon;
	void UpdateQuickSlot(int32 InItemNum, TSoftObjectPtr<UTexture2D> InItemData);
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TObjectPtr<UTooltip> InventoryToolTip;
	UPROPERTY(EditDefaultsOnly, Category = "Inven")
	UInventoryComponent* InventoryComponentRef;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	FName QuickSlotName;
protected:
	FItemData ItemData;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TObjectPtr<UTexture2D> ItemEmptyIcon;
	TSoftObjectPtr<UTexture2D> ItemFillIcon;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TObjectPtr<UTexture2D> BorderEmptyIcon;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TObjectPtr<UTexture2D> BorderFillIcon;

	void SetQuantityVisible(bool bActive) const;
	virtual void OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;//마우스 버튼 누를때.
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override; //마우스 더블클릭.
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;//드래그할때.
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent)override;//마우스가 위젯을 떠났을때.
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;//사용자가 위젯에 문가를 드롭했을때.
private:
	UItemBase* ItemRef;
};
