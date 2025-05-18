#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMORPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties (constructor)
	UInventoryComponent();

	// Proprietate pentru inventory
	UPROPERTY( VisibleAnywhere, Category = "Inventory")
	TArray<UItemBase*> Items; 

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FName> ItemIDs;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 MaxItems = 20;

	// Delegate pentru actualizarea inventarului
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UItemBase* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UItemBase*> GetItems() const { return Items; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FName ItemID, int32 Quantity = 1);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UItemBase* GetItem(FName ItemID) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItem(FName ItemID, int32 Quantity = 1) const;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddItem(UItemBase* NewItem);
	void Server_AddItem_Implementation(UItemBase* NewItem);
	bool Server_AddItem_Validate(UItemBase* NewItem) { return true; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void DropItem(FName ItemID, int32 Quantity = 1);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DropItem(FName ItemID, int32 Quantity);
	void Server_DropItem_Implementation(FName ItemID, int32 Quantity);
	bool Server_DropItem_Validate(FName ItemID, int32 Quantity) { return true; }

	UItemBase* FindItemByID(FName ItemID) const;


	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


		
	
};
