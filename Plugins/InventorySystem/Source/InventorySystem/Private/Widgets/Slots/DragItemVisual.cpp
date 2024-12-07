// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Slots/DragItemVisual.h"
#include "Components/Image.h"
void UDragItemVisual::SetDragItemImage(TSoftObjectPtr<UTexture2D> InImage)
{
	ItemImage->SetBrushFromSoftTexture(InImage);
}

void UDragItemVisual::OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent)
{
	Super::OnOwningPlayerUIComponentInitialized(OwningUIComponent);
}
