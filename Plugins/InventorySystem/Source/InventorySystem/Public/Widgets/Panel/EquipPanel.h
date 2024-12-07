// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/InventoryWidgetBase.h"
#include "EquipPanel.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UEquipPanel : public UInventoryWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent) override;
private:
	UFUNCTION()
	void CloseEquipPanel();
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseBtn;
};
