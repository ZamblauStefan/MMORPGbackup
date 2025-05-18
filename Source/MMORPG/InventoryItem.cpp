// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "ItemTooltip.h"
#include "ItemBase.h"
#include "Kismet/GameplayStatics.h"


void UInventoryItem::NativeConstruct()
{
	Super::NativeConstruct();

	// Verificam daca referintele sunt corecte
	if (!ItemIcon)
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryItem] ItemIcon nu este setat in Blueprint!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryItem] ItemIcon a fost setat corect!"));
	}

	if (!ItemQuantity)
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryItem] ItemQuantity nu este setat in Blueprint!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryItem] ItemQuantity a fost setat corect!"));
	}
}


void UInventoryItem::SetItemData(UItemBase* NewItem)
{
	if (!NewItem)
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryItem] NewItem este NULL!"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryItem] NewItem este valid: %s"), *NewItem->ItemID.ToString());
	}


	ItemData = NewItem;
	// Verificam datele
	if (!ItemData->AssetData.Icon)
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryItem] NewItem are Icon!"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryItem]  NewItem NU are Icon!"));
	}

	if (!ItemData->Quantity)
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryItem]  NewItem are Quantity!"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryItem] NewItem NU are Quantity valid!"));
	}

	// Verificam daca sunt valide referintele din Blueprint
	if (!ItemIcon)
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryItem] ItemIcon nu este setat în Blueprint!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryItem] ItemIcon este valid!"));
	}

	if (!ItemQuantity)
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryItem] ItemQuantity nu este setat în Blueprint!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryItem] ItemQuantity este valid!"));
	}

	if (ItemData)
	{
		if (ItemData->AssetData.Icon)
		{
			UE_LOG(LogTemp, Warning, TEXT("[InventoryItem] Setam icon-ul pentru %s!"), *NewItem->ItemID.ToString());
			ItemIcon->SetBrushFromTexture(NewItem->AssetData.Icon);
			//ItemIcon->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[InventoryItem] Icon-ul pentru %s este NULL!"), *NewItem->ItemID.ToString());
			ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
		}

		ItemQuantity->SetText(FText::AsNumber(NewItem->Quantity));
		ItemQuantity->SetVisibility(NewItem->Quantity > 1 ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
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
