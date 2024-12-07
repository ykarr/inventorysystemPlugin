// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDataTable.h"
#include "InventoryComponent.generated.h"

class UItemBase;
class UItemDropOverlay;
class UInputMappingContext;
class UInputAction;

DECLARE_DELEGATE_TwoParams(FOnSlotUpdateDelegate, int32, UItemBase*);
DECLARE_DELEGATE_TwoParams(FOnSlotChange, int32, int32);
DECLARE_DELEGATE_OneParam(FOnInventoryDropItem, int32);
DECLARE_DELEGATE_OneParam(FOnInventoryUseItem, int32);
DECLARE_DELEGATE_ThreeParams(FOnSetQuickSlotDelegate, FName, FName,bool);
DECLARE_DELEGATE_FourParams(FOnQuickSlotUpdate, FName , int32 , TSoftObjectPtr<UTexture2D>,FName);
/**
 *
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYSYSTEM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	void InitInventoryWidget();
protected:
	UInventoryComponent();
	virtual void BeginPlay() override;
private:
	//Inventory Data
	UPROPERTY(EditDefaultsOnly, Category = "InventorySettings")
	int32 Inventorycapacity; //Inventory SIze
	UPROPERTY(EditAnywhere, Category = "Inven")
	TMap<int32 , FName> InventoryMap;//Inventory Slot ItemName

	TMap<FName, TMap<int32, UItemBase*>> InventoryDataMap; //Inventory Data
	TMap<FName, FName> QuickSlotMap;
	UPROPERTY(EditDefaultsOnly, Category = "Inven")//Set ObjectType for Pickup
	TArray<TEnumAsByte<	EObjectTypeQuery> > ObjectTypes;
	TMultiMap<FName, FName> MultiMap;
	


	//Input
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInputAction(int32 Order);
	void OpenInventory();// KeyBoard I Key
	void PickupItemtoInventory();// KeyBoard G Key

	UPROPERTY(EditAnywhere, Category = "Inven")
	UInputMappingContext* InventoryContext;
	UPROPERTY(EditAnywhere, Category = "Inven")
	UInputAction* IA_InvenOpen;
	UPROPERTY(EditAnywhere, Category = "Inven")
	UInputAction* IA_ItemPickup;

	//Inventory UI
	UPROPERTY(EditDefaultsOnly, Category = "InventorySettings")
	TSoftClassPtr<UItemDropOverlay> InventoryOverlayClass;
	UItemDropOverlay* InventoryOverlay;

	//Inventory Add / Delete /Change
	bool AddNewItem(UItemBase* InItem);
	bool StackableItemAdd(UItemBase* InItem);
	bool NonStackableItemAdd(UItemBase* InItem);
	int32 FindNextPartialSlot(FName& ItemID);
	int32 CalculateNumberForFullStack(UItemBase* InItem, int32 InItemQauntity);
	int32 FindCheckEmptySlot(FName& InItemID);
	void InventoryItemDrop(int32 SlotNum);
	void InvenUseItem(int32 InItemSlot);//use
	void ItemSlotChange(int32 fromItem, int32 ToItem);

	//QuickSlots
	void SetQuickSlot(FName QuickSlotName, FName ItemID, bool bIsQuickSlotSet);
	void QuickItemUpdate(FName InItemID);
public:
	//Delegate
	FOnSlotUpdateDelegate OnSlotUpdateDelegate;
	FOnInventoryDropItem OnInventoryDropItem;
	FOnSlotChange OnSlotChange;
	FOnSetQuickSlotDelegate OnSetQuickSlotDelegate;
	FOnQuickSlotUpdate OnQuickSlotUpdate;
	FOnInventoryUseItem OnInventoryUseItem;

	FORCEINLINE int32 GetInventorycapacity() const { return Inventorycapacity; }
};
