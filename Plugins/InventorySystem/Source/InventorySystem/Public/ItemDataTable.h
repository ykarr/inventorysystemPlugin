// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemDataTable.generated.h"

UENUM(BlueprintType)
enum class EItemType :uint8 {
	Armor UMETA(DisplayName = "Armor"),
	Weapon UMETA(DisplayName = "Weapon"),
	Shield UMETA(DisplayName = "Shield"),
	Spell UMETA(DisplayName = "Spell"),
	Consumable UMETA(DisplayName = "Consumable"),
	Quest UMETA(DisplayName = "Quest"),
	Mundane  UMETA(DisplayName = "Mundane")
};
USTRUCT(BlueprintType)
struct FItemData :public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	//UPROPERTY(EditDefaultsOnly)
	//FName ItemID;
	UPROPERTY(EditDefaultsOnly,  Category = "Inventory")
	FName ItemName;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	EItemType ItemType;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 MaxStackAmount;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSoftObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	FText ItemDescription;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bIsStackable;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	bool bIsCanUseable;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	float Damage;
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSoftClassPtr<AActor> BaseActor;
};

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UItemDataTable : public UDataTable
{
	GENERATED_BODY()
	
};
