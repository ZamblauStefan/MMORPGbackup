
#include "InventoryPanel.h"
#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "ItemBase.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"


void UInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();
}


void UInventoryPanel::PopulateInventory()
{
	if (!IsValid(GridPanel) || !IsValid(LinkedInventory))
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryPanel] GridPanel sau LinkedInventory sunt NULL!"));
		return;
	}



	GridPanel->ClearChildren();
	int Row = 0;
	int Col = 0;

	UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] Populam gridul cu iteme."));

	for (FName ItemID : LinkedInventory->ItemIDs)
	{
		UItemBase* Item = LinkedInventory->FindItemByID(ItemID);

		if (Item)
		{
			UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] Am gasit item: %s"), *Item->ItemID.ToString());

			// verificare item.icon
			if (Item->AssetData.Icon)
			{
				UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] Icon setat pentru %s: %s"), *Item->ItemID.ToString(), *Item->AssetData.Icon->GetName());
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("[InventoryPanel] Icon LIPSESTE pentru %s"), *Item->ItemID.ToString());
			}


			UInventoryItem* InventoryItem = CreateWidget<UInventoryItem>(this, UInventoryItem::StaticClass());
			if (InventoryItem)
			{
				InventoryItem->SetItemData(Item);

				UUniformGridSlot* GridSlot = GridPanel->AddChildToUniformGrid(InventoryItem, Row, Col);
				GridSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
				GridSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

				Col++;
				if (Col >= 5)
				{
					Col = 0;
					Row++;
				}
			}
		}
	}
}


void UInventoryPanel::BindToInventory(UInventoryComponent* InventoryComponent)
{
	if (InventoryComponent)
	{
		LinkedInventory = InventoryComponent;
		bool bIsAlreadyBound = false;

		// Iteram prin delegate-uri sa vedem daca este legat deja
		for (const auto& DelegateInstance : InventoryComponent->OnInventoryUpdated.GetAllObjects())
		{
			if (DelegateInstance == this)
			{
				bIsAlreadyBound = true;
				break;
			}
		}

		if (!bIsAlreadyBound)
		{
			InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryPanel::PopulateInventory);
			UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] Delegate-ul pentru OnInventoryUpdated a fost legat!"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] Delegate-ul era deja legat, nu mai legam din nou!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryPanel] InventoryComponent este NULL la bind!"));
	}

}


void UInventoryPanel::NativeDestruct()
{
	Super::NativeDestruct();

	if (LinkedInventory && LinkedInventory->OnInventoryUpdated.IsBound())
	{
		LinkedInventory->OnInventoryUpdated.RemoveDynamic(this, &UInventoryPanel::PopulateInventory);
		UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] Delegate-ul a fost sters la destruct!"));
	}
}




