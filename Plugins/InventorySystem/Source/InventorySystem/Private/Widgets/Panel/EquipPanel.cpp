// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Panel/EquipPanel.h"
#include "Components/Button.h"
void UEquipPanel::OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent)
{
	Super::OnOwningPlayerUIComponentInitialized(OwningUIComponent);
	CloseBtn->OnClicked.AddUniqueDynamic(this, &ThisClass::CloseEquipPanel);
}

void UEquipPanel::CloseEquipPanel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

