#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "ItemPickup.h"
#include "ItemDataStructs.generated.h"


UENUM()
enum class EItemQuality : uint8
{
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary"),
	Unique UMETA(DisplayName = "Unique"),
	Mythical UMETA(DisplayName = "Mythical"),
	Godly UMETA(DisplayName = "Godly"),
	Demonic UMETA(DisplayName = "Demonic")
};


UENUM()
enum class EItemType : uint8
{
	Armor UMETA(DisplayName = "Armor"),
	Weapon UMETA(DisplayName = "Weapon"),
	Shield UMETA(DisplayName = "Shield"),
	Spell UMETA(DisplayName = "Spell"),
	Consumable UMETA(DisplayName = "Consumable"),
	Food UMETA(DisplayName = "Food"),
	Potion UMETA(DisplayName = "Potion"),
	QuestItem UMETA(DisplayName = "QuestItem"),
	Miscellaneous UMETA(DisplayName = "Miscellaneous")
};

USTRUCT()
struct FItemStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float MaxHealth;
	UPROPERTY(EditAnywhere)
	float HealthRegen;
	UPROPERTY(EditAnywhere)
	float HealthRestoreAmount;
	UPROPERTY(EditAnywhere)
	float MaxMana;
	UPROPERTY(EditAnywhere)
	float ManaRegen;
	UPROPERTY(EditAnywhere)
	float ManaRestoreAmount;
	UPROPERTY(EditAnywhere)
	float MaxSkillStamina;
	UPROPERTY(EditAnywhere)
	float SkillStaminaRegen;
	UPROPERTY(EditAnywhere)
	float SkillStaminaRestoreAmount;
	UPROPERTY(EditAnywhere)
	float MaxMovementStamina;
	UPROPERTY(EditAnywhere)
	float MovementStaminaRegen;
	UPROPERTY(EditAnywhere)
	float MovementStaminaRestoreAmount;
	UPROPERTY(EditAnywhere)
	float PhysicalAttack;
	UPROPERTY(EditAnywhere)
	float MagicalAttack;
	UPROPERTY(EditAnywhere)
	float PhysicalDefense;
	UPROPERTY(EditAnywhere)
	float MagicalDefense;
	UPROPERTY(EditAnywhere)
	int Strength;
	UPROPERTY(EditAnywhere)
	int Constitution;
	UPROPERTY(EditAnywhere)
	int Dexterity;
	UPROPERTY(EditAnywhere)
	int Intelligence;
	UPROPERTY(EditAnywhere)
	int Wisdom;
	UPROPERTY(EditAnywhere)
	int Luck;
	UPROPERTY(EditAnywhere)
	float Accuracy;
	UPROPERTY(EditAnywhere)
	float Evasion;
	UPROPERTY(EditAnywhere)
	float CriticalHitRate;
	UPROPERTY(EditAnywhere)
	float CriticalHitDamage;

};

USTRUCT()
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	FText DisplayName;
	UPROPERTY(EditAnywhere)
	FText Description;
	UPROPERTY(EditAnywhere)
	FText InteractionText;
	UPROPERTY(EditAnywhere)
	FText UsageText;

};

USTRUCT()
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon;
	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh;

	// sound later
};

USTRUCT()
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere)
	float Weight;
	UPROPERTY(EditAnywhere)
	int32 MaxStack;
	UPROPERTY(EditAnywhere)
	bool bIsStackable;
	UPROPERTY(EditAnywhere)
	int32 BuyPrice;
	UPROPERTY(EditAnywhere)
	int32 SellPrice;
	
};

USTRUCT()
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Item Data")
	FName ItemID;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemType ItemType;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	EItemQuality ItemQuality;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemStatistics ItemStatistics;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemTextData TextData;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemNumericData NumericData;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FItemAssetData AssetData;

	// requirements
	UPROPERTY(EditAnywhere, Category = "Item Data")
	int32 LevelRequirement = 1;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	FGameplayTagContainer Tags;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	TSubclassOf<AItemPickup> SpawnableActorClass;

	// durability / cooldown
	UPROPERTY(EditAnywhere, Category = "Item Data")
	int32 MaxDurability;
	UPROPERTY(EditAnywhere, Category = "Item Data")
	float Cooldown;

	// delegate pentru use
	DECLARE_DELEGATE_OneParam(FOnUseItem, AActor* /* User */);
	FOnUseItem OnUse;


};