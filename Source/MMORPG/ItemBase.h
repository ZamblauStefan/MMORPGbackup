// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataStructs.h"
#include "ThirdPersonMPCharacter.h"
#include "InventoryComponent.h"
#include "ItemBase.generated.h"


/**
 * 
 */
UCLASS()
class MMORPG_API UItemBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	int32 Quantity = 0;
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	FName ItemID;
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	UInventoryComponent* OwningInventory;
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	EItemType ItemType;
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	EItemQuality ItemQuality;
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	FItemStatistics ItemStatistics;
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	FItemTextData TextData;
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	FItemNumericData NumericData;
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	FItemAssetData AssetData;

	// requirements
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	int32 LevelRequirement = 0;
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	FGameplayTagContainer Tags;
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	TSubclassOf<AItemPickup> SpawnableActorClass;

	// durability / cooldown
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	int32 MaxDurability;
	UPROPERTY(Replicated, EditAnywhere, Category = "Item")
	float Cooldown;

	// delegate pentru use
	DECLARE_DELEGATE_OneParam(FOnUseItem, AActor* /* User */);
	FOnUseItem OnUse;

public:
	UItemBase();

	UFUNCTION(Category = "Item")
	UItemBase* CreateItemCopy();
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemStackWeight() const { return Quantity * NumericData.Weight; };
	UFUNCTION(Category = "Item")
	FORCEINLINE float GetItemSingleWeight() const { return NumericData.Weight; };
	UFUNCTION(Category = "Item")
	FORCEINLINE bool IsFullItemStack() const { return Quantity == NumericData.MaxStack; };
	UFUNCTION(Category = "Item")
	void SetQuantity(const int32 NewQuantity);

	virtual void Use(AThirdPersonMPCharacter* Character);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION()
	void OnRep_ItemData(); // Sincronizare client-server

	// functii pentru sincronizare client-server
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SyncItemData(FName NewItemID, int32 NewQuantity, FItemAssetData NewAssetData);

	void SyncItemData(FName NewItemID, int32 NewQuantity, FItemAssetData NewAssetData);

protected:

	// functie ce perimite cautarea in TArray ( inventory) dupa un ID
	bool operator ==(const FName& OtherID) const
	{
		return this->ItemID == OtherID;
	}

	
};
