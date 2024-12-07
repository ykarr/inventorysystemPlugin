// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InventoryWidgetBase.h"
#include "InventorySlot.generated.h"

class UImage;
class UTextBlock;
class UBorder;
class UInventoryComponent;
class UTooltip;
class UDragItemVisual;
class UItemBase;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventorySlot : public UInventoryWidgetBase
{
	GENERATED_BODY()
public:
	void UpdateSlot(UItemBase* ItemBase);
	int32 SlotIndex;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TObjectPtr<UTooltip> InventoryToolTip;
protected:
	//virtual void NativeOnInitialized()override;
	virtual void OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent) override;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityText;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;
	UPROPERTY(EditDefaultsOnly, Category = "Inven")
	TObjectPtr<UTexture2D> ItemEmptyIcon;
	UPROPERTY(EditDefaultsOnly, Category = "Inven")
	TObjectPtr<UTexture2D> Hover;
	UPROPERTY(EditDefaultsOnly, Category = "Inven")
	TObjectPtr<UTexture2D> Normal;
	UPROPERTY(EditDefaultsOnly, Category = "Inven")
	TObjectPtr<UTexture2D> Pressed;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UDragItemVisual> DragItemVisualClass;
	//UPROPERTY(EditDefaultsOnly)
	//TSubclassOf<UInventoryToolTip> ToolTipClass;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UTooltip> InventoryToolTipClass;

private:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;//마우스 버튼 누를때.
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override; //마우스 더블클릭.
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;//드래그할때.
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent)override;//마우스가 위젯을 떠났을때.
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;//사용자가 위젯에 문가를 드롭했을때.
	
	void ASyncLoadImageBoard(TSoftObjectPtr<UTexture2D> InImage, TObjectPtr<UBorder> InItemBorder);
	void SetQuantityVisible(bool bActive) const;

	UPROPERTY(EditDefaultsOnly, Category = "Inven")
	UInventoryComponent* InventoryComponentRef;
	UPROPERTY(EditDefaultsOnly, Category = "Inven")
	UItemBase* ItemRef;
};
