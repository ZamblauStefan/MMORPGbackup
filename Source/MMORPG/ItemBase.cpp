
#include "ItemBase.h"
#include "Net/UnrealNetwork.h"

UItemBase::UItemBase()
{

}

void UItemBase::Multicast_SyncItemData_Implementation(FName NewItemID, int32 NewQuantity, FItemAssetData NewAssetData)
{
	SyncItemData(NewItemID, NewQuantity, NewAssetData);
}

void UItemBase::SyncItemData(FName NewItemID, int32 NewQuantity, FItemAssetData NewAssetData)
{
	UE_LOG(LogTemp, Warning, TEXT("[ItemBase] Sincronizare manuala pe client pentru %s"), *NewItemID.ToString());

	ItemID = NewItemID;
	Quantity = NewQuantity;
	AssetData = NewAssetData;
}

void UItemBase::OnRep_ItemData()
{
	UE_LOG(LogTemp, Warning, TEXT("[ItemBase] Datele au fost sincronizate pe client pentru %s!"), *ItemID.ToString());
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
	/*
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
	*/
}

void UItemBase::Use(AThirdPersonMPCharacter* Character)
{

}

void UItemBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemBase, ItemID);
	DOREPLIFETIME(UItemBase, Quantity);
	DOREPLIFETIME(UItemBase, ItemType);
	DOREPLIFETIME(UItemBase, ItemQuality);
	DOREPLIFETIME(UItemBase, ItemStatistics);
	DOREPLIFETIME(UItemBase, TextData);
	DOREPLIFETIME(UItemBase, NumericData);
	DOREPLIFETIME(UItemBase, AssetData);
	//DOREPLIFETIME(UItemBase, OwningInventory);
	DOREPLIFETIME(UItemBase, LevelRequirement);
	DOREPLIFETIME(UItemBase, Tags);
	DOREPLIFETIME(UItemBase, SpawnableActorClass);
	DOREPLIFETIME(UItemBase, MaxDurability);
	DOREPLIFETIME(UItemBase, Cooldown);
	//DOREPLIFETIME(UItemBase, OnUse);



}





