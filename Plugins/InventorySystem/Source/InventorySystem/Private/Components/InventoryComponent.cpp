// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
//UI
#include "Widgets/Overlay/ItemDropOverlay.h"
//ASyncLoad
#include "Engine/AssetManager.h"
//#include "Widgets/Slots/InventorySlot.h"
#include "ItemActor.h"
//InputSetting
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
//Controller
#include "GameFramework/PlayerController.h"
//Pickup item
#include "ItemBase.h"
#include "Kismet/KismetSystemLibrary.h"
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Inventorycapacity = 32;
	OnInventoryDropItem.BindUObject(this, &ThisClass::InventoryItemDrop);
	OnSlotChange.BindUObject(this, &ThisClass::ItemSlotChange);
	OnSetQuickSlotDelegate.BindUObject(this, &ThisClass::SetQuickSlot);
	OnInventoryUseItem.BindUObject(this, &ThisClass::InvenUseItem);
}
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	InitInventoryWidget();
	FInputModeGameAndUI InputMode;
	Cast<APawn>(GetOwner())->GetController<APlayerController>()->SetInputMode(InputMode);
	Cast<APawn>(GetOwner())->GetController<APlayerController>()->SetShowMouseCursor(true);
	SetInputAction(1);
}
//Inventory Init
void UInventoryComponent::InitInventoryWidget()
{
	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(InventoryOverlayClass.ToSoftObjectPath(), FStreamableDelegate::CreateLambda([this]() {
		APlayerController* PlayerController = Cast<APawn>(GetOwner())->GetController<APlayerController>();
		if (InventoryOverlayClass.Get()) {
			InventoryOverlay=CreateWidget<UItemDropOverlay>(PlayerController, InventoryOverlayClass.Get());
			InventoryOverlay->AddToViewport();
			InventoryOverlay->InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
			InventoryOverlay->EquipSystem->SetVisibility(ESlateVisibility::Collapsed);
		}
	}));
}
//Inventory UI
void UInventoryComponent::SetInputAction(int32 Order)
{
	if (APawn* Character = Cast<APawn>(GetOwner())) {
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController())) {
			UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			if (Subsystem) {
				Subsystem->AddMappingContext(InventoryContext, Order);
			}
			UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
			if (EnhancedInputComponent) {
				EnhancedInputComponent->BindAction(IA_InvenOpen, ETriggerEvent::Started, this, &UInventoryComponent::OpenInventory);
				EnhancedInputComponent->BindAction(IA_ItemPickup, ETriggerEvent::Triggered, this, &UInventoryComponent::PickupItemtoInventory);
			}
		}
	}
}
void UInventoryComponent::OpenInventory()
{
	if (InventoryOverlay->InventoryWidget->GetVisibility() == ESlateVisibility::Visible ||
		InventoryOverlay->EquipSystem->GetVisibility() == ESlateVisibility::Visible) {
		InventoryOverlay->InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
		InventoryOverlay->EquipSystem->SetVisibility(ESlateVisibility::Collapsed);
	}
	else {
		InventoryOverlay->InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		InventoryOverlay->EquipSystem->SetVisibility(ESlateVisibility::Visible);
	}
}
void UInventoryComponent::PickupItemtoInventory()
{
	TArray<FHitResult> Hits;
	FHitResult Hit;
	const FVector UpVec = GetOwner()->GetActorUpVector();
	const FVector Start = GetOwner()->GetActorLocation();// +UpVec * 100;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(GetOwner());
	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(GetOwner());
	TArray<AActor*> OutActors;

	if (UKismetSystemLibrary::CapsuleOverlapActors(GetWorld(), Start, 100, 100, ObjectTypes, AItemActor::StaticClass(), ActorToIgnore, OutActors)) {
		DrawDebugCapsule(GetWorld(), Start, 100, 100, FQuat::Identity, FColor::Green);
		for (AActor* HitActor : OutActors) {
			if (AItemActor* ItemActor = Cast<AItemActor>(HitActor)) {
				if (AddNewItem(ItemActor->ItemBase)) {
					UE_LOG(LogTemp, Display, TEXT("Success"));
					ItemActor->Destroy();
				}
			}
		}
	}
	else {
		DrawDebugCapsule(GetWorld(), Start, 100, 100, FQuat::Identity, FColor::Red);
	}
}
//QuickSlot 
void UInventoryComponent::SetQuickSlot(FName QuickSlotName, FName ItemID, bool bIsQuickSlotSet)
{
	if (bIsQuickSlotSet) {
		MultiMap.Add({ ItemID ,QuickSlotName });
	}
	else {
		if (MultiMap.FindPair(ItemID, QuickSlotName))
		{
			MultiMap.RemoveSingle(ItemID, QuickSlotName); // FSS- KKK 쌍만 제거
			OnQuickSlotUpdate.ExecuteIfBound(QuickSlotName, -1, nullptr, "");
			return;
		}
	}
	QuickItemUpdate(ItemID);

}
void UInventoryComponent::QuickItemUpdate(FName InItemID)
{
	int32 Sum = 0;
	FItemData* QuickSlotItemData;
	if (MultiMap.Contains(InItemID)) {
		if (InventoryDataMap.Contains(InItemID)) {
			if (TMap<int32, UItemBase*>* ItemBaseTests = InventoryDataMap.Find(InItemID)) {
				for (TPair<int32, UItemBase*> Pair : *ItemBaseTests) {
					Sum += FMath::Clamp(Pair.Value->ItemQuantity, 0, 9999);
					
					QuickSlotItemData = Pair.Value->GetItemData();
				}
			}
		}
		if (MultiMap.Contains(InItemID)) {
			UE_LOG(LogTemp, Display, TEXT("%s"),*FText::AsNumber(Sum).ToString());
			TArray<FName> FoundQuickSlotValues;
			MultiMap.MultiFind(InItemID, FoundQuickSlotValues);
			for (auto& FoundValue : FoundQuickSlotValues) {
				//아이템 갯수가 1보다 작은경우 자동으로 퀵슬롯 비활성.
				OnQuickSlotUpdate.Execute(FoundValue, Sum, QuickSlotItemData->ItemIcon, InItemID);
			}
			if (Sum <1) {
				MultiMap.Remove(InItemID);
			}
		}
		
		
	}
}
//Inventory Add / Use / Drop / Change
bool UInventoryComponent::AddNewItem(UItemBase* InItem)
{
	if (InItem->GetItemData()->bIsStackable) {
		return StackableItemAdd(InItem);
	}
	else {
		return NonStackableItemAdd(InItem);
	}
}
bool UInventoryComponent::StackableItemAdd(UItemBase* InItem)
{
	FName ItemID = InItem->ItemID;
	FItemData* ItemData = InItem->GetItemData();
	if (!ItemData) {
		return false;
	}
	if (InventoryMap.Num()<= Inventorycapacity) {
		int32 AmountToDistribute = InItem->ItemQuantity;
		int32 StackItemNum = FindNextPartialSlot(ItemID);
		while (AmountToDistribute > 0 && StackItemNum >= 0) {
			InventoryMap[StackItemNum] = ItemID;
			TMap<int32, UItemBase*>* ChangeItemDatas =InventoryDataMap.Find(ItemID);
			//for(FInvenItemData& ChangeItemData : *ChangeItemDatas){
			UItemBase* FindItemData = *ChangeItemDatas->Find(StackItemNum);
				//if (ChangeItemData.ItemSlot== StackItemNum) {
			int32 InventoryItemQuantity = FindItemData->ItemQuantity;
			const int32 AmountMakeFullStack = CalculateNumberForFullStack(FindItemData, AmountToDistribute);
			AmountToDistribute -= AmountMakeFullStack;
			FindItemData->ItemQuantity = InventoryItemQuantity + AmountMakeFullStack;
			//위젯 업데이트
			QuickItemUpdate(ItemID);
			OnSlotUpdateDelegate.ExecuteIfBound(StackItemNum, FindItemData);
			//End
			StackItemNum = FindNextPartialSlot(ItemID);
			if (AmountToDistribute <= 0) return true; //모든 아이템이 스택에 들어감.
		}
		InItem->ItemQuantity = AmountToDistribute;
		//스택중인 아이템이 없으므로 새로운 칸에 아이템 추가.
		int32 EmptySlot = FindCheckEmptySlot(ItemID);
		if (EmptySlot != -1) {
			// 빈 슬롯을 찾은 경우.
			InventoryMap.Add(EmptySlot, ItemID);
			// InventoryDataMap의 데이터를 직접 수정
			if (TMap<int32, UItemBase*>* FoundItems = InventoryDataMap.Find(ItemID)) {
				// 기존 데이터에 새 항목 추가
				FoundItems->Add(EmptySlot, InItem);
			}
			else {
				// 데이터가 없으면 새 TMap 생성 후 추가
				TMap<int32, UItemBase*> NewItemData;
				NewItemData.Add(EmptySlot, InItem);
				InventoryDataMap.Add(ItemID, NewItemData);
			}
			// 위젯 업데이트
			QuickItemUpdate(ItemID);
			OnSlotUpdateDelegate.ExecuteIfBound(EmptySlot, InItem);
			return true;
		}
	}
	return false;
}
bool UInventoryComponent::NonStackableItemAdd(UItemBase* InItem)
{
	FName ItemID = InItem->ItemID;
	InItem->ItemQuantity = 1;
	int32 EmptySlot = FindCheckEmptySlot(ItemID);
	if (EmptySlot != -1) {
		// 빈 슬롯을 찾은 경우.
		InventoryMap.Add(EmptySlot, ItemID);
		// InventoryDataMap의 데이터를 직접 수정
		if (TMap<int32, UItemBase*>* FoundItems = InventoryDataMap.Find(ItemID)) {
			// 기존 데이터에 새 항목 추가
			FoundItems->Add(EmptySlot, InItem);
		}
		else {
			// 데이터가 없으면 새 TMap 생성 후 추가
			TMap<int32, UItemBase*> NewItemData;
			NewItemData.Add(EmptySlot, InItem);
			InventoryDataMap.Add(ItemID, NewItemData);
		}
		QuickItemUpdate(ItemID);
		// 위젯 업데이트
		QuickItemUpdate(ItemID);
		OnSlotUpdateDelegate.ExecuteIfBound(EmptySlot, InItem);
		return true;
	}
	return false;
}
void UInventoryComponent::InventoryItemDrop(int32 SlotNum)
{
	if (FName* DropSlotItemID = InventoryMap.Find(SlotNum)) {
		if (TMap<int32,UItemBase*>* ChangeItemDatas = InventoryDataMap.Find(*DropSlotItemID)) {
			if (UItemBase* ItemBase = *ChangeItemDatas->Find(SlotNum)) {
				TSoftClassPtr<AActor>ItemActor = ItemBase->GetItemData()->BaseActor;
				FActorSpawnParameters SpawnParms;
				UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(ItemActor.ToSoftObjectPath(), [this, ItemBase, ItemActor, SlotNum]() {
					const FVector SpawnLocation{ GetOwner()->GetActorLocation() };
					const FTransform SpawnTransform(GetOwner()->GetActorRotation(), SpawnLocation);
					FActorSpawnParameters SpawnParams;
					SpawnParams.Owner = GetOwner();
					SpawnParams.bNoFail = true;//bNoFail을 true로 설정하면 실패해도 오류를 생성하지 않고 작업을 계속할 수 있습니다
					//브젝트를 스폰할 때 충돌이 발생해도 해당 위치에 오브젝트를 스폰할 수 있습니다.
					SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
					TObjectPtr<AItemActor> Pickup = GetWorld()->SpawnActor<AItemActor>(ItemActor.Get(), SpawnTransform, SpawnParams);
					Pickup->ItemBase = ItemBase;
					//위젯 업데이트*
					//
					OnSlotUpdateDelegate.ExecuteIfBound(SlotNum, nullptr);
					//End
					FName RemoveID = *InventoryMap.Find(SlotNum);
					InventoryMap.Remove(SlotNum);
					//Quick슬롯 모두 버리는경우 변경됨.	
					if (InventoryDataMap.Find(RemoveID)->Num() < 1) {
						InventoryDataMap.Remove(RemoveID);
					}
					else {
						TMap<int32,UItemBase*>* FindItemDataToDelete = InventoryDataMap.Find(RemoveID);
						FindItemDataToDelete->Remove(SlotNum);
					}
					QuickItemUpdate(RemoveID);
				});
			}
		}
	}
}
int32 UInventoryComponent::FindNextPartialSlot(FName& ItemID)
{
	if (!InventoryDataMap.Contains(ItemID)) {
		return -1;
	}
	if (TMap<int32, UItemBase*>* FoundItems = InventoryDataMap.Find(ItemID)) {
		TArray<int32> SortedKeys;
		FoundItems->GetKeys(SortedKeys);
		// 2. 키를 오름차순으로 정렬
		SortedKeys.Sort();
		// 3. 정렬된 키를 기반으로 TMap의 값 순회 및 처리
		for (int32 Key : SortedKeys)
		{
			UItemBase* ItemBase = *FoundItems->Find(Key);
			if (!ItemBase->IsFullItemStack()) {
				return Key;
			}
		}
	}
	return -1;
}
int32 UInventoryComponent::CalculateNumberForFullStack(UItemBase* InItem, int32 InItemQauntity)
{
	const int32 AmountMakeFullStack = InItem->GetItemData()->MaxStackAmount - InItem->ItemQuantity;
	return FMath::Min(InItemQauntity, AmountMakeFullStack);
}
int32 UInventoryComponent::FindCheckEmptySlot(FName& InItemID)
{
	for (int i = 0; i < Inventorycapacity; i++) {
		if (!InventoryMap.Contains(i)) return i;
	}
	return -1;
}
void UInventoryComponent::ItemSlotChange(int32 fromItem, int32 toItem)
{
	FName ItemName1;
	FName ItemName2;
	UItemBase* ItemToMove1 = nullptr;
	UItemBase* ItemToMove2 = nullptr;
	if (InventoryMap.Contains(fromItem)) {
		ItemName1 = *InventoryMap.Find(fromItem);
		InventoryMap.Remove(fromItem);
		TMap<int32, UItemBase*>* FindItemData1 = InventoryDataMap.Find(ItemName1);
		if (!FindItemData1->RemoveAndCopyValue(fromItem, ItemToMove1))
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to remove and copy value from slot: %d"), fromItem);
			return;
		}
		if (InventoryMap.Contains(toItem)) {
			ItemName2 = *InventoryMap.Find(toItem);
			InventoryMap.Remove(toItem);
			TMap<int32, UItemBase*>* FindItemData2 = InventoryDataMap.Find(ItemName2);
			if (!FindItemData2->RemoveAndCopyValue(toItem, ItemToMove2))
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to remove and copy value from slot: %d"), toItem);
				return;
			}
			InventoryMap.Add({ fromItem ,ItemName2 });
			InventoryMap.Add({ toItem ,ItemName1 });
			FindItemData1->Add({ toItem ,ItemToMove1 });
			FindItemData2->Add({ fromItem ,ItemToMove2 });
		}
		else {
			InventoryMap.Add({ toItem ,ItemName1 });
			FindItemData1->Add({ toItem ,ItemToMove1 });
		}
		OnSlotUpdateDelegate.ExecuteIfBound(toItem, ItemToMove1);
		OnSlotUpdateDelegate.ExecuteIfBound(fromItem, ItemToMove2);
	}
}
void UInventoryComponent::InvenUseItem(int32 InItemSlot)
{
	TSoftClassPtr<AActor>ItemActor;
	if (!InventoryMap.Contains(InItemSlot)) {
		return;
	}
	if (FName* DropSlotItemID = InventoryMap.Find(InItemSlot)) {
		if (!InventoryDataMap.Contains(*DropSlotItemID)) {
			return;
		}
		if (TMap<int32, UItemBase*>* ChangeItemDatas = InventoryDataMap.Find(*DropSlotItemID)) {
			if (UItemBase* ItemBase = *ChangeItemDatas->Find(InItemSlot)) {
				if (!ItemBase->GetItemData()->bIsCanUseable) {
					//사용할 수 없는 아이템.
					return;
				}
				ItemActor = ItemBase->GetItemData()->BaseActor;
				UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(ItemActor.ToSoftObjectPath(), [this, ItemActor, InItemSlot]() {

					if (FName* DropSlotItemID = InventoryMap.Find(InItemSlot)) {
						if (TMap<int32, UItemBase*>* ChangeItemDatas = InventoryDataMap.Find(*DropSlotItemID)) {
							UItemBase* ItemBase = *ChangeItemDatas->Find(InItemSlot);
							if (ItemActor.Get()) {
								FActorSpawnParameters SpawnParms;
								const FVector SpawnLocation{ GetOwner()->GetActorLocation() };
								const FTransform SpawnTransform(GetOwner()->GetActorRotation(), SpawnLocation);
								FActorSpawnParameters SpawnParams;
								SpawnParams.Owner = GetOwner();
								SpawnParams.bNoFail = true;//bNoFail을 true로 설정하면 실패해도 오류를 생성하지 않고 작업을 계속할 수 있습니다
								//브젝트를 스폰할 때 충돌이 발생해도 해당 위치에 오브젝트를 스폰할 수 있습니다.
								SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
								AItemActor* PickupItem = GetWorld()->SpawnActor<AItemActor>(ItemActor.Get(), SpawnTransform, SpawnParams);
								ItemBase->ItemQuantity -= 1;
								QuickItemUpdate(*DropSlotItemID);
								if (ItemBase->ItemQuantity > 0) {
									//ChangeItemDatas->Remove(InItemSlot);
									ChangeItemDatas->Add({ InItemSlot, ItemBase });
									//위젯 업데이트
									OnSlotUpdateDelegate.ExecuteIfBound(InItemSlot, ItemBase);
								}
								else {
									ChangeItemDatas->Remove(InItemSlot);
									InventoryMap.Remove(InItemSlot);
									//위젯 업데이트
									OnSlotUpdateDelegate.ExecuteIfBound(InItemSlot, nullptr);
								}
							}
						}
					}
					});
			}
		}
	}
				
				
				

}

