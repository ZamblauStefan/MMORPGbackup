// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryItemWidget.h"
#include "ItemBase.h"
#include "ItemTooltip.h"
#include "ItemDataStructs.h"
#include "EquipmentComponent.h"
#include "ThirdPersonMPCharacter.h"
#include <Kismet/GameplayStatics.h>
#include "MyPlayerController.h"


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

    ItemData = Item;

    // setam quantity
    Quantity->SetText(FText::AsNumber(Item->Quantity));
    Quantity->SetVisibility(Item->NumericData.bIsStackable ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

}

void UInventoryItemWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
    UE_LOG(LogTemp, Warning, TEXT("Mouse entered inventory item widget"));

    if (!CurrentTooltip && TooltipClass)
    {
        if (!ItemData)
        {
            UE_LOG(LogTemp, Error, TEXT("ItemData is NULL in NativeOnMouseEnter!"));
            return;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("ItemData is valid: ItemID = %s"), *ItemData->ItemID.ToString());
        }

        UItemTooltip* Tooltip = CreateWidget<UItemTooltip>(GetWorld(), TooltipClass);
        if (Tooltip)
        {
            Tooltip->SetItemDetails(ItemData);
            Tooltip->AddToViewport(100);
            //FVector2D LocalMousePos = InMouseEvent.GetScreenSpacePosition();
            // pozitie Tooltip
            // FVector2D LocalMousePos;
            FVector2D MousePos;
            if (GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePos.X, MousePos.Y))
            {
                Tooltip->SetPositionInViewport(MousePos + FVector2D(10.f, 10.f));
            }
            //Tooltip->SetPositionInViewport(MousePos + FVector2D(10.f, 10.f));
            UE_LOG(LogTemp, Warning, TEXT("Tooltip created successfully."));
            CurrentTooltip = Tooltip;
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Tooltip is NULL after CreateWidget!"));
        }
    }
}

void UInventoryItemWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
    Super::NativeOnMouseLeave(InMouseEvent);

    if (CurrentTooltip)
    {
        UE_LOG(LogTemp, Warning, TEXT("Tooltip removed!"));
        CurrentTooltip->RemoveFromParent();
        CurrentTooltip = nullptr;
    }
}


void UInventoryItemWidget::NativePreConstruct()
{
    Super::NativePreConstruct();

    SetDesiredSizeInViewport(FVector2D(64.f, 64.f));
}

FReply UInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        UE_LOG(LogTemp, Warning, TEXT("Right click on item: %s"), *ItemData->ItemID.ToString());

        if (ItemData)
        {
            UseItem();
        }

        return FReply::Handled();
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}


void UInventoryItemWidget::UseItem()
{
    if (!ItemData) return;

    AThirdPersonMPCharacter* Player = Cast<AThirdPersonMPCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (!Player || !Player->GetEquipmentComponent()) return;

    switch (ItemData->ItemType)
    {
    case EItemType::Helmet:
        Player->GetEquipmentComponent()->EquipItem(EEquipmentSlot::Helmet, ItemData);
        break;

    case EItemType::Armor:
        Player->GetEquipmentComponent()->EquipItem(EEquipmentSlot::Armor, ItemData);
        break;

    case EItemType::Weapon:
        Player->GetEquipmentComponent()->EquipItem(EEquipmentSlot::Weapon, ItemData);
        break;

    case EItemType::Gloves:
        Player->GetEquipmentComponent()->EquipItem(EEquipmentSlot::Gauntlets, ItemData);
        break;

    case EItemType::Boots:
        Player->GetEquipmentComponent()->EquipItem(EEquipmentSlot::Boots, ItemData);
        break;

    case EItemType::Consumable:
        UE_LOG(LogTemp, Warning, TEXT("Used consumable: %s"), *ItemData->ItemID.ToString());
        // Logica de use
        break;
    case EItemType::Potion:
        UE_LOG(LogTemp, Warning, TEXT("Used Potion: %s"), *ItemData->ItemID.ToString());

        // se scade quantity, daca s-a terminat, se elimina din inventory
        if (ItemData->Quantity > 0)
        {
            if (Player->GetInventoryComponent())
            {
                Player->GetInventoryComponent()->RemoveItem(ItemData, 1);
                UE_LOG(LogTemp, Warning, TEXT("Potion %s used"), *ItemData->ItemID.ToString());

            }
        }
        else
        {
            break;
        }

        if (CurrentTooltip)
        {
            CurrentTooltip->RemoveFromParent();
            CurrentTooltip = nullptr;
            UE_LOG(LogTemp, Warning, TEXT("Tooltip removed after item used."));
        }

        float HPamount, MPamount, SPamount;
        HPamount = ItemData->ItemStatistics.HealthRestoreAmount;
        MPamount = ItemData->ItemStatistics.ManaRestoreAmount;
        SPamount = ItemData->ItemStatistics.SkillStaminaRestoreAmount;

        Player->Recovery(HPamount, MPamount, SPamount);

        break;

    default:
        UE_LOG(LogTemp, Warning, TEXT("UseItem: Unsupported item type %d"), (uint8)ItemData->ItemType);
        break;
    }
}







