
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "ItemPickup.generated.h"

//class AThirdPersonMPCharacter;
class UItemBase;
class USphereComponent;


UCLASS()
class MMORPG_API AItemPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemPickup();

	UPROPERTY(EditAnywhere, Category = "Item Data")
	UDataTable* ItemDataTable;  // Referinta catre DataTable-u (TestItems)

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ItemRowName = "test_001";  // Numele randului din tabel

	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<UItemBase> ItemClass;

	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	FName ItemID;
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	int32 Quantity;

	UPROPERTY(Replicated)
	bool bIsPickedUp = false;

	// Functie pentru replicare
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void OnRep_IsPickedUp();

	// Functie apelata cand jucatorul colecteaza itemul
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DestroyItem();

	// Data pentru interactiune
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	//FInteractableData InstanceInteractableData;

	// Referinta catre tabel
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	//UDataTable* ItemTable;

	//UFUNCTION(BlueprintCallable, Category = "Pickup")
	//virtual void Interact(AThirdPersonMPCharacter* InteractingCharacter) override;


	UFUNCTION()
	void SetItemData(FName NewItemID, int32 NewQuantity);

	UFUNCTION()
	void OnRep_ItemData();

	bool Server_HandlePickup_Validate(AThirdPersonMPCharacter* InteractingCharacter) { return true; }

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionSphere;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
