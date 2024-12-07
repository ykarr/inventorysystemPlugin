// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InventoryWidgetBase.h"
#include "Tooltip.generated.h"

class UTextBlock;
class UInventorySlot;
class UImage;
class UItemBase;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UTooltip : public UInventoryWidgetBase
{
	GENERATED_BODY()
public:
	void SettingToolTip(UItemBase* ItemBase);
protected:
	virtual void OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemType;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemImage;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDescription;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageValue;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> UseageText;
};
