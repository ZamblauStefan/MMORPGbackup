// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"
#include "ItemBase.h"


void UInventoryItemWidget::InitializeItem(UItemBase* Item)
{
    if (!Item) return;

    ItemData = Item;

    if (Quantity)
        Quantity->SetText(FText::AsNumber(Item->Quantity));

    if (ItemIcon && Item->AssetData.Icon)
        ItemIcon->SetBrushFromTexture(Item->AssetData.Icon);

}


