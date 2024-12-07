// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/InventoryWidgetBase.h"
#include "Components/InventoryComponent.h"
void UInventoryWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if (UInventoryComponent* PlayerUIComponent = Cast<UInventoryComponent>(GetOwningPlayerPawn()->GetComponentByClass(UInventoryComponent::StaticClass()))) {
		
		OnOwningPlayerUIComponentInitialized(PlayerUIComponent);
	}
}

void UInventoryWidgetBase::OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent)
{
}
