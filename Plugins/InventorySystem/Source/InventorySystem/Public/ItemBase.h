// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataTable.h"
#include "ItemBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class INVENTORYSYSTEM_API UItemBase : public UObject
{
	GENERATED_BODY()
public:
	void SetItemData();
	UPROPERTY(EditAnywhere, Category = "ItemData")
	int32 ItemQuantity;
	UPROPERTY(EditAnywhere, Category = "ItemData")
	FName ItemID;
	UPROPERTY(EditAnywhere, Category = "ItemData")
	UDataTable* ItemDataTable;
	void UseItem();
protected:
	
	
	
private:
	FItemData ItemData;
public:
	FORCEINLINE bool IsFullItemStack() const { return ItemQuantity >= ItemData.MaxStackAmount; };
	FORCEINLINE FItemData* GetItemData() { return &ItemData; };
};
