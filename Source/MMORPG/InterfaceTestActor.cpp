// Fill out your copyright notice in the Description page of Project Settings.

#include "InterfaceTestActor.h"
#include "ItemBase.h"
#include "ThirdPersonMPCharacter.h"



// Sets default values
AInterfaceTestActor::AInterfaceTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	ItemID = "test_001";
	Quantity = 1;
}

// Called when the game starts or when spawned
void AInterfaceTestActor::BeginPlay()
{
	Super::BeginPlay();

	InteractableData = InstanceInteractableData;
	
}

// Called every frame
void AInterfaceTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInterfaceTestActor::BeginFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(true);
	}
}

void AInterfaceTestActor::EndFocus()
{
	if (Mesh)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}

void AInterfaceTestActor::BeginInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling BeginInterract override on Interface Test Actor"));
}

void AInterfaceTestActor::EndInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling EndInteract override on Interface Test Actor"));
}

void AInterfaceTestActor::Interact(AThirdPersonMPCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Calling Interract override on Interface Test Actor"));
	

	if (Character)
	{
		if (Character->GetInventoryComponent())
		{
			if (Character->HasAuthority())
			{
				// Cream un nou item si il adaugam in inventar
				UItemBase* NewItem = NewObject<UItemBase>(UItemBase::StaticClass());

				if (NewItem)
				{
					NewItem->ItemID = ItemID;
					NewItem->Quantity = Quantity;

					if (ItemDatabase)
					{
						static const FString ContextString(TEXT("Item Data Context"));
						FItemData* ItemData = ItemDatabase->FindRow<FItemData>(ItemID, ContextString);

						if (ItemData)
						{
							// asignare proprietati/atribute item creat in inventory cu date din baza de date
							UE_LOG(LogTemp, Warning, TEXT("[InterfaceTestActor] Datele au fost gasite pentru %s"), *ItemID.ToString());
							NewItem->OwningInventory = Character->GetInventoryComponent();
							NewItem->ItemType = ItemData->ItemType;
							NewItem->ItemQuality = ItemData->ItemQuality;
							NewItem->ItemStatistics = ItemData->ItemStatistics;
							NewItem->TextData = ItemData->TextData;
							NewItem->NumericData = ItemData->NumericData;
							NewItem->AssetData = ItemData->AssetData;
							NewItem->LevelRequirement = ItemData->LevelRequirement;
							NewItem->Tags = ItemData->Tags;
							NewItem->SpawnableActorClass = ItemData->SpawnableActorClass;
							NewItem->MaxDurability = ItemData->MaxDurability;
							NewItem->Cooldown = ItemData->Cooldown;

						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("[InterfaceTestActor] Nu am gasit date pentru %s in DataTable!"), *ItemID.ToString());
						}

					}


					// Adaugam in inventar si verificam daca a functionat
					if (Character->GetInventoryComponent()->AddItem(NewItem))
					{
						UE_LOG(LogTemp, Warning, TEXT("[InterfaceTestActor] Item-ul %s a fost adaugat in inventar!"), *ItemID.ToString());
						Destroy();
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("[InterfaceTestActor] Nu s-a putut adauga %s in inventar."), *ItemID.ToString());
					}
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("[InterfaceTestActor] Eroare la creare instanta UItemBase!"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[InterfaceTestActor] Nu am authority pe client, nu pot adauga in inventar."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[InterfaceTestActor] InventoryComp este NULL!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[InterfaceTestActor] Character este null!"));
	}
}

