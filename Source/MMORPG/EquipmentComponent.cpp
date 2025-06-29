// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "ItemBase.h"
#include "BasicHUD.h"
#include <Kismet/GameplayStatics.h>
#include "MyPlayerController.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"

UEquipmentComponent::UEquipmentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    EquippedWeapon = nullptr;
    EquippedHelmet = nullptr;
    EquippedArmor = nullptr;
    EquippedGauntlets = nullptr;
    EquippedBoots = nullptr;
}

UEquipmentComponent::~UEquipmentComponent()
{

}


void UEquipmentComponent::EquipItem_Internal(EEquipmentSlot Slot, UItemBase* Item)
{

    if (Item)
    {
        if (Item->WeaponAttackMontage)
        {
            UE_LOG(LogTemp, Warning, TEXT("[EquipItem] WeaponAttackMontage: %s"), *Item->WeaponAttackMontage->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("[EquipItem] WeaponAttackMontage ESTE NULL la echipare pentru item: %s"), *Item->ItemID.ToString());
        }
    }

    switch (Slot)
    {
    case EEquipmentSlot::Weapon:
        EquippedWeapon = Item;

        if (AThirdPersonMPCharacter* OwnerChar = Cast<AThirdPersonMPCharacter>(GetOwner()))
        {
            if (OwnerChar->HasAuthority())
            {
                UE_LOG(LogTemp, Warning, TEXT("[EquipItem] HasAuthority TRUE"));
                OwnerChar->EquipWeaponFromItem(Item);
                OwnerChar->MulticastEquipWeaponFromItem(Item->ItemID);
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("[EquipItem] HasAuthority FALSE - calling ServerEquipWeaponFromItem"));
                OwnerChar->ServerEquipWeaponFromItem(Item->ItemID);
            }
        }

  

        break;
    case EEquipmentSlot::Helmet:
        EquippedHelmet = Item;
        break;
    case EEquipmentSlot::Armor:
        EquippedArmor = Item;
        break;
    case EEquipmentSlot::Gauntlets:
        EquippedGauntlets = Item;
        break;
    case EEquipmentSlot::Boots:
        EquippedBoots = Item;
        break;
    }

    if (Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("Equipped %s in slot %s"), *Item->ItemID.ToString(), *UEnum::GetValueAsString(Slot));
    }

    // update UI
    if (APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
    {
        if (AMyPlayerController* MPC = Cast<AMyPlayerController>(PC))
        {
            if (UBasicHUD* HUD = MPC->GetBasicHUDWidget())
            {
                if (HUD->EquipmentPanel)
                {
                    HUD->EquipmentPanel->UpdatePanel(this);
                }
            }
        }
    }


    UE_LOG(LogTemp, Warning, TEXT("[EquipItem] Inainte de RecalculateEquipmentStats pe server sau client"));
    RecalculateEquipmentStats();
    UE_LOG(LogTemp, Warning, TEXT("[EquipItem] Dupa RecalculateEquipmentStats"));

    if (!GetOwner()->HasAuthority())
    {
        // daca e deja pe client, se ruleaza local
        UE_LOG(LogTemp, Warning, TEXT("[EquipItem] Nu am authority, rulez local RecalculateEquipmentStats()"));
        ServerRequestRecalculateEquipmentStats();
    }
    else
    {
        // daca e pe server, se trimite RPC doar catre clientul owner
        UE_LOG(LogTemp, Warning, TEXT("[EquipItem] Am authority, trimit ClientRecalculateEquipmentStats()"));
        RecalculateEquipmentStats();
    }

}

void UEquipmentComponent::UnequipItem(EEquipmentSlot Slot)
{
    switch (Slot)
    {
    case EEquipmentSlot::Weapon:
        EquippedWeapon = nullptr;
        break;
    case EEquipmentSlot::Helmet:
        EquippedHelmet = nullptr;
        break;
    case EEquipmentSlot::Armor:
        EquippedArmor = nullptr;
        break;
    case EEquipmentSlot::Gauntlets:
        EquippedGauntlets = nullptr;
        break;
    case EEquipmentSlot::Boots:
        EquippedBoots = nullptr;
        break;
    }


    // update UI
    if (APlayerController* PC = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
    {
        if (AMyPlayerController* MPC = Cast<AMyPlayerController>(PC))
        {
            if (UBasicHUD* HUD = MPC->GetBasicHUDWidget())
            {
                if (HUD->EquipmentPanel)
                {
                    HUD->EquipmentPanel->UpdatePanel(this);
                }
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Unequipped item from slot %s"), *UEnum::GetValueAsString(Slot));


    RecalculateEquipmentStats();

    if (!GetOwner()->HasAuthority())
    {
        // daca e deja pe client, se ruleaza local
        RecalculateEquipmentStats();
    }
    else
    {
        // daca e pe server, se trimite RPC doar catre clientul owner
        ClientRecalculateEquipmentStats();
    }
}

UItemBase* UEquipmentComponent::GetEquippedItem(EEquipmentSlot Slot) const
{
    switch (Slot)
    {
    case EEquipmentSlot::Weapon:
        return EquippedWeapon;
    case EEquipmentSlot::Helmet:
        return EquippedHelmet;
    case EEquipmentSlot::Armor:
        return EquippedArmor;
    case EEquipmentSlot::Gauntlets:
        return EquippedGauntlets;
    case EEquipmentSlot::Boots:
        return EquippedBoots;
    }
    return nullptr;
}


EWeaponType UEquipmentComponent::GetEquippedWeaponType() const
{
    if (EquippedWeapon)
    {
        return EquippedWeapon->WeaponType;
    }

    return EWeaponType::None;
}


void UEquipmentComponent::RecalculateEquipmentStats()
{
    UE_LOG(LogTemp, Warning, TEXT("[RecalculateEquipmentStats] Called on %s"), *GetOwner()->GetName());

    StatsTotals = FEquipmentStatTotals();

    TArray<UItemBase*> Items = {
      EquippedWeapon,
      EquippedHelmet,
      EquippedArmor,
      EquippedGauntlets,
      EquippedBoots
    };

    for (UItemBase* Item : Items)
    {
        if (!Item) continue;

        UE_LOG(LogTemp, Warning, TEXT("[RecalculateEquipmentStats] SERVER EquippedItem=%s"), *Item->ItemID.ToString());

        StatsTotals.BonusMaxHealth += Item->ItemStatistics.MaxHealth;
        StatsTotals.BonusHealthRegen += Item->ItemStatistics.HealthRegen;
        StatsTotals.BonusMaxMana += Item->ItemStatistics.MaxMana;
        StatsTotals.BonusManaRegen += Item->ItemStatistics.ManaRegen;
        StatsTotals.BonusMaxSkillStamina += Item->ItemStatistics.MaxSkillStamina;
        StatsTotals.BonusSkillStaminaRegen += Item->ItemStatistics.SkillStaminaRegen;
        StatsTotals.BonusMaxMovementStamina += Item->ItemStatistics.MaxMovementStamina;
        StatsTotals.BonusMovementStaminaRegen += Item->ItemStatistics.MovementStaminaRegen;

        StatsTotals.BonusPhysicalAttack += Item->ItemStatistics.PhysicalAttack;
        StatsTotals.BonusPhysicalDefense += Item->ItemStatistics.PhysicalDefense;
        StatsTotals.BonusMagicalAttack += Item->ItemStatistics.MagicalAttack;
        StatsTotals.BonusMagicalDefense += Item->ItemStatistics.MagicalDefense;

        StatsTotals.BonusStrength += Item->ItemStatistics.Strength;
        StatsTotals.BonusConstitution += Item->ItemStatistics.Constitution;
        StatsTotals.BonusDexterity += Item->ItemStatistics.Dexterity;
        StatsTotals.BonusIntelligence += Item->ItemStatistics.Intelligence;
        StatsTotals.BonusWisdom += Item->ItemStatistics.Wisdom;
        StatsTotals.BonusLuck += Item->ItemStatistics.Luck;   
    }

    UE_LOG(LogTemp, Warning, TEXT("[RecalculateEquipmentStats] TEST BonusPhysicalAttack=%f"), StatsTotals.BonusPhysicalAttack);

    if (GetOwner()->HasAuthority())
    {
        StatsTotals.BonusPhysicalAttack += 1.0f; // FORCE CHANGE for replication test
        GetOwner()->ForceNetUpdate();
    }

    if (AThirdPersonMPCharacter* Char = Cast<AThirdPersonMPCharacter>(GetOwner()))
    {
        Char->UpdateAll();
    }

    if (GetOwner()->HasAuthority())
    {
        UE_LOG(LogTemp, Warning, TEXT("[RecalculateEquipmentStats] SERVER FINAL StatsTotals: PhysAttack=%f PhysDefense=%f"),
            StatsTotals.BonusPhysicalAttack, StatsTotals.BonusPhysicalDefense);

        GetOwner()->ForceNetUpdate();
    }


}


void UEquipmentComponent::ClientRecalculateEquipmentStats_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("[ClientRecalculateEquipmentStats] Called on client %s"), *GetOwner()->GetName());
    RecalculateEquipmentStats();
}


void UEquipmentComponent::OnRep_StatsTotals()
{
    UE_LOG(LogTemp, Warning, TEXT("[OnRep_StatsTotals] Called on %s"), *GetOwner()->GetName());
    RecalculateEquipmentStats();
}


void UEquipmentComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UEquipmentComponent, StatsTotals);
}


void UEquipmentComponent::ServerRequestRecalculateEquipmentStats_Implementation()
{
    UE_LOG(LogTemp, Warning, TEXT("[ServerRequestRecalculateEquipmentStats] called on server"));

    RecalculateEquipmentStats(); // server calculeaza statusurile

  
}


void UEquipmentComponent::ServerEquipItem_Implementation(EEquipmentSlot Slot, FName ItemID)
{
    AThirdPersonMPCharacter* OwnerChar = Cast<AThirdPersonMPCharacter>(GetOwner());
    if (!OwnerChar || !OwnerChar->WeaponDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("[ServerEquipItem] Nu am OwnerChar sau ItemDataTable"));
        return;
    }


    // cautare in DataTable
    const FItemData* DataTableRow = OwnerChar->WeaponDataTable->FindRow<FItemData>(ItemID, TEXT(""));
    if (!DataTableRow)
    {
        UE_LOG(LogTemp, Error, TEXT("[ServerEquipItem] ItemID %s not found in DataTable!"), *ItemID.ToString());
        return;
    }

    // creare un nou UItemBase pe server
    UItemBase* NewItem = NewObject<UItemBase>(UItemBase::StaticClass());
    NewItem->ItemID = DataTableRow->ItemID;
    NewItem->WeaponAttackMontage = DataTableRow->WeaponAttackMontage;
    NewItem->AssetData = DataTableRow->AssetData;
    NewItem->NumericData = DataTableRow->NumericData;
    NewItem->ItemType = DataTableRow->ItemType;
    NewItem->WeaponType = DataTableRow->WeaponType;
    NewItem->ItemStatistics = DataTableRow->ItemStatistics;
    NewItem->ItemQuality = DataTableRow->ItemQuality;
    NewItem->TextData = DataTableRow->TextData;
    NewItem->AssetData = DataTableRow->AssetData;
    NewItem->Cooldown = DataTableRow->Cooldown;
 
    EquipItem_Internal(Slot, NewItem);
}


void UEquipmentComponent::EquipItem(EEquipmentSlot Slot, UItemBase* Item)
{
    if (!GetOwner()->HasAuthority())
    {
        ServerEquipItem(Slot, Item->ItemID);
        return;
    }

    EquipItem_Internal(Slot, Item);
}


