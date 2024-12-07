// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYSYSTEM_API UItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
public:
	int32 FromIndex;
	int32 toIndex;
	int32 ItemReferenceIndex;
	FName QuickSlotName;
	FName ItemID;
};
