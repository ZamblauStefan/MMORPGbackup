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


void UInventoryPanel::PopulateInventory(UInventoryComponent* InventoryComponent)
{
	if (!GridPanel) return;

	GridPanel->ClearChildren();
	int Row = 0;
	int Col = 0;

	for (UItemBase* Item : InventoryComponent->Items)
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





