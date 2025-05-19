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

	UE_LOG(LogTemp, Warning, TEXT("Overlap with: %s"), *OtherActor->GetName());
	if (HasAuthority() && !bIsPickedUp) // Doar serverul proceseaza pickup-ul
	{
		// verificam ca OtherActor este ThirdPersonMPCharacter
		if (AThirdPersonMPCharacter* Player = Cast<AThirdPersonMPCharacter>(OtherActor))
		{
			// obtinem componenta Inveotry de la player
			UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>();
			if (Inventory) // verificam ca exista inventory si ca itemclass e valid
			{
				if (!ItemDataTable)
				{
					UE_LOG(LogTemp, Error, TEXT("ItemDataTable is invalid!"));
				
				}
				else
				{
					FItemData* Row = ItemDataTable->FindRow<FItemData>(ItemRowName, TEXT(""));
					UItemBase* NewItem = NewObject<UItemBase>(this);
					//FMemory::Memcpy(NewItem, Row, sizeof(FItemData));

					NewItem->Quantity = Quantity;
					NewItem->ItemID = Row->ItemID;
					NewItem->OwningInventory = Player->GetInventoryComponent();
					NewItem->ItemType = Row->ItemType;
					NewItem->ItemQuality = Row->ItemQuality;
					NewItem->ItemStatistics = Row->ItemStatistics;
					NewItem->TextData = Row->TextData;
					NewItem->NumericData = Row->NumericData;
					NewItem->AssetData = Row->AssetData;
					NewItem->LevelRequirement = Row->LevelRequirement;
					NewItem->Tags = Row->Tags;
					NewItem->SpawnableActorClass = Row->SpawnableActorClass;
					NewItem->MaxDurability = Row->MaxDurability;
					NewItem->Cooldown = Row->Cooldown;
					NewItem->OnUse = Row->OnUse;


					if (Row)
					{
						UE_LOG(LogTemp, Warning, TEXT("[ItemPickup] Row found: %s"), *Row->ItemID.ToString());
						UE_LOG(LogTemp, Warning, TEXT("[ItemPickup] NewItem NameID: %s"), *NewItem->ItemID.ToString());

					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("[ItemPickup] Row NOT found!"));
					}

					// adaugam item in inventory
					if (Inventory->AddItem(NewItem))
					{
						if (Player->GetInventoryPanel())
						{
							Player->GetInventoryPanel()->RefreshInventory(Inventory);
						}
						bIsPickedUp = true;
						SetLifeSpan(0.2f);
						//Destroy();
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

