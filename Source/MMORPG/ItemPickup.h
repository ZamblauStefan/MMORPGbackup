// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "ItemPickup.generated.h"

class AThirdPersonMPCharacter;

UCLASS()
class MMORPG_API AItemPickup : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemPickup();

	UPROPERTY(ReplicatedUsing = OnRep_ItemData)
	FName ItemID;
	UPROPERTY(ReplicatedUsing = OnRep_ItemData)
	int32 Quantity;

	// Data pentru interactiune
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	//FInteractableData InstanceInteractableData;

	// Referinta catre tabel
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	//UDataTable* ItemTable;

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	virtual void Interact(AThirdPersonMPCharacter* InteractingCharacter) override;

	UFUNCTION()
	void SetItemData(FName NewItemID, int32 NewQuantity);

	UFUNCTION()
	void OnRep_ItemData();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	bool Server_HandlePickup_Validate(AThirdPersonMPCharacter* InteractingCharacter) { return true; }

protected:

	//////////////

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
