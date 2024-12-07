// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UInventoryWidgetBase : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized()override;
	/*virtual void NativePreConstruct()override;
	virtual void NativeConstruct()override;
	virtual void NativeDestruct()override;*/

	UFUNCTION(BlueprintCallable, Category = "Inventory", meta = (DisplayName = "OnOwningPlayerUIComponentInitialized"))
	virtual void OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent);
};
