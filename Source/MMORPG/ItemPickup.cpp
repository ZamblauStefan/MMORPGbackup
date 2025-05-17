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
	SetReplicates(true);

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

