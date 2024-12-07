// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
//Item
//#include "ItemBase.h"
#include "Components/SphereComponent.h"
#include "Components/InventoryComponent.h"
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	/*ItemMesh*/
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(GetRootComponent());
	ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	//ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	//ItemMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	SetRootComponent(ItemMesh);

	ItemPickupSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ItemPickupSphereComponent"));
	ItemPickupSphereComponent->SetupAttachment(ItemMesh);
	ItemPickupSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnItemBeginOverlap);
	ItemPickupSphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ItemPickupSphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Ignore);
	//ItemPickupSphereComponent->OnComponentEndOverlap.AddDynamic(this, &APickupItemBase::OnItemEndOverlap);
	/*debug*/
	ItemMesh->SetCanEverAffectNavigation(false);
	ItemMesh->SetSimulatePhysics(true);

	ItemID = "HealthPotion1";

}

void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Display, TEXT("begin"));
	InitItemData();
}

void AItemActor::InitItemData()
{
	if (!ItemBase) {
		UE_LOG(LogTemp, Display, TEXT("Init"));
		ItemBase = NewObject<UItemBase>(this, ItemBaseClass);
		ItemBase->ItemDataTable = ItemDataTable;
		ItemBase->ItemID = ItemID;
		ItemBase->ItemQuantity = ItemQuantity;
		ItemBase->SetItemData();
	}
}

void AItemActor::PickupItem()
{

}

void AItemActor::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APawn* Player = Cast<APawn>(OtherActor)) {
		if (UInventoryComponent* PlayerUIComponent = Cast<UInventoryComponent>(Player->GetComponentByClass(UInventoryComponent::StaticClass()))) {
			//if (PlayerUIComponent->AddNewItem(ItemBase)) {
				//UE_LOG(LogTemp, Display, TEXT("Success"));
				//Destroy();
			//}
		}
	}
}

void AItemActor::UseItem()
{

}


