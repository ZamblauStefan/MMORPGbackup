#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
//#include "ItemDataStructs.h"
#include "InventoryComponent.generated.h"

class UItemBase;
class AItemPickup;
struct FItemData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MMORPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, const TArray<UItemBase*>&, Items);

	// Sets default values for this component's properties (constructor)
	UInventoryComponent();

	void AddItemFromWorld(AItemPickup* ItemPickup);

	

	// Proprietate pentru inventory
	UPROPERTY( VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UItemBase>>  Items;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	//TArray<FName> ItemIDs;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 MaxItems = 20;

	// Delegate pentru actualizarea UI-ului 
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UItemBase* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UItemBase*> GetItems() const { return Items; }

	UFUNCTION(Client, Reliable)
	void Client_AddItem(const FItemData& ItemRow, int32 Quantity);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(UItemBase* Item, int32 Quantity);


	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


		
	
};
