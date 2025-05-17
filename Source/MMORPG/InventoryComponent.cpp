// Fill out your copyright notice in the Description page of Project Settings.

#include "InventoryComponent.h"
#include "ItemBase.h"
#include "ItemPickup.h"
#include "Net/UnrealNetwork.h"



// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);

	// ...
}

bool UInventoryComponent::AddItem(UItemBase* NewItem)
{
	if (!NewItem)
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryComponent] Item este NULL!"));
		return false;
	}

	if (!GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryComponent] GetOwner() este NULL!"));
		return false;
	}

	if (GetOwnerRole() == ROLE_Authority)
	{
		Items.Add(NewItem);
		UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] Item %s a fost adaugat in lista de iteme!"), *NewItem->ItemID.ToString());

		// Trigger pentru delegate-ul pentru UI
		if (OnInventoryUpdated.IsBound())
		{
			UE_LOG(LogTemp, Warning, TEXT("[InventoryComponent] Delegate-ul este legat! Facem broadcast!"));
			OnInventoryUpdated.Broadcast();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[InventoryComponent] Delegate-ul OnInventoryUpdated NU este legat!"));
		}

		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Item-ul %s nu s-a putut adauga (nu avem Authority)."), *NewItem->ItemID.ToString());
		Server_AddItem(NewItem);
	}
	return false;
}

void UInventoryComponent::Server_AddItem_Implementation(UItemBase* NewItem)
{
	AddItem(NewItem);
}


UItemBase* UInventoryComponent::GetItem(FName ItemID) const
{
	for (UItemBase* Item : Items)
	{
		if (Item && Item->ItemID == ItemID)
		{
			return Item;
		}
	}
	return nullptr;
}

bool UInventoryComponent::RemoveItem(FName ItemID, int32 Quantity)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		UItemBase* Item = GetItem(ItemID);
		if (Item && Item->Quantity >= Quantity)
		{
			Item->SetQuantity(Item->Quantity - Quantity);
			if (Item->Quantity <= 0)
			{
				Items.Remove(Item);
			}
			return true;
		}
	}
	return false;
}


bool UInventoryComponent::HasItem(FName ItemID, int32 Quantity) const
{
	UItemBase* Item = GetItem(ItemID);
	return Item && Item->Quantity >= Quantity;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UInventoryComponent, Items);
}

// drop item from inventory
void UInventoryComponent::DropItem(FName ItemID, int32 Quantity)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		Server_DropItem(ItemID, Quantity);
	}
	else
	{
		Server_DropItem(ItemID, Quantity);
	}
}


void UInventoryComponent::Server_DropItem_Implementation(FName ItemID, int32 Quantity)
{
	UItemBase* Item = GetItem(ItemID);

	if (Item && Item->Quantity >= Quantity)
	{
		// creare un pickup in lume
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = Cast<APawn>(GetOwner());

		FVector SpawnLocation = GetOwner()->GetActorLocation() + GetOwner()->GetActorForwardVector() * 100;
		FRotator SpawnRotation = GetOwner()->GetActorRotation();

		AItemPickup* DroppedItem = GetWorld()->SpawnActor<AItemPickup>(
			Item->SpawnableActorClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);

		if (DroppedItem)
		{
			// setare date item pe actorul din lume
			DroppedItem->SetItemData(Item->ItemID, Quantity);

			// se scade cantitatea din inventar
			RemoveItem(ItemID, Quantity);

			// replicare pe toti clientii
			DroppedItem->SetReplicates(true);
			DroppedItem->SetReplicateMovement(true);
		}
	}
}



void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

