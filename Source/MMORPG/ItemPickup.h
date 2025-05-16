// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemPickup.generated.h"

class AThirdPersonMPCharacter;

UCLASS()
class MMORPG_API AItemPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemPickup();

	UFUNCTION()
	void SetItemData(FName NewItemID, int32 NewQuantity);

	UFUNCTION()
	void OnRep_ItemData();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UPROPERTY(ReplicatedUsing = OnRep_ItemData)
	FName ItemID;

	UPROPERTY(ReplicatedUsing = OnRep_ItemData)
	int32 Quantity;

	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void OnInteract(AThirdPersonMPCharacter* InteractingCharacter);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_HandlePickup(AThirdPersonMPCharacter* InteractingCharacter);
	void Server_HandlePickup_Implementation(AThirdPersonMPCharacter* InteractingCharacter);
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
