// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "ItemBase.h"
#include "ItemPickup.h"
#include "ItemDataStructs.h"
#include "Net/UnrealNetwork.h"



// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	//SetIsReplicatedByDefault(true);

}

void UInventoryComponent::Client_AddItem_Implementation(const FItemData& ItemRow, int32 Quantity)
{
	UItemBase* NewItem = NewObject<UItemBase>(this);

	NewItem->Quantity = Quantity;
	NewItem->ItemID = ItemRow.ItemID;
	NewItem->OwningInventory = this;
	NewItem->ItemType = ItemRow.ItemType;
	NewItem->ItemQuality = ItemRow.ItemQuality;
	NewItem->ItemStatistics = ItemRow.ItemStatistics;
	NewItem->TextData = ItemRow.TextData;
	NewItem->NumericData = ItemRow.NumericData;
	NewItem->AssetData = ItemRow.AssetData;
	NewItem->LevelRequirement = ItemRow.LevelRequirement;
	NewItem->Tags = ItemRow.Tags;
	NewItem->SpawnableActorClass = ItemRow.SpawnableActorClass;
	NewItem->MaxDurability = ItemRow.MaxDurability;
	NewItem->Cooldown = ItemRow.Cooldown;
	NewItem->OnUse = ItemRow.OnUse;
	NewItem->WeaponAttackMontage = ItemRow.WeaponAttackMontage;
	NewItem->WeaponType = ItemRow.WeaponType;

	// verificare is Stackable

	if (NewItem->NumericData.bIsStackable)
	{
		for (UItemBase* ExistingItem : Items)
		{
			if (ExistingItem && ExistingItem->ItemID == NewItem->ItemID)
			{
				ExistingItem->Quantity += NewItem->Quantity;
				UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] Stack %s. Cantitate noua: %d"), *NewItem->ItemID.ToString(), ExistingItem->Quantity);

				OnInventoryUpdated.Broadcast(Items);
				return;
			}
			
		}
	}

		Items.Add(NewItem);
		OnInventoryUpdated.Broadcast(Items);


	UE_LOG(LogTemp, Warning, TEXT("[Client_AddItem] Adaug itemul %s cu cantitatea %d (Client-side)."), *ItemRow.ItemID.ToString(), Quantity);
}

bool UInventoryComponent::AddItem(UItemBase* Item)
{
	if (!Item || Items.Num() >= MaxItems)
		return false;

	// Verifica dubluri daca e necesar
	if (Item->NumericData.bIsStackable)
	{
		for (auto& ExistingItem : Items)
		{
			if (ExistingItem->ItemID == Item->ItemID)
			{
				ExistingItem->Quantity += Item->Quantity;
				return true;
			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] Adding item: %s"), *Item->ItemID.ToString()); // Debug 1
	Items.Add(Item);
	UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] Inventory now has %d items"), Items.Num()); // Debug 2
	OnInventoryUpdated.Broadcast(Items);
	return true;

}

void UInventoryComponent::RemoveItem(UItemBase* Item, int32 Quantity)
{
	if (Item)
	{
		Item->Quantity -= Quantity;
		if (Item->Quantity <= 0)
		{
			Items.Remove(Item);
		}

		// Trigger UI update
		if (OnInventoryUpdated.IsBound())
		{
			OnInventoryUpdated.Broadcast(Items);
		}
		

	}
}



void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

