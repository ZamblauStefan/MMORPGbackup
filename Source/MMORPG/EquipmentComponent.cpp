// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "ItemBase.h"
#include "BasicHUD.h"
#include <Kismet/GameplayStatics.h>
#include "MyPlayerController.h"
#include "Engine/StaticMeshActor.h"

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


void UEquipmentComponent::EquipItem(EEquipmentSlot Slot, UItemBase* Item)
{
    switch (Slot)
    {
    case EEquipmentSlot::Weapon:
        EquippedWeapon = Item;

        // echipare weapon fizic pe caracter
        if (Item && Item->AssetData.Mesh)
        {
            AThirdPersonMPCharacter* OwnerChar = Cast<AThirdPersonMPCharacter>(GetOwner());
            if (OwnerChar)
            {
                USkeletalMeshComponent* MeshComp = OwnerChar->GetMesh();

                // Daca exista deja o arma echipata, o distrugem
                if (EquippedWeaponActor)
                {
                    EquippedWeaponActor->Destroy();
                    EquippedWeaponActor = nullptr;
                }

                // Spawnam noua arma
                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = OwnerChar;

                AStaticMeshActor* NewWeapon = OwnerChar->GetWorld()->SpawnActor<AStaticMeshActor>(AStaticMeshActor::StaticClass(), SpawnParams);
                if (NewWeapon)
                {
                    NewWeapon->GetStaticMeshComponent()->SetStaticMesh(Item->AssetData.Mesh);
                    NewWeapon->AttachToComponent(MeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, FName("ik_hand_r"));
                    EquippedWeaponActor = NewWeapon;
                }
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

    // echipare weapon fizic pe caracter



    RecalculateEquipmentStats();
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

    if (AThirdPersonMPCharacter* Char = Cast<AThirdPersonMPCharacter>(GetOwner()))
    {
        Char->UpdateAll();
    }

}



