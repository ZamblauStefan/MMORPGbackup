
#include "EquipmentSlot.h"
#include "ItemBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "EquipmentComponent.h"
#include "ThirdPersonMPCharacter.h"


void UEquipmentSlot::SetSlot(UItemBase* Item)
{
    StoredItem = Item;

    if (Item)
    {
        if (IconImage)
        {
            IconImage->SetBrushFromTexture(Item->AssetData.Icon);
        }

        if (NameText)
        {
            NameText->SetText(FText::FromName(Item->ItemID));
        }
    }
    else
    {
        if (IconImage)
        {
            IconImage->SetBrushFromTexture(nullptr);
        }

        if (NameText)
        {
            NameText->SetText(FText::FromString("Empty"));
        }
    }
}

FReply UEquipmentSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
    {
        // caracterul
        APawn* Pawn = GetOwningPlayerPawn();
        if (Pawn)
        {
            AThirdPersonMPCharacter* Character = Cast<AThirdPersonMPCharacter>(Pawn);
            if (Character && Character->GetEquipmentComponent())
            {
                Character->GetEquipmentComponent()->UnequipItem(SlotType);
                UE_LOG(LogTemp, Warning, TEXT("Unequipped item from slot: %d"), static_cast<int32>(SlotType));
            }
        }

        return FReply::Handled();
    }

    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}


