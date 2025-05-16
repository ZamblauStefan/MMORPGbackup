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
	UPROPERTY(EditAnywhere, Category = "Item")
	int32 Quantity;
	UPROPERTY(EditAnywhere, Category = "Item")
	FName ItemID;
	UPROPERTY(EditAnywhere, Category = "Item")
	UInventoryComponent* OwningInventory;
	UPROPERTY(EditAnywhere, Category = "Item")
	EItemType ItemType;
	UPROPERTY(EditAnywhere, Category = "Item")
	EItemQuality ItemQuality;
	UPROPERTY(EditAnywhere, Category = "Item")
	FItemStatistics ItemStatistics;
	UPROPERTY(EditAnywhere, Category = "Item")
	FItemTextData TextData;
	UPROPERTY(EditAnywhere, Category = "Item")
	FItemNumericData NumericData;
	UPROPERTY(EditAnywhere, Category = "Item")
	FItemAssetData AssetData;

	// requirements
	UPROPERTY(EditAnywhere, Category = "Item")
	int32 LevelRequirement = 0;
	UPROPERTY(EditAnywhere, Category = "Item")
	FGameplayTagContainer Tags;
	UPROPERTY(EditAnywhere, Category = "Item")
	TSubclassOf<AItemPickup> SpawnableActorClass;

	// durability / cooldown
	UPROPERTY(EditAnywhere, Category = "Item")
	int32 MaxDurability;
	UPROPERTY(EditAnywhere, Category = "Item")
	float Cooldown;

	// delegate pentru use
	DECLARE_DELEGATE_OneParam(FOnUseItem, AActor* /* User */);
	FOnUseItem OnUse;

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


protected:

	// functie ce perimite cautarea in TArray ( inventory) dupa un ID
	bool operator ==(const FName& OtherID) const
	{
		return this->ItemID == OtherID;
	}

	
};
