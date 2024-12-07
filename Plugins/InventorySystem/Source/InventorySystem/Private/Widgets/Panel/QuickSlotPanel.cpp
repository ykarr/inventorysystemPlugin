// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Panel/QuickSlotPanel.h"
#include "Widgets/Slots/Tooltip.h"
#include "Engine\AssetManager.h"
#include "Components/InventoryComponent.h"
#include "Widgets/Slots/QuickSlot.h"
#include "Components/HorizontalBox.h"


void UQuickSlotPanel::OnOwningPlayerUIComponentInitialized(UInventoryComponent* OwningUIComponent)
{
	Super::OnOwningPlayerUIComponentInitialized(OwningUIComponent);
	InventoryComponentRef = OwningUIComponent;
	InventoryComponentRef->OnQuickSlotUpdate.BindUObject(this,&ThisClass::UpdateInventoryQuickSlotToMatchName);
}

void UQuickSlotPanel::UpdateInventoryQuickSlotToMatchName(FName Index, int32 InItemNum, TSoftObjectPtr<UTexture2D> ItemData, FName InItemID)
{
	
	// 디버그 로그로 함수 호출 확인
	UE_LOG(LogTemp, Log, TEXT("UpdateInventoryQuickSlotToMatchName called"));
	UE_LOG(LogTemp, Log, TEXT("Index: %s, InItemNum: %d"), *Index.ToString(), InItemNum);
	// ItemData 디버깅
	//UE_LOG(LogTemp, Log, TEXT("ItemData Name: %s"), *ItemData.ItemName.ToString());
	
	UQuickSlot* QuickSlot = *QuickSlotArrays.Find(Index);
	QuickSlot->ItemID = InItemID;
	QuickSlot->UpdateQuickSlot(InItemNum,ItemData);
}

void UQuickSlotPanel::NativeConstruct()
{
	Super::NativeConstruct();
	InventoryToolTip = CreateWidget<UTooltip>(GetOwningPlayer(), InventoryToolTipClass);
	InitQuickSlot();
}

void UQuickSlotPanel::InitQuickSlot()
{
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(QuickSlotClass.ToSoftObjectPath(), [this]() {
		if (QuickSlotClass.Get()) {
			for (int32 i = 0; i < 10; i++) {
				TObjectPtr<UQuickSlot> QuickSlot = CreateWidget<UQuickSlot>(GetOwningPlayer(), QuickSlotClass.Get());
				FString SlotNameString = FString::Printf(TEXT("QuickSlot%d"), i);
				FName SlotName = FName(*SlotNameString);
				QuickSlot->QuickSlotName = SlotName;
				QuickSlot->InventoryToolTip = InventoryToolTip;
				//QuickSlot->InventoryComponentRef = InventoryComponentRef;
				QuickSlotHorizontalBox->AddChild(QuickSlot);
				QuickSlotArrays.Add({ SlotName,QuickSlot });
			}
		}
	});
}

