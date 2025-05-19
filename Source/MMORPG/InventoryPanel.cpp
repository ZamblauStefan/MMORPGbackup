
#include "InventoryPanel.h"
#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "InventoryItemWidget.h"
#include "ItemBase.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void UInventoryPanel::BindToInventory(UInventoryComponent* InventoryComponent)
{
	LinkedInventory = InventoryComponent;
}

void UInventoryPanel::RefreshInventory(const TArray<class UItemBase*>& Items)
{

	if (!GridPanel || !ItemWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Inventory Panel] Problem with Grid/ItemWidget/InventoryComponent!"));
		return;

	}

	GridPanel->ClearChildren();

	int32 Row = 0;
	int32 Column = 0;
	const int32 ColumnsPerRow = 5;

	for (UItemBase* Item : Items)
	{
		if (UInventoryItemWidget* ItemWidget = CreateWidget<UInventoryItemWidget>(this, ItemWidgetClass))
		{
			ItemWidget->InitItem(Item);

			if (UUniformGridSlot* GridSlot = GridPanel->AddChildToUniformGrid(ItemWidget, Row, Column))
			{
				GridSlot->SetHorizontalAlignment(HAlign_Fill);
				GridSlot->SetVerticalAlignment(VAlign_Fill);

				UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] Processing item: %s"), *Item->ItemID.ToString());

				if (Item->AssetData.Icon)
				{
					UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] Item has Icon"));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] Item doesn't have Icon"));
				}
			}

			if (++Column >= ColumnsPerRow)
			{
				Column = 0;
				Row++;
			}
		}
	}
}

/*
void UInventoryPanel::BindToInventory(UInventoryComponent* InventoryComp)
{
	LinkedInventory = InventoryComp;

	// Refresh UI imediat dupa legare
	if (LinkedInventory)
	{
		RefreshInventory(LinkedInventory);

		// Optional: Asculta pentru modificari viitoare
		//LinkedInventory->OnInventoryUpdated.AddDynamic(this, &UInventoryPanel::RefreshInventory);
	}
}
*/

/*
void UInventoryPanel::InitializePanel(UInventoryComponent* InventoryComp)
{
	LinkedInventory = InventoryComp;
	LinkedInventory->OnInventoryUpdated.AddDynamic(this, &UInventoryPanel::RefreshInventory);
	RefreshInventory();
}


void UInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();
}


void UInventoryPanel::PopulateInventory(const TArray<UItemBase*>& Items)
{
	if (!IsValid(GridPanel))
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryPanel] GridPanel este NULL!"));
		return;
	}

	if (!IsValid(LinkedInventory))
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryPanel] LinkedInventory este NULL!"));
		return;
	}

	GridPanel->ClearChildren();

	// Daca nu avem iteme, iesim
	if (LinkedInventory->ItemIDs.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] Inventarul este gol."));
		return;
	}


	int32 Row = 0;
	int32 Col = 0;

	const int32 MaxColumns = 5; // Magic number inlocuit cu constanta

	UE_LOG(LogTemp, Log, TEXT("[InventoryPanel] Populam gridul cu %d iteme."), LinkedInventory->ItemIDs.Num());
	
	for (const FName& ItemID : LinkedInventory->ItemIDs)
	{

		if (ItemID.IsNone())
		{
			UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] ItemID invalid (IsNone)"));
			continue;
		}

		UItemBase* Item = LinkedInventory->FindItemByID(ItemID);
		if (!IsValid(Item))
		{
			UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] Itemul cu ID %s nu a fost gasit in inventar"), *ItemID.ToString());
			continue;
		}

		UE_LOG(LogTemp, Verbose, TEXT("[InventoryPanel] Procesam item: %s"), *Item->ItemID.ToString());

			// verificare item assetdata
			if (!Item->AssetData.Icon)
			{
				UE_LOG(LogTemp, Warning, TEXT("[InventoryPanel] Icon lipseste pentru %s - trebuie adaugd efault"), *Item->ItemID.ToString());
				// Item->AssetData.Icon = DefaultIcon;
			}
	


			UInventoryItem* InventoryItem = CreateWidget<UInventoryItem>(this, InventoryItemClass ? InventoryItemClass.Get() : UInventoryItem::StaticClass());
			if (!IsValid(InventoryItem))
			{

				UE_LOG(LogTemp, Error, TEXT("[InventoryPanel] Nu s-a putut crea widgetul InventoryItem pentru %s"), *Item->ItemID.ToString());
				continue;

			}

				// setare item data
				InventoryItem->SetItemData(Item);
				// adaugare in grid
				if (UUniformGridSlot* GridSlot = GridPanel->AddChildToUniformGrid(InventoryItem, Row, Col))
				{
					GridSlot->SetHorizontalAlignment(HAlign_Fill);
					GridSlot->SetVerticalAlignment(VAlign_Fill);

				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("[InventoryPanel] Nu s-a putut adauga itemul %s in grid"), *Item->ItemID.ToString());
				}

				// actualizare positie in grid
				Col++;
				if (Col >= MaxColumns)
				{
					Col = 0;
					Row++;
				}
		
	}



	UE_LOG(LogTemp, Log, TEXT("[InventoryPanel] Populare inventar finalizata."));
}


void UInventoryPanel::PopulateInventory(const TArray<UItemBase*>& Items)
{
	if (!GridPanel || !ItemWidgetClass) return;

	GridPanel->ClearChildren();

	int32 Row = 0, Column = 0;
	for (UItemBase* Item : Items)
	{
		if (UInventoryItemWidget* ItemWidget = CreateWidget<UInventoryItemWidget>(this, ItemWidgetClass))
		{
			ItemWidget->InitItem(Item); // Folosește InitItem

			if (UUniformGridSlot* NewSlot = GridPanel->AddChildToUniformGrid(ItemWidget, Row, Column))
			{
				NewSlot->SetHorizontalAlignment(HAlign_Fill);
				NewSlot->SetVerticalAlignment(VAlign_Fill);
			}

			Column++;
			if (Column >= 5)
			{
				Column = 0;
				Row++;
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

*/

