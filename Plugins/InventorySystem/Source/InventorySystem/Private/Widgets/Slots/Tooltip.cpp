// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Slots/Tooltip.h"
#include "ItemBase.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
void UTooltip::SettingToolTip(UItemBase* ItemBase)
{
	DamageValue->SetVisibility(ESlateVisibility::Collapsed);
	if (ItemBase == nullptr) {
		return;
	}
	if (FItemData* ItemData = ItemBase->GetItemData()) {
		ItemName->SetText(FText::FromName(ItemData->ItemName));
		switch (ItemData->ItemType)
		{
		case EItemType::Armor:
			ItemType->SetText(FText::FromString("Armor"));
			break;
		case EItemType::Weapon:
			ItemType->SetText(FText::FromString("Weapon"));
			DamageValue->SetText(FText::AsNumber(ItemBase->GetItemData()->Damage));
			DamageValue->SetVisibility(ESlateVisibility::Visible);
			break;
		case EItemType::Shield:
			ItemType->SetText(FText::FromString("Shield"));
			break;
		case EItemType::Spell:
			ItemType->SetText(FText::FromString("Spell"));
			break;
		case EItemType::Consumable:
			ItemType->SetText(FText::FromString("Consumable"));
			break;
		case EItemType::Quest:
			ItemType->SetText(FText::FromString("Quest"));
			break;
		case EItemType::Mundane:
			ItemType->SetText(FText::FromString("Mundane"));
			break;
		default:
			break;
		}
		//ItemType->SetText(ItemData->);
		ItemImage->SetBrushFromSoftTexture(ItemData->ItemIcon);
		//ItemImage->SetBrushFromTexture(PickupItemBase->ItemData->ItemIcon);
		ItemDescription->SetText(ItemData->ItemDescription);
	}
}

void UTooltip::OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent)
{
	Super::OnOwningPlayerUIComponentInitialized(OwningUIComponent);
}

