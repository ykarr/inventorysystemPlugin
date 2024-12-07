// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.h"
#include "ItemActor.generated.h"

class USphereComponent;
//class UItemBase;

UCLASS()
class INVENTORYSYSTEM_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, Category = "ItemData")
	UItemBase* ItemBase;
	UPROPERTY(EditAnywhere, Category = "ItemData")
	TSubclassOf<UItemBase> ItemBaseClass;
protected:
	AItemActor();
	virtual void BeginPlay() override;
	void InitItemData();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Components")
	TObjectPtr<UStaticMeshComponent> ItemMesh;

	UPROPERTY(EditAnywhere, Category = "ItemData")
	FName ItemID;
	UPROPERTY(EditAnywhere, Category = "ItemData")
	int32 ItemQuantity;
	
	UPROPERTY(EditAnywhere, Category = "ItemData")
	UDataTable* ItemDataTable;
public:	
	void PickupItem();
	UFUNCTION()
	void OnItemBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	USphereComponent* ItemPickupSphereComponent;

	int32 tmp;

	void UseItem();

};
