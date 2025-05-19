// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPickup.h"
#include "ItemBase.h"
#include "InventoryPanel.h"
#include "ThirdPersonMPCharacter.h"
#include "Components/SphereComponent.h"
#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AItemPickup::AItemPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	//SetReplicates(true);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>("CollisionSphere");
	RootComponent = CollisionSphere;
	CollisionSphere->SetCollisionProfileName("OverlapAllDynamic");

}

void AItemPickup::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && !bIsPickedUp) // Doar serverul proceseaza pickup-ul
	{
		// verificam ca OtherActor este ThirdPersonMPCharacter
		if (AThirdPersonMPCharacter* Player = Cast<AThirdPersonMPCharacter>(OtherActor))
		{
			// obtinem componenta Inveotry de la player
			UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>();
			if (Inventory) // verificam ca exista inventory si ca itemclass e valid
			{
				// cream instanta a item-ului
				UItemBase* NewItem = NewObject<UItemBase>(this);
				if (ItemDataTable)
				{
					UItemBase* Row = ItemDataTable->FindRow<UItemBase>(ItemRowName, TEXT("Context"));
					FMemory::Memcpy(NewItem, Row, sizeof(UItemBase));

					// adaugam item in inventory
					if (Inventory->AddItem(NewItem))
					{
						if (Player->GetInventoryPanel())
						{
							Player->GetInventoryPanel()->RefreshInventory(Inventory);
						}
						bIsPickedUp = true;
						
						Destroy();
					}
				}
			}
		}
	}
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
	DOREPLIFETIME(AItemPickup, bIsPickedUp);

	//DOREPLIFETIME(AItemPickup, ItemID);
	//DOREPLIFETIME(AItemPickup, Quantity);
}

void AItemPickup::OnRep_IsPickedUp()
{
	if (bIsPickedUp) Destroy(); // Distruge actorul pe toti clientii
}

/*
void AItemPickup::Interact(AThirdPersonMPCharacter* InteractingCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("[ItemPickup] Am ajuns in Interact cu itemul %s!"), *ItemID.ToString());

	if (InteractingCharacter)
	{
		InteractingCharacter->Server_PickupItem(this);

		if (InteractingCharacter->HasAuthority())
		{
			InteractingCharacter->Server_PickupItem(this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[ItemPickup] Nu am authority pe client, nu pot trimite catre server!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ItemPickup] Character este null!"));
	}

}
*/


// Called when the game starts or when spawned
void AItemPickup::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemPickup::OnOverlapBegin);
	
}


// Called every frame
void AItemPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

