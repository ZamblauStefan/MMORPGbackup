
#include "EquipmentPanel.h"
#include "EquipmentComponent.h"
#include "ItemBase.h"
#include "EquipmentSlot.h"

void UEquipmentPanel::UpdatePanel(UEquipmentComponent* Equipment)
{
    if (!Equipment) return;

    if (WeaponSlot)
        WeaponSlot->SetSlot(Equipment->GetEquippedItem(EEquipmentSlot::Weapon));

    if (HelmetSlot)
        HelmetSlot->SetSlot(Equipment->GetEquippedItem(EEquipmentSlot::Helmet));

    if (ArmorSlot)
        ArmorSlot->SetSlot(Equipment->GetEquippedItem(EEquipmentSlot::Armor));

    if (GauntletsSlot)
        GauntletsSlot->SetSlot(Equipment->GetEquippedItem(EEquipmentSlot::Gauntlets));

    if (BootsSlot)
        BootsSlot->SetSlot(Equipment->GetEquippedItem(EEquipmentSlot::Boots));
}




