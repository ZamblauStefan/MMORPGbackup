// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

UItemBase::UItemBase()
{

}

UItemBase* UItemBase::CreateItemCopy()
{
	UItemBase* ItemCopy = NewObject<UItemBase>(StaticClass());
	ItemCopy->ItemID = this->ItemID;
	ItemCopy->Quantity = this->Quantity;
	ItemCopy->ItemType = this->ItemType;
	ItemCopy->ItemQuality = this->ItemQuality;
	ItemCopy->ItemStatistics = this->ItemStatistics;
	ItemCopy->TextData = this->TextData;
	ItemCopy->NumericData = this->NumericData;
	ItemCopy->AssetData = this->AssetData;
	ItemCopy->LevelRequirement = this->LevelRequirement;
	ItemCopy->Tags = this->Tags;
	ItemCopy->SpawnableActorClass = this->SpawnableActorClass;
	ItemCopy->MaxDurability = this->MaxDurability;
	ItemCopy->Cooldown = this->Cooldown;
	ItemCopy->OnUse = this->OnUse;


	return ItemCopy;
}

void UItemBase::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
	{
		Quantity = FMath::Clamp(NewQuantity, 0 , NumericData.bIsStackable ? NumericData.MaxStack : 1);

		if (OwningInventory)
		{
			if (Quantity <= 0)
			{
				OwningInventory->RemoveItem(ItemID, Quantity);
			}
		}
	}
}

void UItemBase::Use(AThirdPersonMPCharacter* Character)
{

}
