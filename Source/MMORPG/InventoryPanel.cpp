// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPanel.h"
#include "InventoryComponent.h"
#include "InventoryItem.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"


void UInventoryPanel::NativeConstruct()
{
	Super::NativeConstruct();
}


void UInventoryPanel::PopulateInventory()
{
	if (!GridPanel || !LinkedInventory) return;

	GridPanel->ClearChildren();
	int Row = 0;
	int Col = 0;

	for (UItemBase* Item : LinkedInventory->Items)
	{
		if (Item)
		{
			UInventoryItem* InventoryItem = CreateWidget<UInventoryItem>(this, UInventoryItem::StaticClass());
			if (InventoryItem)
			{
				InventoryItem->SetItemData(Item);

				UUniformGridSlot* GridSlot = GridPanel->AddChildToUniformGrid(InventoryItem, Row, Col);

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
		InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UInventoryPanel::PopulateInventory);
	}
}



