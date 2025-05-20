// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"
#include "ItemBase.h"


void UInventoryItemWidget::InitItem(UItemBase* Item)
{
    if (!Item || !ItemIcon || !Quantity)
    {
        UE_LOG(LogTemp, Warning, TEXT("[InventoryItemWidget] Invalid item or UI elements!"));
        return;
    }

    // Setam icon
    if (Item->AssetData.Icon)
    {
        ItemIcon->SetBrushFromTexture(Item->AssetData.Icon);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[InventoryItemWidget] Item %s has no icon!"), *Item->ItemID.ToString());
    }

    //ItemData = Item;

    // setam quantity
    Quantity->SetText(FText::AsNumber(Item->Quantity));
    Quantity->SetVisibility(Item->NumericData.bIsStackable ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

}


