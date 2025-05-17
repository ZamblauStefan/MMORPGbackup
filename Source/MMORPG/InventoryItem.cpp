// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ItemTooltip.h"
#include "ItemBase.h"
#include "Kismet/GameplayStatics.h"


void UInventoryItem::SetItemData(UItemBase* NewItem)
{
	if (!NewItem)
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryItem] NewItem este NULL!"));
		return;
	}

	if (!NewItem->AssetData.Icon)
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryItem] Icon-ul pentru item este NULL!"));
		return;
	}



	ItemData = NewItem;

	if (ItemData)
	{
		// Setam iconita si cantitatea
		ItemIcon->SetBrushFromTexture(ItemData->AssetData.Icon);
		ItemQuantity->SetText(FText::AsNumber(ItemData->Quantity));
		ItemQuantity->SetVisibility(ItemData->Quantity > 1 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UInventoryItem::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (ItemData)
	{
		// Cream tooltip-ul doar daca nu exista deja
		if (!Tooltip)
		{
			Tooltip = CreateWidget<UItemTooltip>(GetWorld(), UItemTooltip::StaticClass());
			Tooltip->AddToViewport();
			Tooltip->SetItemDetails(ItemData);
		}

		// Setam pozitia mouse-ului
		FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
		Tooltip->SetPositionInViewport(MousePosition + FVector2D(10, 10));
		Tooltip->SetVisibility(ESlateVisibility::Visible);
	}
}

void UInventoryItem::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (Tooltip)
	{
		Tooltip->SetVisibility(ESlateVisibility::Collapsed);
	}
}
