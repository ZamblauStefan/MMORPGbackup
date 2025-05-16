// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "WB_ItemTooltip.h"
#include "ItemBase.h"
#include "Kismet/GameplayStatics.h"



void UWB_InventoryItem::SetItemData(UItemBase* NewItem)
{
	ItemData = NewItem;

	if (ItemData)
	{
		// Setam iconita si cantitatea
		ItemIcon->SetBrushFromTexture(ItemData->AssetData.Icon);
		ItemQuantity->SetText(FText::AsNumber(ItemData->Quantity));
		ItemQuantity->SetVisibility(ItemData->Quantity > 1 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UWB_InventoryItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (ItemData)
	{
		// Cream tooltip-ul doar daca nu exista deja
		if (!Tooltip)
		{
			Tooltip = CreateWidget<UWB_ItemTooltip>(GetWorld(), UWB_ItemTooltip::StaticClass());
			Tooltip->AddToViewport();
			Tooltip->SetItemDetails(ItemData);
		}

		// Setam pozitia mouse-ului
		FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
		Tooltip->SetPositionInViewport(MousePosition + FVector2D(10, 10));
		Tooltip->SetVisibility(ESlateVisibility::Visible);
	}
}

void UWB_InventoryItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (Tooltip)
	{
		Tooltip->SetVisibility(ESlateVisibility::Collapsed);
	}
}
