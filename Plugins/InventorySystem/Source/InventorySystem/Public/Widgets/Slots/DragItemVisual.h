// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InventoryWidgetBase.h"
#include "DragItemVisual.generated.h"

class UImage;
class UTextBlock;
class UButton;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UDragItemVisual : public UInventoryWidgetBase
{
	GENERATED_BODY()
public:
	void SetDragItemImage(TSoftObjectPtr<UTexture2D> InImage);
protected:
	virtual void OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent) override;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> QuantityText;
};
