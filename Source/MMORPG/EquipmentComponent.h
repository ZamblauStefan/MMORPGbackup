#pragma once

#include "ItemBase.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipmentComponent.generated.h"

class ABasicHUD;

// structura pentru statusurile oferite in total de iteme
USTRUCT(BlueprintType)
struct FEquipmentStatTotals
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float BonusMaxHealth = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusHealthRegen = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusHealthRegenInterval = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusMaxMana = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusManaRegen = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusManaRegenInterval = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusMaxSkillStamina = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusSkillStaminaRegen = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusSkillStaminaRegenInterval = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusMaxMovementStamina = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusMovementStaminaRegen = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusMovementStaminaRegenInterval = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusPhysicalAttack = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusMagicalAttack = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusPhysicalDefense = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusMagicalDefense = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusStrength = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusConstitution = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusDexterity = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusIntelligence = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusWisdom = 0.f;
    UPROPERTY(BlueprintReadOnly)
    float BonusLuck = 0.f;


};

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
    Weapon,
    Helmet,
    Armor,
    Gauntlets,
    Boots
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MMORPG_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UEquipmentComponent();
	~UEquipmentComponent();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    class UItemBase* EquippedWeapon;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    class UItemBase* EquippedHelmet;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    class UItemBase* EquippedArmor;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    class UItemBase* EquippedGauntlets;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    class UItemBase* EquippedBoots;
    UPROPERTY()
    FEquipmentStatTotals StatsTotals;

    UPROPERTY()
    AStaticMeshActor* EquippedWeaponActor;

    UFUNCTION(BlueprintCallable)
    void EquipItem(EEquipmentSlot Slot, UItemBase* Item);

    UFUNCTION(BlueprintCallable, Category = "Equipment")
    EWeaponType GetEquippedWeaponType() const;

    UFUNCTION(BlueprintCallable)
    void UnequipItem(EEquipmentSlot Slot);

    UFUNCTION(BlueprintCallable)
    UItemBase* GetEquippedItem(EEquipmentSlot Slot) const;

    void RecalculateEquipmentStats();
};

