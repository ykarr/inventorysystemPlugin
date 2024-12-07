// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

void UItemBase::SetItemData()
{
	if (ItemDataTable && !ItemID.IsNone()) {
		ItemData = *ItemDataTable->FindRow<FItemData>(ItemID,ItemID.ToString());
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Not Set APickupItemBase::SetItemData(ItemDataTable||ItemID)"));
	}
}

void UItemBase::UseItem()
{

}
