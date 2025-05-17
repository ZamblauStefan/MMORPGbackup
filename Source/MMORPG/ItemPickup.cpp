// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickup.h"
#include "ItemBase.h"
#include "ThirdPersonMPCharacter.h"
#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AItemPickup::AItemPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AItemPickup::SetItemData(FName NewItemID, int32 NewQuantity)
{
	ItemID = NewItemID;
	Quantity = NewQuantity;
	OnRep_ItemData();
}

void AItemPickup::OnRep_ItemData()
{
	// update la UI sau modificari vizuale
	UE_LOG(LogTemp, Warning, TEXT("Item %s cu cantitatea %d a fost aruncat în lume!"), *ItemID.ToString(), Quantity);

}

void AItemPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemPickup, ItemID);
	DOREPLIFETIME(AItemPickup, Quantity);
}

void AItemPickup::OnInteract(AThirdPersonMPCharacter* InteractingCharacter)
{
	if (HasAuthority())
	{
		Server_HandlePickup(InteractingCharacter);
	}
	else
	{
		Server_HandlePickup(InteractingCharacter);
	}
}

void AItemPickup::Server_HandlePickup_Implementation(AThirdPersonMPCharacter* InteractingCharacter)
{
	if (InteractingCharacter)
	{
		UInventoryComponent* Inventory = InteractingCharacter->GetInventoryComponent();
		if (Inventory)
		{
			// Cream o instanta noua de UItemBase si setam datele
			UItemBase* NewItem = NewObject<UItemBase>(UItemBase::StaticClass());
			if (NewItem)
			{
				NewItem->ItemID = ItemID;
				NewItem->Quantity = Quantity;

				// adaugam item-ul in inventarul jucatorului
				if (Inventory->AddItem(NewItem))
				{
					UE_LOG(LogTemp, Warning, TEXT("%s a fost adaugat in inventar!"), *ItemID.ToString());

					// Daca s-a adaugat cu succes, distrugem obiectul din lume
					Destroy();
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Nu s-a putut adauga %s in inventar."), *ItemID.ToString());
				}
			}
		}
	}
}




// Called when the game starts or when spawned
void AItemPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

